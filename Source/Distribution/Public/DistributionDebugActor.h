// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DistributionFunctionLibrary.h"
#include "GameFramework/Actor.h"
#include "DistributionDebugActor.generated.h"

UCLASS()
class DISTRIBUTION_API ADistributionDebugActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADistributionDebugActor();

	UPROPERTY(EditAnywhere, Category=Distribution)
	EDistributionDataType DistributionType;

	UPROPERTY(EditAnywhere, Category=Distribution)
	FLinearColor PointColor = FLinearColor::Blue;
	
	UPROPERTY(EditAnywhere, Category=Distribution)
	float Lifetime = 5.f;
	
	UPROPERTY(EditAnywhere, Category=Distribution, meta=(EditCondition="DistributionType == EDistributionDataType::Sunflower"))
	FDistributionSunflowerArgs Sunflower;

	UPROPERTY(EditAnywhere, Category=Distribution, meta=(EditCondition="DistributionType == EDistributionDataType::Spiral"))
	FDistributionSpiralArgs Spiral;
	
	UPROPERTY(EditAnywhere, Category=Distribution, meta=(EditCondition="DistributionType == EDistributionDataType::Circular || DistributionType == EDistributionDataType::CircularUniform || DistributionType == EDistributionDataType::CircularUniform2"))
	FDistributionCircularArgs Circular;
	
	UPROPERTY(EditAnywhere, Category=Distribution, meta=(EditCondition="DistributionType == EDistributionDataType::CustomCurve"))
	FDistributionCustomCurveArgs CustomCurve;

	FTimerHandle TimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostLoad() override;
	virtual void PostActorCreated() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void Visualize();
	
};
