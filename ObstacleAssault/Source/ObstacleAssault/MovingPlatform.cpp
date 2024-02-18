// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();

	UE_LOG(LogTemp, Display, TEXT("Configured Moved Distance: %f"), MaxDistanceMoved);
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	MovePlatform(DeltaTime);
	RotatePlatform(DeltaTime);
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
	// Reverse direction if it has moved far enough
	if (ShouldPlatformReturn())
	{
		FVector MoveDirection = PlatformVelocity.GetSafeNormal();
		StartLocation = StartLocation + (MoveDirection * MaxDistanceMoved);
		SetActorLocation(StartLocation);
		PlatformVelocity *= -1;
	} else 
	{
		// Get current location
		FVector CurrentLocation = GetActorLocation();

		// Add vector to that location
		CurrentLocation = CurrentLocation + (PlatformVelocity * DeltaTime); 

		// Set the location
		SetActorLocation(CurrentLocation);
	}
}

void AMovingPlatform::RotatePlatform(float DeltaTime)
{
	AddActorLocalRotation(RotationVelocity * DeltaTime);
}

bool AMovingPlatform::ShouldPlatformReturn() const
{
	return GetDistanceMoved() > MaxDistanceMoved;
}

float AMovingPlatform::GetDistanceMoved() const
{
	return FVector::Distance(StartLocation, GetActorLocation());
}