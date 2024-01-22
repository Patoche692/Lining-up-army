// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitCategory.h"

/**
 * 
 */
class TESTTASK_API FUnit
{
public:
	FUnit(EUnitCategory Category, int R, int C);
	~FUnit();

	void Draw(const UWorld *InWorld, FVector Center, int Angle);
	FVector2D GetSize() const;
	EUnitCategory GetUnitCategory() const;

	inline static float IndividualInnerRadius = 16.0f;
	inline static float IndividualOuterRadius = 20.0f;
	inline static float GapBetweenUnits = 12.0f;

private:
	int Rows, Columns;
	EUnitCategory UnitCategory;
};
