// Fill out your copyright notice in the Description page of Project Settings.


#include "Line.h"

FLine::FLine()
{
}

FVector2D FLine::GetSize() const
{
	return Size;
}

void FLine::SetSize(const FVector2D S)
{
	Size = S;
}

void FLine::AddUnit(FUnit& Unit)
{
	Units.Add(Unit);
}


TArray<FUnit>& FLine::GetUnits()
{
	return Units;
}

FLine::~FLine()
{
}
