#pragma once

#include "CoreMinimal.h"
#include "Game/Object/J1Object.h"
#include "J1Projectile.generated.h"

class UProjectileData;
class UProjectileMovementComponent;
class AJ1Creature;
class UJ1SkillBase;

UCLASS()
class J1_API AJ1Projectile : public AJ1Object
{
	GENERATED_BODY()
public:
	AJ1Projectile();
	~AJ1Projectile();

public:
	virtual void Tick(float DeltaTime) override;

public:
	/** initialize info **/
	virtual void SetInfo(Protocol::ObjectInfo& InObjectInfo) override;

	TObjectPtr<UProjectileData> GetCreatureData() { return ProjectileData; }

	// Projectile Movement Component
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

public:
	/** setter & getter **/
	void SetDestPos(FVector InDestPos);
	void SetProjectileVelocity(FVector InDestPos);

	void SetOwner(TObjectPtr<AJ1Creature> InOwner);
	void SetOwnerSkill(TObjectPtr<UJ1SkillBase> InOwnerSkill);

public:
	/** process **/
	bool HasReachedTarget();

public:
	/** information **/
	FVector DestPos;

	TObjectPtr<UProjectileData> ProjectileData;

	TObjectPtr<AJ1Creature> Owner;

	TObjectPtr<UJ1SkillBase> OwnerSkill;
};
