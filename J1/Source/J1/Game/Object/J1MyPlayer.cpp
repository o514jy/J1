// Fill out your copyright notice in the Description page of Project Settings.


#include "J1MyPlayer.h"
// Network
#include "J1NetworkManager.h"
// Camera
#include "Camera/CameraComponent.h"
// Controller
#include "Controllers/J1MyPlayerController.h"
// System
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

AJ1MyPlayer::AJ1MyPlayer()
{
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	DefaultCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	DefaultCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	DefaultCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AJ1MyPlayer::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();


}


void AJ1MyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckAndRegisterMove(DeltaTime);
}

void AJ1MyPlayer::CheckAndRegisterMove(float DeltaTime)
{
	// Send 판정
	bool ForceSendPacket = false;

	// 목적지에 도착해서 멈췄는지 확인
	if (PosInfo->state() == Protocol::MOVE_STATE_RUN)
	{
		if (CompareNowPosAndDestPos() == true)
		{
			ForceSendPacket = true;
			SetMoveState(Protocol::MOVE_STATE_IDLE);
		}
	}

	MovePacketSendTimer -= DeltaTime;

	if (MovePacketSendTimer <= 0 || ForceSendPacket)
	{
		MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

		// send Notify Pos Packet
		SetMoveState(Protocol::MOVE_STATE_RUN);

		Protocol::C_NOTIFY_POS NotifyPosPkt;

		{
			Protocol::PosInfo* Info = NotifyPosPkt.mutable_info();
			Info->CopyFrom(*PosInfo);
		}

		GetNetworkManager()->SendPacket(NotifyPosPkt);
	}
}

void AJ1MyPlayer::ProcessMove(const Protocol::PosInfo& Info)
{
	Cast<AJ1MyPlayerController>(Controller)->ProcessMove(Info);
}

void AJ1MyPlayer::ProcessSkill(const Protocol::S_SKILL& InSkillPkt)
{
	Super::ProcessSkill(InSkillPkt);
}

bool AJ1MyPlayer::CompareNowPosAndDestPos()
{
	FVector nowPos, destPos;

	nowPos.X = PosInfo->x();
	nowPos.Y = PosInfo->y();
	nowPos.Z = PosInfo->z();
	destPos.X = PosInfo->dest_x();
	destPos.Y = PosInfo->dest_y();
	destPos.Z = PosInfo->dest_z();

	float dist = FVector::DistSquared(nowPos, destPos);
	if (dist <= 0.01f)
	{
		return true;
	}

	return false;
}

/*
* GameInstance의 Subsystem인 NetWorkManager를 가져옵니다.
*/
UJ1NetworkManager* AJ1MyPlayer::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UJ1NetworkManager>();
}




