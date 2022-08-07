// Fill out your copyright notice in the Description page of Project Settings.


#include "DistributionDebugActor.h"

#include "DistributionFunctionLibrary.h"

// Sets default values
ADistributionDebugActor::ADistributionDebugActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADistributionDebugActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADistributionDebugActor::PostLoad()
{
	Super::PostLoad();
	if (!TimerHandle.IsValid())
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ADistributionDebugActor::Visualize, GetWorld()->GetDeltaSeconds(), true);
	}
}

void ADistributionDebugActor::PostActorCreated()
{
	Super::PostActorCreated();
	if (!TimerHandle.IsValid())
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ADistributionDebugActor::Visualize, GetWorld()->GetDeltaSeconds(), true);
	}
}

void ADistributionDebugActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (TimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ADistributionDebugActor::Visualize, GetWorld()->GetDeltaSeconds(), true);
	}
}

void ADistributionDebugActor::RestartVisualization()
{
	if (TimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ADistributionDebugActor::Visualize, GetWorld()->GetDeltaSeconds(), true);
	}
	else
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ADistributionDebugActor::Visualize, GetWorld()->GetDeltaSeconds(), true);
	}
}

// Called every frame
void ADistributionDebugActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#ifndef VISUALIZE
#define VISUALIZE(distribution) \
	case EDistributionDataType::##distribution##: \
		UDistributionFunctionLibrary::DrawDistribution(this, UDistributionFunctionLibrary::Distribute##distribution##(##distribution##), Color, Lifetime); \
		break;
#endif

void ADistributionDebugActor::Visualize()
{
	using UDFL = UDistributionFunctionLibrary;
	const FColor Color = PointColor.ToFColorSRGB();
	
	switch (DistributionType)
	{
	case EDistributionDataType::None: break;
	case EDistributionDataType::Sunflower:
		UDFL::DrawDistribution(this, UDFL::DistributeSunflower(Sunflower), Color, Lifetime);
		break;
	case EDistributionDataType::Spiral:
		UDFL::DrawDistribution(this, UDFL::DistributeSpiral(Spiral), Color, Lifetime);
		break;
	case EDistributionDataType::Circular:
		UDFL::DrawDistribution(this, UDFL::DistributeCircular(Circular), Color, Lifetime);
		break;
	case EDistributionDataType::CircularUniform:
		UDFL::DrawDistribution(this, UDFL::DistributeCircularUniform(Circular), Color, Lifetime);
		break;
	case EDistributionDataType::CircularUniform2:
		UDFL::DrawDistribution(this, UDFL::DistributeCircularUniform2(Circular), Color, Lifetime);
		break;
	case EDistributionDataType::CustomCurve:
		UDFL::DrawDistribution(this, UDFL::DistributeCustomCurve(CustomCurve), Color, Lifetime);
		break;
	VISUALIZE(Cone);
	VISUALIZE(Ring);
	default: ;
	}
}

#undef VISUALIZE
