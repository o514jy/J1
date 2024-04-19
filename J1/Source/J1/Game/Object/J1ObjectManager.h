#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "J1ObjectManager.generated.h"

class AJ1MyPlayer;
class AJ1Player;
class AJ1Object;
class AJ1Creature;

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
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	TObjectPtr<AActor> SpawnObject(Protocol::ObjectInfo InObjectInfo);
	bool DespawnObject(uint64 InObjectId);

public:
	Protocol::ObjectType GetObjectTypeById(uint64 InObjectId);

	TObjectPtr<AJ1Object> GetObjectById(uint64 InObjectId);
	TObjectPtr<AJ1Creature> GetCreatureById(uint64 InObjectId);

	TObjectPtr<AActor> GetActorById(uint64 InObjectId);

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
};
