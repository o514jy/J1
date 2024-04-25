#include "J1ObjectManager.h"
#include "J1Object.h"
#include "J1Creature.h"
#include "J1Player.h"
#include "J1MyPlayer.h"
#include "J1LogChannels.h"
#include "System/J1GameInstance.h"

uint64 UJ1ObjectManager::GManagerCount = 0;

UJ1ObjectManager& UJ1ObjectManager::Get()
{
	for (auto World : GEngine->GetWorldContexts())
	{
		if (const UGameInstance* GameInstance = World.World()->GetGameInstance())
		{
			if (UJ1ObjectManager* ObjectManager = GameInstance->GetSubsystem<UJ1ObjectManager>())
			{
				return *ObjectManager;
			}
		}
	}

	UE_LOG(LogJ1, Fatal, TEXT("Can't find UJ1ObjectManager"));

	return *NewObject<UJ1ObjectManager>();
}

void UJ1ObjectManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ManagerCount = GManagerCount++;
}

TObjectPtr<AActor> UJ1ObjectManager::SpawnObject(Protocol::ObjectInfo InObjectInfo)
{
	const Protocol::ObjectType objectType = InObjectInfo.object_type();
	const uint64 objectId = InObjectInfo.object_id();
	// 중복 체크
	if (Objects.Find(objectId) != nullptr || Creatures.Find(objectId) != nullptr)
		return nullptr;
	
	FVector SpawnLocation(InObjectInfo.pos_info().x(), InObjectInfo.pos_info().y(), InObjectInfo.pos_info().z());

	TObjectPtr<AActor> object;

	if (objectType == Protocol::ObjectType::OBJECT_TYPE_CREATURE)
	{
		Protocol::CreatureType creatureType = InObjectInfo.creature_type();
		TObjectPtr<AJ1Creature> creature;
		if (creatureType == Protocol::CreatureType::CREATURE_TYPE_PLAYER)
		{
			creature = Cast<AJ1Creature>(GetWorld()->SpawnActor(Cast<UJ1GameInstance>(GetGameInstance())->OtherPlayerClass, &SpawnLocation));
		}
		else if (creatureType == Protocol::CreatureType::CREATURE_TYPE_MONSTER)
		{
			Protocol::MonsterType monsterType = InObjectInfo.monster_type();
			if (monsterType == Protocol::MonsterType::MONSTER_TYPE_GENERAL)
			{

			}
			else if (monsterType == Protocol::MonsterType::MONSTER_TYPE_BOSS)
			{
				creature = Cast<AJ1Creature>(GetWorld()->SpawnActor(Cast<UJ1GameInstance>(GetGameInstance())->StartBossClass, &SpawnLocation));
			}
		}
		
		creature->SetInfo(InObjectInfo);
		Creatures.Add(objectId, creature);
		object = creature;
	}
	else if (objectType == Protocol::ObjectType::OBJECT_TYPE_PROJECTILE)
	{

	}
	else if (objectType == Protocol::ObjectType::OBJECT_TYPE_ENV)
	{

	}

	return object;
}

bool UJ1ObjectManager::DespawnObject(uint64 InObjectId)
{
	Protocol::ObjectType objectType = GetObjectTypeById(InObjectId);

	UWorld* World = GetWorld();
	if (World == nullptr)
		return false;

	if (objectType == Protocol::ObjectType::OBJECT_TYPE_CREATURE)
	{
		TObjectPtr<AJ1Creature>* Creature = Creatures.Find(InObjectId);
		if (Creature == nullptr)
			return false;

		// Destroy Object
		World->DestroyActor(*Creature);
	}

	return true;
}

// [UNUSED(1)][TYPE(31)][ID(32)]
Protocol::ObjectType UJ1ObjectManager::GetObjectTypeById(uint64 InObjectId)
{
	uint64 type = (InObjectId >> 32) & 0x7FFFFFFF;
	return (Protocol::ObjectType)type;
}

TObjectPtr<AJ1Object> UJ1ObjectManager::GetObjectById(uint64 InObjectId)
{
	if (Objects[InObjectId] == nullptr)
		return nullptr;

	return Objects[InObjectId];
}

TObjectPtr<AJ1Creature> UJ1ObjectManager::GetCreatureById(uint64 InObjectId)
{
	if (Creatures[InObjectId] == nullptr)
		return nullptr;

	return Creatures[InObjectId];
}

TObjectPtr<AActor> UJ1ObjectManager::GetActorById(uint64 InObjectId)
{
	Protocol::ObjectType objectType = GetObjectTypeById(InObjectId);

	if (objectType == Protocol::ObjectType::OBJECT_TYPE_CREATURE)
	{
		if (Creatures.Find(InObjectId) == nullptr)
			return nullptr;

		return Cast<AActor>(Creatures[InObjectId]);
	}

	return nullptr;
}

bool UJ1ObjectManager::AddActorToMap(TObjectPtr<AActor> InActor, uint64 InObjectId)
{
	Protocol::ObjectType objectType = GetObjectTypeById(InObjectId);

	if (objectType == Protocol::ObjectType::OBJECT_TYPE_CREATURE)
	{
		if (Creatures.Find(InObjectId) != nullptr)
			return false;

		Creatures.Add(InObjectId, Cast<AJ1Creature>(InActor));
	}

	return true;
}
