// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* acceptableActor = GetAcceptableActor();
    if (acceptableActor != nullptr)
    {
        UPrimitiveComponent* component = Cast<UPrimitiveComponent>(acceptableActor->GetRootComponent());
        if (component != nullptr) {
            component->SetSimulatePhysics(false);
        }
        acceptableActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
    }
    SetShouldMove(acceptableActor != nullptr);
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
    TArray<AActor*> actors;
    GetOverlappingActors(actors);

    for (AActor* actor : actors)
    {
        if (actor->ActorHasTag(acceptableActorTag))
        {
            return actor;
        }
    }

    return nullptr;
}

void UTriggerComponent::SetMover(UMover* newMover)
{
    mover = newMover;
}

void UTriggerComponent::SetShouldMove(bool shouldMove)
{
    if (mover != nullptr)
    {
        mover->SetShouldMove(shouldMove);
    }
}
