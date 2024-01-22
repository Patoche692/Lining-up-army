// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit.h"

FUnit::FUnit(EUnitCategory Category, int R, int C)
{
	UnitCategory = Category;
	Rows = R;
	Columns = C;
}

void FUnit::Draw(const UWorld *InWorld, FVector Center, int Angle)
{
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Columns; j++)
		{
			FVector IndividualPosition = FVector(Center.X - IndividualOuterRadius * (Rows - 1) + i * IndividualOuterRadius * 2, Center.Y - IndividualOuterRadius * (Columns - 1) + j * IndividualOuterRadius * 2, Center.Z);
			FVector RelativePosition = (IndividualPosition - Center).RotateAngleAxis(Angle, FVector::ZAxisVector);
			FColor Color;
			if (UnitCategory == EUnitCategory::UC_Front)
			{
				Color = FColor::White;
			}
			else if (UnitCategory == EUnitCategory::UC_Ranged)
			{
				Color = FColor::Magenta;
			}
			else if (UnitCategory == EUnitCategory::UC_Siege)
			{
				Color = FColor::Cyan;
			}
			DrawDebugCircle(InWorld, Center + RelativePosition, IndividualInnerRadius, 6, Color, false, -1, 0, 4, FVector::YAxisVector, FVector::XAxisVector, false);
		}
	}
}

FVector2D FUnit::GetSize() const
{
	return FVector2D(Columns * IndividualOuterRadius * 2 + GapBetweenUnits, Rows * IndividualOuterRadius * 2 + GapBetweenUnits);
}

EUnitCategory FUnit::GetUnitCategory() const { return UnitCategory; }

FUnit::~FUnit()
{
}
