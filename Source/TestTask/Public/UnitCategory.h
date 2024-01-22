// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *	Class defining the different categories of a unit
 */
UENUM(BlueprintType)
enum class EUnitCategory
{
	UC_Front	UMETA(DisplayName = "Front"),
	UC_Ranged	UMETA(DisplayName = "Ranged"),
	UC_Siege	UMETA(DisplayName = "Siege")
};
