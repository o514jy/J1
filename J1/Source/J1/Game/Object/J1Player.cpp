#include "J1Player.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
// AI
#include "Controllers/J1PlayerController.h"
#include "J1MyPlayer.h"
// Data


AJ1Player::AJ1Player()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(20.f, 96.0f);
	GetCharacterMovement()->RotationRate = FRotator(0.f, 1080.f, 0.f);
	
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	//TempDestInfo = new Protocol::PosInfo();
}

AJ1Player::~AJ1Player()
{
	//delete TempDestInfo;
}

void AJ1Player::BeginPlay()
{
	Super::BeginPlay();
}

void AJ1Player::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//if (IsMyPlayer() == false)
	//{
	//	const Protocol::MoveState State = PosInfo->state();
	//
	//	if (State == Protocol::MOVE_STATE_RUN)
	//	{
	//		SetActorRotation(FRotator(0, TempDestInfo->yaw(), 0));
	//		AddMovementInput(GetActorForwardVector());
	//	}
	//	else if (State == Protocol::MOVE_STATE_IDLE)
	//	{
	//		// �÷��̾�� �̹� ����µ� ���� ���������� ���� ���� ���
	//		FVector location;
	//		location.X = TempDestInfo->x();
	//		location.Y = TempDestInfo->y();
	//		location.Z = TempDestInfo->z();
	//
	//		FVector MyLocation = GetActorLocation();
	//
	//		if (FVector::Dist(location, MyLocation) > 10.f)
	//		{
	//			FVector worldDir = (location - MyLocation).GetSafeNormal();
	//			AddMovementInput(worldDir);
	//		}
	//	}
	//}
}

void AJ1Player::SetPosInfo(const Protocol::PosInfo& Info, bool Forced)
{
	Super::SetPosInfo(Info, Forced);

	//if (Forced == true)
	//{
	//	SetTempDestPosInfo(Info);
	//	SetTempDestYaw(Info.yaw());
	//}
}

//void AJ1Player::SetTempDestPosInfo(const Protocol::PosInfo& InPosInfo)
//{
//	if (ObjectInfo->object_id() != 0)
//	{
//		assert(ObjectInfo->object_id() == Info.object_id());
//	}
//
//	TempDestInfo->CopyFrom(InPosInfo);
//
//	// ���´� ����
//	SetMoveState(InPosInfo.state());
//}
//
//void AJ1Player::SetTempDestYaw(const float InYaw)
//{
//	TempDestYaw = InYaw;
//}

void AJ1Player::SetInfo(const Protocol::ObjectInfo& InObjectInfo)
{
	Super::SetInfo(InObjectInfo);

	// temp dest pos info and yaw
	//SetTempDestPosInfo(InObjectInfo.pos_info());
	//SetTempDestYaw(InObjectInfo.pos_info().yaw());
}

void AJ1Player::ProcessMove(const Protocol::PosInfo& Info)
{
	Cast<AJ1PlayerController>(GetController())->ProcessMove(Info);
	// ������ ����
	//SetTempDestPosInfo(Info);
}

void AJ1Player::ProcessSkill(const Protocol::S_SKILL& InSkillPkt)
{
	Super::ProcessSkill(InSkillPkt);
}
