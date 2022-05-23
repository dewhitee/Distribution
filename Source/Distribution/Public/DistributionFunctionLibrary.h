// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DistributionFunctionLibrary.generated.h"

class UCurveVector;

UENUM(BlueprintType)
enum class EDistributionDataType : uint8
{
	None,
	Sunflower,
	Spiral,
	Circular,
	CircularUniform,
	CircularUniform2,
	CustomCurve
};

USTRUCT(BlueprintType)
struct FDistributionData
{
	GENERATED_BODY()

	FDistributionData() {}
	FDistributionData(TArray<FVector> InPoints, EDistributionDataType InType)
		: Points(InPoints)
		, Type(InType) {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FVector> Points;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDistributionDataType Type = EDistributionDataType::None;
};

USTRUCT(BlueprintType)
struct FDistributionSunflowerArgs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AdjustedSize;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Alpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius;
};

USTRUCT(BlueprintType)
struct FDistributionSpiralArgs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AdjustedSize;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpiralCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpiralDensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpiralSize;
};

USTRUCT(BlueprintType)
struct FDistributionCircularArgs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AdjustedSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius;
};

USTRUCT(BlueprintType)
struct FDistributionCustomCurveArgs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AdjustedSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveVector* CustomVectorCurve = nullptr;
};

/**
 * 
 */
UCLASS()
class DISTRIBUTION_API UDistributionFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category=Distribution)
	static FTransform GetSingleInCircle(int32 Value);

	UFUNCTION(BlueprintCallable, Category=Distribution)
	static TArray<FTransform> TransformsFromDistribution(const FDistributionData& InDistribution);

	UFUNCTION(BlueprintCallable, Category=Distribution)
	static void RotateTransformsToPointAtLocation(UPARAM(ref) TArray<FTransform>& Transforms, const FVector Location);

	UFUNCTION(BlueprintCallable, Category=Distribution)
	static void RotateTransformsToPointFromLocation(UPARAM(ref) TArray<FTransform>& Transforms, const FVector Location);

	UFUNCTION(BlueprintCallable, Category=Distribution, meta=(WorldContext="WorldContextObject"))
	static void DrawDistribution(const UObject* WorldContextObject, const FDistributionData& Distribution,
		const FColor Color = FColor::Blue, float Lifetime = 5.f);

	UFUNCTION(BlueprintCallable, Category=Distribution)
	static FDistributionData DistributeSunflower(const FDistributionSunflowerArgs& Args);

	UFUNCTION(BlueprintCallable, Category=Distribution)
	static FDistributionData DistributeSpiral(const FDistributionSpiralArgs& Args);

	UFUNCTION(BlueprintCallable, Category=Distribution)
	static FDistributionData DistributeCircular(const FDistributionCircularArgs& Args);
	
	UFUNCTION(BlueprintCallable, Category=Distribution)
	static FDistributionData DistributeCircularUniform(const FDistributionCircularArgs& Args);

	UFUNCTION(BlueprintCallable, Category=Distribution)
	static FDistributionData DistributeCircularUniform2(const FDistributionCircularArgs& Args);

	UFUNCTION(BlueprintCallable, Category=Distribution)
	static FDistributionData DistributeCustomCurve(const FDistributionCustomCurveArgs& Args);

private:
	static float GetPolarRadius(int32 Index, const float Count, const int32 BoundaryPoints);
};
