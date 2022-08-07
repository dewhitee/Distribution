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
	CustomCurve,
	Cone,
	Ring
};

USTRUCT(BlueprintType)
struct DISTRIBUTION_API FDistributionData
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
struct DISTRIBUTION_API FDistributionSunflowerArgs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AdjustedSize = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Alpha = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 1.f;
};

USTRUCT(BlueprintType)
struct DISTRIBUTION_API FDistributionSpiralArgs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AdjustedSize = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpiralCoefficient = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpiralDensity = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpiralSize = 1.f;
};

USTRUCT(BlueprintType)
struct DISTRIBUTION_API FDistributionCircularArgs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AdjustedSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 1.f;
};

USTRUCT(BlueprintType)
struct DISTRIBUTION_API FDistributionCustomCurveArgs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AdjustedSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveVector* CustomVectorCurve = nullptr;
};

USTRUCT(BlueprintType)
struct DISTRIBUTION_API FDistributionConeArgs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Direction = FVector::ZAxisVector * 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, ClampMax=180))
	float HalfAngleInDegrees = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Distance = 100.f;
};

/**
 * @see https://en.wikipedia.org/wiki/Annulus_%28mathematics%29
 */
USTRUCT(BlueprintType)
struct DISTRIBUTION_API FDistributionRingArgs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InnerRadius = 128.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OuterRadius = 256.f;
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

	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Distribution)
	static FDistributionData DistributeSunflower(const FDistributionSunflowerArgs& Args);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Distribution)
	static FDistributionData DistributeSpiral(const FDistributionSpiralArgs& Args);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Distribution)
	static FDistributionData DistributeCircular(const FDistributionCircularArgs& Args);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Distribution)
	static FDistributionData DistributeCircularUniform(const FDistributionCircularArgs& Args);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Distribution)
	static FDistributionData DistributeCircularUniform2(const FDistributionCircularArgs& Args);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Distribution)
	static FDistributionData DistributeCustomCurve(const FDistributionCustomCurveArgs& Args);

	UFUNCTION(BlueprintPure, Category=Distribution)
	static FDistributionData DistributeCone(const FDistributionConeArgs& Args);
	
	UFUNCTION(BlueprintPure, Category=Distribution)
	static FDistributionData DistributeRing(const FDistributionRingArgs& Args);

private:
	static float GetPolarRadius(int32 Index, const float Count, const int32 BoundaryPoints);
};
