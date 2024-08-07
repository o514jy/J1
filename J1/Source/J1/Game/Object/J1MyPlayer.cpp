#include "J1MyPlayer.h"
// Network
#include "J1NetworkManager.h"
// Camera
#include "Camera/CameraComponent.h"
// Controller
#include "Controllers/J1MyPlayerController.h"
// System
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

AJ1MyPlayer::AJ1MyPlayer()
{
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 900.f;
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

void AJ1MyPlayer::SetAutoRunning(bool InFlag)
{
	bAutoRunning = InFlag;
	if (bAutoRunning == true)
	{
		SetMoveState(Protocol::MoveState::MOVE_STATE_RUN);
	}
	else
	{
		SetMoveState(Protocol::MoveState::MOVE_STATE_IDLE);
	}
}

bool AJ1MyPlayer::GetAutoRunning()
{
	return bAutoRunning;
}

void AJ1MyPlayer::SetDesiredYaw(float InYaw)
{
	DesiredYaw = InYaw;
}

float AJ1MyPlayer::GetDesiredYaw()
{
	return DesiredYaw;
}

void AJ1MyPlayer::CheckAndRegisterMove(float DeltaTime)
{
	// Send 판정
	bool ForceSendPacket = false;

	// 이동 상태가 달라졌는지 확인
	if (bLastAutoRunning != bAutoRunning)
	{
		ForceSendPacket = true;
		bLastAutoRunning = bAutoRunning;
	}

	MovePacketSendTimer -= DeltaTime;

	if (MovePacketSendTimer <= 0 || ForceSendPacket == true)
	{
		MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

		RegisterNotifyPos();
	}
}

void AJ1MyPlayer::RegisterMove()
{
	Protocol::C_MOVE MovePkt;
	{
		Protocol::PosInfo* Info = MovePkt.mutable_info();
		Info->CopyFrom(*PosInfo);
		Info->set_yaw(DesiredYaw);
	}
	GetNetworkManager()->SendPacket(MovePkt);
}

void AJ1MyPlayer::RegisterNotifyPos()
{
	Protocol::C_NOTIFY_POS NotifyPosPkt;
	{
		Protocol::PosInfo* Info = NotifyPosPkt.mutable_info();
		Info->CopyFrom(*PosInfo);
		Info->set_yaw(DesiredYaw);
	}
	GetNetworkManager()->SendPacket(NotifyPosPkt);
}

void AJ1MyPlayer::ProcessSkill(const Protocol::S_SKILL& InSkillPkt)
{
	Super::ProcessSkill(InSkillPkt);
}

void AJ1MyPlayer::ProcessTeleport(const Protocol::S_TELEPORT& TeleportPkt)
{
	UGameplayStatics::OpenLevel(this, LevelName_Dungeon_One);

	// send pkt
	Protocol::C_TELEPORT_FIN finPkt;
	{
		Protocol::ObjectInfo* info = finPkt.mutable_info();
		info->CopyFrom(TeleportPkt.info());
		finPkt.set_start_room_type(TeleportPkt.start_room_type());
		finPkt.set_dest_room_type(TeleportPkt.dest_room_type());
	}
	GetNetworkManager()->SendPacket(finPkt);
}

/*
* GameInstance의 Subsystem인 NetWorkManager를 가져옵니다.
*/
UJ1NetworkManager* AJ1MyPlayer::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UJ1NetworkManager>();
}




