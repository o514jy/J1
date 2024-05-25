#include "J1MyPlayerController.h"
// Player
#include "Object/J1MyPlayer.h"
#include "Object/J1Player.h"
#include "Engine/LocalPlayer.h"
// Network
#include "Network/J1NetworkManager.h" 
// Niagara
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
// Input
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
// AI
#include "Blueprint/AIBlueprintHelperLibrary.h"
// Navigation & Spline
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"
// Data
#include "Data/J1InputData.h"
// GAS
#include "J1GameplayTags.h"
// System
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "J1LogChannels.h"
#include "System/J1AssetManager.h"
#include "Kismet/KismetMathLibrary.h"
// Skill
#include "J1/Game/Skill/J1SkillComponent.h"

AJ1MyPlayerController::AJ1MyPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	sendMovePacketThreshold = 0.2f;
	ShortPressThreshold = 0.3f;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AJ1MyPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	if (const UJ1InputData* InputData = UJ1AssetManager::GetAssetByName<UJ1InputData>("InputData"))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
		
		auto SetDestinationClickAction = InputData->FindInputActionByTag(J1GameplayTags::Input_Action_SetDestinationClick);
		auto BaseAttackAction = InputData->FindInputActionByTag(J1GameplayTags::Input_Action_BaseAttack);
		auto QAction = InputData->FindInputActionByTag(J1GameplayTags::Input_Action_Q);

		// Mouse Right Button
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AJ1MyPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AJ1MyPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AJ1MyPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AJ1MyPlayerController::OnSetDestinationReleased);
		
		// Mouse Left Button
		EnhancedInputComponent->BindAction(BaseAttackAction, ETriggerEvent::Triggered, this, &AJ1MyPlayerController::OnBaseAttackTriggered);

		// Q
		EnhancedInputComponent->BindAction(QAction, ETriggerEvent::Triggered, this, &AJ1MyPlayerController::OnQTriggered);
	}
	else
	{
		UE_LOG(LogJ1, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	// Set up action bindings
	//if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	//{
	//	// Setup mouse input events
	//	EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AJ1MyPlayerController::OnInputStarted);
	//	EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AJ1MyPlayerController::OnSetDestinationTriggered);
	//	EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AJ1MyPlayerController::OnSetDestinationReleased);
	//	EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AJ1MyPlayerController::OnSetDestinationReleased);
	//}
	//else
	//{
	//	UE_LOG(LogJ1, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	//}
}

void AJ1MyPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (const UJ1InputData* InputData = UJ1AssetManager::GetAssetByName<UJ1InputData>("InputData"))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputData->InputMappingContext, 0);
		}
	}

	//Add Input Mapping Context
	//if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	//{
	//	Subsystem->AddMappingContext(DefaultMappingContext, 0);
	//}
}

void AJ1MyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	AutoRun();
}

void AJ1MyPlayerController::OnInputStarted()
{
	StopMovement();
}

void AJ1MyPlayerController::OnSetDestinationTriggered()
{
	TObjectPtr<AJ1Creature> creature = Cast<AJ1Creature>(GetPawn());
	if (creature->GetMoveState() == Protocol::MoveState::MOVE_STATE_SKILL)
	{
		return;
	}

	if (creature->GetMoveState() == Protocol::MoveState::MOVE_STATE_DEAD)
	{
		return;
	}

	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, true, Hit);

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
		GetMyPlayer()->SetDestPosInfo(CachedDestination);
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);

		// set desired yaw
		FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(ControlledPawn->GetActorLocation(), CachedDestination);
		GetMyPlayer()->SetDesiredYaw(Rotator.Yaw);
	}

	// send S_MOVE
	GetMyPlayer()->RegisterMove();
}

void AJ1MyPlayerController::OnSetDestinationReleased()
{
	TObjectPtr<AJ1Creature> creature = Cast<AJ1Creature>(GetPawn());
	if (creature->GetMoveState() == Protocol::MoveState::MOVE_STATE_SKILL)
	{
		return;
	}
	if (creature->GetMoveState() == Protocol::MoveState::MOVE_STATE_DEAD)
	{
		return;
	}

	APawn* ControlledPawn = GetPawn();
	if (FollowTime <= ShortPressThreshold && ControlledPawn != nullptr)
	{
		if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
		{
			Spline->ClearSplinePoints();
			if (NavPath->PathPoints.Num() >= 1)
			{
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 1.f);
				}

				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				GetMyPlayer()->SetAutoRunning(true);
			}
		}
	}
	UJ1InputData* InputData = UJ1AssetManager::GetAssetByName<UJ1InputData>("InputData");
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, InputData->GetFXCursor(), CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	FollowTime = 0.f;
}

void AJ1MyPlayerController::OnBaseAttackTriggered()
{
	// 전투상황이 아닐 경우 여기부터 처리

	// 전투 목적일 때 이미 다른 스킬을 쓰고있는 상태면 불가능
	TObjectPtr<AJ1Creature> creature = Cast<AJ1Creature>(GetPawn());
	if (creature->GetMoveState() == Protocol::MoveState::MOVE_STATE_SKILL)
		return;
	if (creature->GetMoveState() == Protocol::MoveState::MOVE_STATE_DEAD)
	{
		return;
	}

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, true, OUT Hit);
	FVector location = Hit.Location;

	// 싸우는 상황이면 평타
	if (creature != nullptr)
	{
		creature->SkillComponent->RegisterNormalAttack(location);
	}
	
}

void AJ1MyPlayerController::OnQTriggered()
{
	// 이미 다른 스킬을 쓰고있는 상태면 불가능
	TObjectPtr<AJ1Creature> creature = Cast<AJ1Creature>(GetPawn());
	if (creature->GetMoveState() == Protocol::MoveState::MOVE_STATE_SKILL)
		return;
	if (creature->GetMoveState() == Protocol::MoveState::MOVE_STATE_DEAD)
	{
		return;
	}

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, true, OUT Hit);
	FVector location = Hit.Location;

	// Q스킬 날리겠다는 요청 보내기
	if (creature != nullptr)
	{
		creature->SkillComponent->RegisterAuroraQ(location);
	}
}

void AJ1MyPlayerController::AutoRun()
{
	if (GetMyPlayer()->GetAutoRunning() == false)
		return;

	if (TObjectPtr<AJ1MyPlayer> ControlledPawn = GetMyPlayer())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		// set desired yaw
		FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(ControlledPawn->GetActorLocation(), CachedDestination);
		GetMyPlayer()->SetDesiredYaw(Rotator.Yaw);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			GetMyPlayer()->SetAutoRunning(false);
		}
	}
}

TObjectPtr<AJ1MyPlayer> AJ1MyPlayerController::GetMyPlayer()
{
	return Cast<AJ1MyPlayer>(GetPawn());
}

/*
 * GameInstance의 Subsystem인 NetWorkManager를 가져옵니다.
 */
UJ1NetworkManager* AJ1MyPlayerController::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UJ1NetworkManager>();
}