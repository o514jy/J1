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
}

AJ1Player::~AJ1Player()
{
	
}

void AJ1Player::BeginPlay()
{
	Super::BeginPlay();
}

void AJ1Player::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
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
	Cast<AJ1PlayerController>(Controller)->ProcessMove(Info);
}

void AJ1Player::ProcessSkill(const Protocol::S_SKILL& InSkillPkt)
{
	Super::ProcessSkill(InSkillPkt);
}
