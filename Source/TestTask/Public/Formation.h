// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unit.h"
#include "GameFramework/Pawn.h"
#include "Formation.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class TESTTASK_API AFormation : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFormation();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SetNewFormationAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CancelFormationAction;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnSetNewFormation();
	void OnAdjustFormation();
	void OnSetFormationReleased();
	void OnCancelFormation();

	void SortUnits();
	void DrawFormation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	FVector StartingPoint;
	FVector CurrentPoint;
	FVector2D StartingPoint2D;
	FVector2D CurrentPoint2D;
	bool bTriggered;

	TArray<FUnit> Units, FrontUnits, RangedUnits, SiegeUnits;
	//TMap<FUnit, TTuple<double, double>> UnitsPositions;
	TMap<EUnitCategory, TArray<FUnit>> CategorizedUnits;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

};
