// Fill out your copyright notice in the Description page of Project Settings.


#include "DistributionFunctionLibrary.h"

#include "Curves/CurveVector.h"
#include "Kismet/KismetMathLibrary.h"

FTransform UDistributionFunctionLibrary::GetSingleInCircle(int32 Value)
{
	return {};
}

TArray<FTransform> UDistributionFunctionLibrary::TransformsFromDistribution(const FDistributionData& InDistribution)
{
	TArray<FTransform> OutTransforms;
	for (const FVector Point : InDistribution.Points)
	{
		OutTransforms.Add(FTransform(Point));
	}
	return OutTransforms;
}

void UDistributionFunctionLibrary::RotateTransformsToPointAtLocation(TArray<FTransform>& Transforms, const FVector Location)
{
	for (auto& Transform : Transforms)
	{
		Transform.SetRotation(FQuat(UKismetMathLibrary::FindLookAtRotation(Transform.GetLocation(), Location)));
	}
}

void UDistributionFunctionLibrary::RotateTransformsToPointFromLocation(TArray<FTransform>& Transforms, const FVector Location)
{
	for (auto& Transform : Transforms)
	{
		Transform.SetRotation(FQuat(UKismetMathLibrary::FindLookAtRotation(Location, Transform.GetLocation())));
	}
}

void UDistributionFunctionLibrary::DrawDistribution(const UObject* WorldContextObject, const FDistributionData& Distribution,
	const FColor Color, float Lifetime)
{
	for (const auto& Point : Distribution.Points)
	{
		DrawDebugPoint(WorldContextObject->GetWorld(), Point, 5.f, Color, false, Lifetime);
	}
}

FDistributionData UDistributionFunctionLibrary::DistributeSunflower(const FDistributionSunflowerArgs& Args)
{
	const int32 BoundaryPoints = FMath::RoundToInt(Args.Alpha * FMath::Sqrt(static_cast<float>(Args.AdjustedSize)));

	TArray<FVector> OutLocations;
	OutLocations.Reserve(Args.AdjustedSize);

	for (int32 i = 1; i <= Args.AdjustedSize; i++)
	{
		const float PolarRadius = GetPolarRadius(i, Args.AdjustedSize, BoundaryPoints);
		const auto Theta = (2 * PI * i) / FMath::Pow(UE_GOLDEN_RATIO, 2);

		FVector SpawnLoc = FVector(
			Args.Location.X + (Args.Radius * PolarRadius * FMath::Cos(Theta)),
			Args.Location.Y + (Args.Radius * PolarRadius * FMath::Sin(Theta)),
			Args.Location.Z);

		OutLocations.Add(SpawnLoc);
	}
	return FDistributionData(OutLocations, EDistributionDataType::Sunflower);
}

FDistributionData UDistributionFunctionLibrary::DistributeSpiral(const FDistributionSpiralArgs& Args)
{
	TArray<FVector> OutLocations;
	OutLocations.Reserve(Args.AdjustedSize);

	for (int32 i = 1; i <= Args.AdjustedSize; i++)
	{
		const float Angle = (Args.SpiralCoefficient / Args.SpiralDensity) * i;

		FVector SpawnLoc = FVector(
			Args.Location.X + (Args.SpiralSize * Angle) * FMath::Cos(Angle),
			Args.Location.Y + (Args.SpiralSize * Angle) * FMath::Sin(Angle),
			Args.Location.Z
		);

		OutLocations.Add(SpawnLoc);
	}

	return FDistributionData(OutLocations, EDistributionDataType::Spiral);
}

FDistributionData UDistributionFunctionLibrary::DistributeCircular(const FDistributionCircularArgs& Args)
{
	TArray<FVector> OutLocations;
	OutLocations.Reserve(Args.AdjustedSize);

	for (int32 i = 1; i <= Args.AdjustedSize; i++)
	{
		const auto Theta = (2 * PI * i) / FMath::Pow(UE_GOLDEN_RATIO, 2);

		FVector SpawnLoc = FVector(
			Args.Location.X + (Args.Radius * FMath::Cos(Theta)),
			Args.Location.Y + (Args.Radius * FMath::Sin(Theta)),
			Args.Location.Z);

		OutLocations.Add(SpawnLoc);
	}

	return FDistributionData(OutLocations, EDistributionDataType::Circular);
}

FDistributionData UDistributionFunctionLibrary::DistributeCircularUniform(const FDistributionCircularArgs& Args)
{
	TArray<FVector> OutLocations;
	OutLocations.Reserve(Args.AdjustedSize);

	for (int32 i = 1; i <= Args.AdjustedSize; i++)
	{
		const auto Theta = PI + i;
		FVector SpawnLoc = FVector(
			Args.Location.X + Args.Radius * FMath::Cos(Theta),
			Args.Location.Y + Args.Radius * FMath::Sin(Theta),
			Args.Location.Z);

		OutLocations.Add(SpawnLoc);
	}

	return FDistributionData(OutLocations, EDistributionDataType::CircularUniform);
}

FDistributionData UDistributionFunctionLibrary::DistributeCircularUniform2(const FDistributionCircularArgs& Args)
{
	TArray<FVector> OutLocations;
	OutLocations.Reserve(Args.AdjustedSize);

	const float Angle = (2.f * PI) / Args.AdjustedSize;
	for (int32 i = 1; i <= Args.AdjustedSize; i++)
	{
		const auto Theta = Angle * i;
		FVector SpawnLoc = FVector(
			Args.Location.X + Args.Radius * FMath::Cos(Theta),
			Args.Location.Y + Args.Radius * FMath::Sin(Theta),
			Args.Location.Z);

		OutLocations.Add(SpawnLoc);
	}

	return FDistributionData(OutLocations, EDistributionDataType::CircularUniform);
}

FDistributionData UDistributionFunctionLibrary::DistributeCustomCurve(const FDistributionCustomCurveArgs& Args)
{
	if (!Args.CustomVectorCurve)
	{
		return {};
	}
	
	TArray<FVector> OutLocations;
	OutLocations.Reserve(Args.AdjustedSize);

	float MinTime, MaxTime;
	Args.CustomVectorCurve->GetTimeRange(MinTime, MaxTime);

	const float Step = MaxTime / Args.AdjustedSize;
	for (float CurrentStepValue = 0.f; CurrentStepValue <= MaxTime; CurrentStepValue += Step)
	{
		const FVector CurrentValue = Args.CustomVectorCurve->GetVectorValue(CurrentStepValue);

		FVector SpawnLoc = FVector(
			Args.Location.X + CurrentValue.X,
			Args.Location.Y + CurrentValue.Y,
			Args.Location.Z + CurrentValue.Z
		);

		OutLocations.Add(SpawnLoc);
	}

	return FDistributionData(OutLocations, EDistributionDataType::CustomCurve);
}

FDistributionData UDistributionFunctionLibrary::DistributeCone(const FDistributionConeArgs& Args)
{
	TArray<FVector> OutLocations;
	OutLocations.Reserve(Args.Count);

	for (int32 i = 0; i < Args.Count; i++)
	{
		FVector Loc = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(Args.Direction, Args.HalfAngleInDegrees) * Args.Distance;
		Loc.X += Args.Location.X;
		Loc.Y += Args.Location.Y;
		Loc.Z += Args.Location.Z;
		
		OutLocations.Add(Loc);
	}
	
	return FDistributionData(OutLocations, EDistributionDataType::Cone);
}

FDistributionData UDistributionFunctionLibrary::DistributeConeUniform(const FDistributionConeArgs& Args)
{
	TArray<FVector> OutLocations;
	OutLocations.Reserve(Args.Count);
	
	const float Angle = (Args.HalfAngleInDegrees * 2.f) / Args.Count;

	for (int32 i = 0; i < Args.Count; i++)
	{
		const auto Theta = Angle * i;
		FVector Loc = FVector(
			Args.Location.X + Args.Distance * FMath::Cos(Theta),
			Args.Location.Y + Args.Distance * FMath::Sin(Theta),
			Args.Location.Z);

		
		/*float const RandU = FMath::FRand();
		float const RandV = FMath::FRand();

		// Get spherical coords that have an even distribution over the unit sphere
		// Method described at http://mathworld.wolfram.com/SpherePointPicking.html	
		float Theta = 2.f * UE_PI * RandU;
		float Phi = FMath::Acos((2.f * RandV) - 1.f);

		// restrict phi to [0, ConeHalfAngleRad]
		// this gives an even distribution of points on the surface of the cone
		// centered at the origin, pointing upward (z), with the desired angle
		Phi = FMath::Fmod(Phi, Args.HalfAngleInDegrees);

		// get axes we need to rotate around
		FMatrix const DirMat = FRotationMatrix(Args.Direction.Rotation());
		// note the axis translation, since we want the variation to be around X
		FVector const DirZ = DirMat.GetScaledAxis( EAxis::X );		
		FVector const DirY = DirMat.GetScaledAxis( EAxis::Y );

		FVector Loc = Args.Direction.RotateAngleAxis(Phi * 180.f / UE_PI, DirY);
		Loc = Loc.RotateAngleAxis(Theta * 180.f / UE_PI, DirZ);

		// ensure it's a unit vector (might not have been passed in that way)
		Loc = Loc.GetSafeNormal();*/

		Loc.X += Args.Location.X;
		Loc.Y += Args.Location.Y;
		Loc.Z += Args.Location.Z;
		
		OutLocations.Add(Loc);
	}
	
	return FDistributionData(OutLocations, EDistributionDataType::ConeUniform);
}

FDistributionData UDistributionFunctionLibrary::DistributeRing(const FDistributionRingArgs& Args)
{
	TArray<FVector> OutLocations;
	OutLocations.Reserve(Args.Count);

	//const float R1 = Args.InnerRadius;
	//const float R2 = Args.OuterRadius;
	
	const float r1 = Args.OuterRadius;
	const float r2 = /*R2 / R1*/Args.InnerRadius;
	for (int32 i = 1; i <= Args.Count; i++)
	{
		const float Theta = (2.f * PI) * FMath::RandRange(0.0, 1.0);
		//const float U = FMath::RandRange(0.0, 1.0) + FMath::RandRange(0.0, 1.0);
		//float r = U > 1 ? 2 - U : U;
		//r = r < r2 ? r2 + r * ((R1 - R2) / R2) : r;

		const float U = 2.0 / (r1 * r1 - r2 * r2);
		const float r = FMath::Sqrt(2.0 * FMath::RandRange(0.0, 1.0) / U + r2 * r2);
		
		
		//const float Dist = FMath::Sqrt(
		//	i * (FMath::Pow(Args.OuterRadius, 2) - FMath::Pow(Args.InnerRadius, 2) + FMath::Pow(Args.InnerRadius, 2))
		//);
		
		FVector SpawnLoc = FVector(
			Args.Location.X + r * FMath::Cos(Theta),
			Args.Location.Y + r * FMath::Sin(Theta),
			Args.Location.Z);

		OutLocations.Add(SpawnLoc);
	}
	
	return FDistributionData(OutLocations, EDistributionDataType::Ring);
}

float UDistributionFunctionLibrary::GetPolarRadius(int32 Index, const float Count, const int32 BoundaryPoints)
{
	return Index > Count - BoundaryPoints
		? 1.f /* On the boundary */
		: FMath::Sqrt(static_cast<float>(Index - 1 / 2) /* Distance to the boundary */) / FMath::Sqrt(Count - (BoundaryPoints + 1.f) / 2);
}
