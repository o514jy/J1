#include "J1Projectile.h"
#include "Data/J1DataManager.h"
#include "Object/J1Creature.h"
#include "Object/J1ObjectManager.h"
#include "Data/J1Data.h"
#include "GameFramework/ProjectileMovementComponent.h"

AJ1Projectile::AJ1Projectile()
{
	DestPos = FVector();
	ProjectileData = nullptr;

	Owner = nullptr;
	OwnerSkill = nullptr;

	// Create and configure a projectile movement component
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f; // 중력 무시
}

AJ1Projectile::~AJ1Projectile()
{
}

void AJ1Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasReachedTarget() == true)
	{
		if (Owner != nullptr)
		{
			Owner->GetManager(Object)->DespawnObject(ObjectId);
		}
	}
}

void AJ1Projectile::SetInfo(Protocol::ObjectInfo& InObjectInfo)
{
	Super::SetInfo(InObjectInfo);

	ProjectileData = GetManager(Data)->GameData->ProjectileData[TemplateId];

	// Initial speed and maximum speed
	ProjectileMovement->InitialSpeed = ProjectileData->MoveSpeed;
	ProjectileMovement->MaxSpeed = ProjectileData->MoveSpeed;

	SetProjectileVelocity(DestPos);
}

void AJ1Projectile::SetDestPos(FVector InDestPos)
{
	DestPos = InDestPos;
}

void AJ1Projectile::SetProjectileVelocity(FVector InDestPos)
{
	// Calculate the direction to the target
	FVector Direction = (DestPos - GetActorLocation()).GetSafeNormal();
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
}

void AJ1Projectile::SetOwner(TObjectPtr<AJ1Creature> InOwner)
{
	Owner = InOwner;
}

void AJ1Projectile::SetOwnerSkill(TObjectPtr<UJ1SkillBase> InOwnerSkill)
{
	OwnerSkill = InOwnerSkill;
}

bool AJ1Projectile::HasReachedTarget()
{
	// Check if the projectile is close enough to the target location
	// Adjust tolerance as needed
	return FVector::Dist(GetActorLocation(), DestPos) <= 10.0f;
}
