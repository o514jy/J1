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
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	GetCharacterMovement()->bRunPhysicsWithNoController = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	ObjectInfo = MakeShared<Protocol::ObjectInfo>(Protocol::ObjectInfo());
	PosInfo = MakeShared<Protocol::PosInfo>(Protocol::PosInfo());
}

AJ1Player::~AJ1Player()
{
	ObjectInfo = nullptr;
	PosInfo = nullptr;
}

void AJ1Player::BeginPlay()
{
	Super::BeginPlay();

	{
		FVector Location = GetActorLocation();
		PosInfo->set_x(Location.X);
		PosInfo->set_y(Location.Y);
		PosInfo->set_z(Location.Z);
		PosInfo->set_yaw(GetControlRotation().Yaw);

		SetMoveState(Protocol::MOVE_STATE_IDLE);
	}
}

void AJ1Player::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	{
		FVector Location = GetActorLocation();
		PosInfo->set_x(Location.X);
		PosInfo->set_y(Location.Y);
		PosInfo->set_z(Location.Z);
		PosInfo->set_yaw(GetControlRotation().Yaw);
	}
}

bool AJ1Player::IsMyPlayer()
{
	return Cast<AJ1MyPlayer>(this) != nullptr;
}

void AJ1Player::SetInfo(const Protocol::ObjectInfo& InObjectInfo)
{
	Super::SetInfo(InObjectInfo);

	// player data
}

void AJ1Player::ProcessMove(const Protocol::PosInfo& Info)
{
	Cast<AJ1AIController>(Controller)->ProcessMove(Info);
}

void AJ1Player::ProcessSkill(const Protocol::SkillSlot& Slot)
{
	Super::ProcessSkill(Slot);
}