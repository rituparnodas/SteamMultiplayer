// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveTriggers > 0)
	{
		if (HasAuthority()) // Not on Server == client
		{
			auto Location = GetActorLocation();  // Because TargetLocation is Local Needs To Global
			float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
			float JourneyTravelled = (Location - GlobalStartLocation).Size();
			if (JourneyTravelled >= JourneyLength)
			{
				FVector Swap = GlobalTargetLocation;
				GlobalTargetLocation = GlobalStartLocation;
				GlobalStartLocation = Swap;
			}
			auto Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal(); // B-A Vector
			Location += Speed * DeltaTime * Direction;
			SetActorLocation(Location);
		}
	}	
}

void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers++;
}

void AMovingPlatform::RemoveActveTrigger()
{
	if (ActiveTriggers > 0)
	{
		ActiveTriggers--;
	}
}