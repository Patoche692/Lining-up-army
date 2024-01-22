// Fill out your copyright notice in the Description page of Project Settings.


#include "Formation.h"

#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FormationController.h"
#include "Line.h"
#include "Camera/CameraComponent.h"
#include "Engine/HitResult.h"
#include "Engine/LocalPlayer.h"
#include "Framework/MultiBox/MultiBoxDefs.h"
#include "GameFramework/HUD.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AFormation::AFormation()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	StartingPoint = FVector::Zero();
	StartingPoint2D = FVector2D::Zero();
	bTriggered = false;

	Units.Add(FUnit(EUnitCategory::UC_Front, 4, 5));
	Units.Add(FUnit(EUnitCategory::UC_Ranged, 1, 6));
	Units.Add(FUnit(EUnitCategory::UC_Siege, 3, 4));
	Units.Add(FUnit(EUnitCategory::UC_Front, 3, 4));
	Units.Add(FUnit(EUnitCategory::UC_Siege, 3, 4));
	Units.Add(FUnit(EUnitCategory::UC_Front, 2, 1));
	Units.Add(FUnit(EUnitCategory::UC_Ranged, 3, 4));
	Units.Add(FUnit(EUnitCategory::UC_Front, 3, 4));
	Units.Add(FUnit(EUnitCategory::UC_Ranged, 3, 4));
	Units.Add(FUnit(EUnitCategory::UC_Front, 4, 5));
	Units.Add(FUnit(EUnitCategory::UC_Ranged, 1, 6));
	Units.Add(FUnit(EUnitCategory::UC_Siege, 3, 4));
	Units.Add(FUnit(EUnitCategory::UC_Front, 3, 4));
	Units.Add(FUnit(EUnitCategory::UC_Siege, 3, 4));
	Units.Add(FUnit(EUnitCategory::UC_Front, 2, 1));
	Units.Add(FUnit(EUnitCategory::UC_Ranged, 3, 4));
	Units.Add(FUnit(EUnitCategory::UC_Front, 3, 4));
	Units.Add(FUnit(EUnitCategory::UC_Ranged, 3, 4));

	SortUnits();

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void AFormation::SortUnits()
{
	for (int Category = 0; Category <= 2; Category++)
	{
		CategorizedUnits.Add(static_cast<EUnitCategory>(Category), {});
	}
	for (auto& Unit: Units)
	{
		CategorizedUnits[Unit.GetUnitCategory()].Add(Unit);
	}
}


// Called when the game starts or when spawned
void AFormation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFormation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFormation::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PC = Cast<APlayerController>(GetController());
	PC->bShowMouseCursor = true;
	PC->DefaultMouseCursor = EMouseCursor::Default;
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	Subsystem->AddMappingContext(DefaultMappingContext, 0);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(SetNewFormationAction, ETriggerEvent::Started, this, &AFormation::OnSetNewFormation);
		EnhancedInputComponent->BindAction(SetNewFormationAction, ETriggerEvent::Triggered, this, &AFormation::OnAdjustFormation);
		EnhancedInputComponent->BindAction(SetNewFormationAction, ETriggerEvent::Completed, this, &AFormation::OnSetFormationReleased);
		EnhancedInputComponent->BindAction(CancelFormationAction, ETriggerEvent::Started, this, &AFormation::OnCancelFormation);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

void AFormation::OnSetNewFormation()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	FHitResult Hit;
	if (!PlayerController->GetHitResultUnderCursor(ECC_WorldStatic, false, Hit))
	{
		return;
	}

	float MouseX, MouseY;
	PlayerController->GetMousePosition(MouseX, MouseY);
	FVector2D CursorLocation2D = FVector2D(MouseX, MouseY);

	bTriggered = true;
	StartingPoint = Hit.Location;
	StartingPoint2D = CursorLocation2D;
}

void AFormation::OnAdjustFormation()
{
	if (!bTriggered)
		return;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	FHitResult Hit;
	if (PlayerController->GetHitResultUnderCursor(ECC_WorldStatic, false, Hit))
	{
		CurrentPoint = Hit.Location;

		float MouseX, MouseY;
		PlayerController->GetMousePosition(MouseX, MouseY);
		CurrentPoint2D = FVector2D(MouseX, MouseY);
	}

	DrawFormation();
}


void AFormation::DrawFormation()
{

	const float Distance = (CurrentPoint2D - StartingPoint2D).Length() * 3;
	FVector Direction = (StartingPoint - CurrentPoint).GetSafeNormal();
	FVector NormalDirection = Direction.RotateAngleAxis(90, FVector::ZAxisVector);

	FVector LineVector = NormalDirection * Distance;

	double Angle = Direction.HeadingAngle() * 180.0f / PI;

	// Don't draw anything if the cursor is at the origin point
	if (Direction.Length() == 0) {
		return;
	}

	DrawDebugLine(GetWorld(), StartingPoint - LineVector / 2, StartingPoint + LineVector / 2, FColor::White, false, -1, 0, 5);

	TArray<FLine> Lines;
	int CurrentLine = 0;

	for (int i = 0; i < 3; i++) {
		int TotalWidth = 0;
		int MaxDepth = 0;
		for (auto& Unit : CategorizedUnits[static_cast<EUnitCategory>(i)])
		{
			if (Lines.Num() <= CurrentLine)
			{
				Lines.Add(FLine());
			}
			FVector2D UnitSize = Unit.GetSize();
			TotalWidth += UnitSize.X;
			if (UnitSize.Y > MaxDepth)
			{
				MaxDepth = UnitSize.Y;
			}

			Lines[CurrentLine].AddUnit(Unit);
			if (TotalWidth > Distance)
			{
				Lines[CurrentLine].SetSize(FVector2D(TotalWidth, MaxDepth));
				CurrentLine++;
				TotalWidth = 0;
				MaxDepth = 0;
			}
		}
		// Handle the back line of each category
		if (CurrentLine < Lines.Num()) {
			Lines[CurrentLine].SetSize(FVector2D(TotalWidth, MaxDepth));
			CurrentLine++;
		}
	}

	FVector Start = StartingPoint;
	for (auto& Line : Lines)
	{
		TArray<FUnit> LineUnits = Line.GetUnits();
		Start = Start + Direction * Line.GetSize().Y / 2;
		FVector Center = Start + NormalDirection * Line.GetSize().X / 2;
		for (int i = 0; i < LineUnits.Num(); i++)
		{
			FUnit Unit = LineUnits[i];
			Center -= NormalDirection * Unit.GetSize().X / 2;

			Unit.Draw(GetWorld(), Center, Angle);
			Center -= NormalDirection * Unit.GetSize().X / 2;
		}
		Start += Direction * Line.GetSize().Y / 2;
	}
}



void AFormation::OnSetFormationReleased()
{
	bTriggered = false;

	/*
	 * We move the different units at the specified spots
	 */
}

void AFormation::OnCancelFormation()
{
	bTriggered = false;
}
