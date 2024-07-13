#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "J1ObjectManager.generated.h"

class AJ1MyPlayer;
class AJ1Player;
class AJ1Object;
class AJ1Creature;
class AJ1Env;
class AJ1Projectile;
class UJ1SkillBase;

UCLASS()
class J1_API UJ1ObjectManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	static UJ1ObjectManager& Get();
private:
	static uint64 GManagerCount;
	uint64 ManagerCount;

public:
	/** initialize **/
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	/** setter & getter **/

public:
	TObjectPtr<AActor> SpawnObject(Protocol::ObjectInfo InObjectInfo, TObjectPtr<AJ1Creature> InOwner = nullptr, TObjectPtr<UJ1SkillBase> InOwnerSkill = nullptr, FVector InDestPos = FVector());
	bool DespawnObject(uint64 InObjectId);

public:
	Protocol::ObjectType GetObjectTypeById(uint64 InObjectId);

	TObjectPtr<AJ1Object> GetObjectById(uint64 InObjectId);
	TObjectPtr<AJ1Creature> GetCreatureById(uint64 InObjectId);
	TObjectPtr<AJ1Projectile> GetProjectileById(uint64 InObjectId);

	TObjectPtr<AActor> GetActorById(uint64 InObjectId);

	TSubclassOf<AJ1Creature> GetCreatureClassById(int32 InTemplateId);
	TSubclassOf<AJ1Projectile> GetProjectileClassById(int32 InTemplateId);

public:
	bool AddActorToMap(TObjectPtr<AActor> InActor, uint64 InObjectId);

public:

public:
	UPROPERTY()
	TObjectPtr<AJ1MyPlayer> MyPlayer;
	UPROPERTY()
	TMap<uint64, TObjectPtr<AJ1Object>> Objects;
	UPROPERTY()
	TMap<uint64, TObjectPtr<AJ1Creature>> Creatures;
	UPROPERTY()
	TMap<uint64, TObjectPtr<AJ1Env>> Envs;
	UPROPERTY()
	TMap<uint64, TObjectPtr<AJ1Projectile>> Projectiles;
};
