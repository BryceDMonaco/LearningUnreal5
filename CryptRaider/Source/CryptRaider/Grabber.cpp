// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	
	if (physicsHandle != nullptr)
	{
		FVector targetLocation = GetComponentLocation() + (GetForwardVector() * holdDistance);
		physicsHandle->SetTargetLocationAndRotation(targetLocation, GetComponentRotation());
	}
}

void UGrabber::Grab()
{
	FHitResult hitResult;

	if (GetGrabbableInReach(hitResult))
	{
		UPrimitiveComponent* hitComponent = hitResult.GetComponent();
		hitComponent->WakeAllRigidBodies();

		DrawDebugSphere(GetWorld(), hitResult.ImpactPoint, 10, 10, FColor::Blue, false, 5);
		UE_LOG(LogTemp, Display, TEXT("Hit: %s"), *(hitResult.GetActor()->GetActorNameOrLabel()));

		GetPhysicsHandle()->GrabComponentAtLocationWithRotation(hitComponent, NAME_None, hitResult.ImpactPoint, GetComponentRotation());
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	
	if (physicsHandle == nullptr || physicsHandle->GetGrabbedComponent() == nullptr)
	{
		return;
	}

	physicsHandle->ReleaseComponent();

	return;
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
		UPhysicsHandleComponent* physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

		if (physicsHandle == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("No physics handle!"));
		}

		return physicsHandle;
}

bool UGrabber::GetGrabbableInReach(FHitResult& outHitResult) const
{
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	
	if (physicsHandle == nullptr)
	{
		return false;
	}

	FVector start = GetComponentLocation();
	FVector end = start + (GetForwardVector() * maxGrabDistance);
	DrawDebugLine(GetWorld(), start, end, FColor::Red);

	FCollisionShape sphere = FCollisionShape::MakeSphere(grabRadius);

	return 	GetWorld()->SweepSingleByChannel(outHitResult, start, end, FQuat::Identity, ECC_GameTraceChannel2, sphere);
}