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
// Data
#include "Data/J1InputData.h"
// GAS
#include "J1GameplayTags.h"
// System
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "J1LogChannels.h"
#include "System/J1AssetManager.h"
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

void AJ1MyPlayerController::OnInputStarted()
{
	//StopMovement();
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
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		if (FollowTime > sendMovePacketThreshold)
		{
			RegisterMove(CachedDestination);
		}
		//FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		//ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
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

	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		RegisterMove(CachedDestination);
		UJ1InputData* InputData = UJ1AssetManager::GetAssetByName<UJ1InputData>("InputData");
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, InputData->GetFXCursor(), CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

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

void AJ1MyPlayerController::RegisterMove(FVector location)
{
	// send Notify Pos Packet
	Protocol::C_MOVE MovePkt;

	AJ1MyPlayer* myPlayer = Cast<AJ1MyPlayer>(GetPawn());

	{
		Protocol::PosInfo* Info = MovePkt.mutable_info();
		Info->CopyFrom(*myPlayer->GetPosInfo());
		Info->set_dest_x(location.X);
		Info->set_dest_y(location.Y);
		Info->set_dest_z(location.Z);
	}
	
	GetNetworkManager()->SendPacket(MovePkt);
}

void AJ1MyPlayerController::ProcessMove(const Protocol::PosInfo& posInfo)
{
	// send Notify Pos Packet
	AJ1MyPlayer* myPlayer = Cast<AJ1MyPlayer>(GetPawn());
	//myPlayer->SetMoveState(Protocol::MOVE_STATE_RUN);

	// 목적지 기입
	myPlayer->SetPosInfo(posInfo);

	// start move
	FVector location;
	location.X = posInfo.dest_x();
	location.Y = posInfo.dest_y();
	location.Z = posInfo.dest_z();
	//if (FollowTime <= ShortPressThreshold)
	{
		
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, location);
	}
	//else
	//{
	//	FVector WorldDirection = (location - GetPawn()->GetActorLocation()).GetSafeNormal();
	//	GetPawn()->AddMovementInput(WorldDirection, 1.0, false);
	//}
}

void AJ1MyPlayerController::RegisterNotifyPos()
{
	AJ1MyPlayer* myPlayer = Cast<AJ1MyPlayer>(GetPawn());

	Protocol::C_NOTIFY_POS notifyPkt;
	{
		Protocol::PosInfo* posInfo = notifyPkt.mutable_info();
		posInfo->CopyFrom(*myPlayer->GetPosInfo());
	}
	GetManager(Network)->SendPacket(notifyPkt);
}

void AJ1MyPlayerController::ProcessNotifyPos(const Protocol::PosInfo& Info)
{
	// todo : 위치 교정해야하면 여기서 해주고

	// 답장 보내기
	RegisterNotifyPos();
}

/*
 * GameInstance의 Subsystem인 NetWorkManager를 가져옵니다.
 */
UJ1NetworkManager* AJ1MyPlayerController::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UJ1NetworkManager>();
}