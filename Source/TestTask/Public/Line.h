// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unit.h"

/**
 * Class representing a line in a formation
 */
class TESTTASK_API FLine
{
public:
	FLine();
	~FLine();

	FVector2D GetSize() const;
	void SetSize(const FVector2D S);
	void AddUnit(FUnit& Unit);
	TArray<FUnit>& GetUnits();
private:
	FVector2D Size = FVector2D::Zero();
	TArray<FUnit> Units;
};
