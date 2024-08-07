#include "J1ObjectManager.h"
#include "J1Object.h"
#include "J1Projectile.h"
#include "J1Creature.h"
#include "J1Player.h"
#include "J1Monster.h"
#include "J1MyPlayer.h"
#include "J1Boss.h"
#include "J1Env.h"
#include "J1Portal.h"
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

TObjectPtr<AActor> UJ1ObjectManager::SpawnObject(Protocol::ObjectInfo InObjectInfo, TObjectPtr<AJ1Creature> InOwner, TObjectPtr<UJ1SkillBase> InOwnerSkill, FVector InDestPos)
{
	const Protocol::ObjectType objectType = InObjectInfo.object_type();
	const uint64 objectId = InObjectInfo.object_id();

	// 중복 체크
	if (Objects.Find(objectId) != nullptr)
	{
		if (Objects[objectId] == nullptr)
			Objects.Remove(objectId);
		else
			return nullptr;
	}

	if (Creatures.Find(objectId) != nullptr)
	{
		if (Creatures[objectId] == nullptr)
			Creatures.Remove(objectId);
		else
			return nullptr;
	}
	
	FTransform SpawnTransform;
	FVector SpawnLocation(InObjectInfo.pos_info().x(), InObjectInfo.pos_info().y(), InObjectInfo.pos_info().z());
	FRotator SpawnRotation;

	TObjectPtr<AActor> object;

	if (objectType == Protocol::ObjectType::OBJECT_TYPE_CREATURE)
	{
		Protocol::CreatureType creatureType = InObjectInfo.creature_type();
		TObjectPtr<AJ1Creature> creature = nullptr;
		TSubclassOf<AJ1Creature> CreatureClass = GetCreatureClassById(InObjectInfo.template_id());
		
		if (creatureType == Protocol::CreatureType::CREATURE_TYPE_PLAYER)
		{
			creature = Cast<AJ1Creature>(GetWorld()->SpawnActor(CreatureClass, &SpawnLocation));
		}
		else if (creatureType == Protocol::CreatureType::CREATURE_TYPE_MONSTER)
		{
			Protocol::MonsterType monsterType = InObjectInfo.monster_type();
			if (monsterType == Protocol::MonsterType::MONSTER_TYPE_GENERAL)
			{
				// temp
				{
				//	auto* wor = GetWorld();
				//	auto* gain = Cast<UJ1GameInstance>(GetGameInstance());
				//	auto cla = gain->GoblinSpearClass;
				//	SpawnLocation.Z = 100.f;
				//	auto* act = wor->SpawnActor(cla, &SpawnLocation);
				//	creature = Cast<AJ1Creature>(act);
				}
				creature = Cast<AJ1Creature>(GetWorld()->SpawnActor(CreatureClass, &SpawnLocation));
			}
			else if (monsterType == Protocol::MonsterType::MONSTER_TYPE_BOSS)
			{
				FActorSpawnParameters actorSpawnParam;
				actorSpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				creature = Cast<AJ1Creature>(GetWorld()->SpawnActor(CreatureClass, &SpawnLocation, &SpawnRotation, actorSpawnParam));
				if (creature == nullptr)
				{
					int a = 3;
				}
			}
			else
			{
				int a = 3;
			}
		}
		else
		{
			int a = 3;
		}
		
		creature->SetInfo(InObjectInfo);
		Creatures.Add(objectId, creature);
		object = creature;
	}
	else if (objectType == Protocol::ObjectType::OBJECT_TYPE_PROJECTILE)
	{
		TObjectPtr<AJ1Projectile> projectile = nullptr;
		Protocol::ProjectileType projectileType = InObjectInfo.projectile_type();
		TSubclassOf<AJ1Projectile> projectileClass = GetProjectileClassById(InObjectInfo.template_id());

		if (projectileType == Protocol::ProjectileType::PROJECTILE_TYPE_GENERAL)
		{
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			projectile = Cast<AJ1Projectile>(GetWorld()->SpawnActor<AJ1Projectile>(projectileClass, SpawnLocation, FRotator(), ActorSpawnParams));
		}

		projectile->SetOwner(InOwner);
		projectile->SetOwnerSkill(InOwnerSkill);
		projectile->SetDestPos(InDestPos);
		projectile->SetInfo(InObjectInfo);
		Projectiles.Add(objectId, projectile);
		object = projectile;
	}
	else if (objectType == Protocol::ObjectType::OBJECT_TYPE_ENV)
	{
		TObjectPtr<AJ1Env> env = nullptr;
		Protocol::EnvType envType = InObjectInfo.env_type();
		if (envType == Protocol::EnvType::ENV_TYPE_PORTAL)
		{
			env = Cast<AJ1Env>(GetWorld()->SpawnActor(Cast<UJ1GameInstance>(GetGameInstance())->PortalClass, &SpawnLocation));
			if (env == nullptr)
			{
				int a = 3;
			}
		}

		env->SetInfo(InObjectInfo);
		Envs.Add(objectId, env);
		object = env;
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
		Creatures.Remove(InObjectId);
		World->DestroyActor(*Creature);
	}
	else if (objectType == Protocol::ObjectType::OBJECT_TYPE_PROJECTILE)
	{
		TObjectPtr<AJ1Projectile>* Projectile = Projectiles.Find(InObjectId);
		if (Projectile == nullptr)
			return false;

		Projectiles.Remove(InObjectId);
		World->DestroyActor(*Projectile);
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
	if (Objects.Find(InObjectId) == nullptr)
		return nullptr;

	return Objects[InObjectId];
}

TObjectPtr<AJ1Creature> UJ1ObjectManager::GetCreatureById(uint64 InObjectId)
{
	if (Creatures.Find(InObjectId) == nullptr)
		return nullptr;

	return Creatures[InObjectId];
}

TObjectPtr<AJ1Projectile> UJ1ObjectManager::GetProjectileById(uint64 InObjectId)
{
	if (Projectiles.Find(InObjectId) == nullptr)
		return nullptr;

	return Projectiles[InObjectId];
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

TSubclassOf<AJ1Creature> UJ1ObjectManager::GetCreatureClassById(int32 InTemplateId)
{
	UJ1GameInstance* instance = Cast<UJ1GameInstance>(GetGameInstance());
	switch (InTemplateId)
	{
	case 1:
		return instance->OtherPlayerClass; // aurora
	case 10:
		return instance->GoblinSpearClass;
	case 11:
		return instance->GoblinSlingshotClass;
	case 12:
		return instance->DemonRedClass;
	case 100:
		return instance->StartBossClass;
	}

	return TSubclassOf<AJ1Creature>();
}

TSubclassOf<AJ1Projectile> UJ1ObjectManager::GetProjectileClassById(int32 InTemplateId)
{
	UJ1GameInstance* instance = Cast<UJ1GameInstance>(GetGameInstance());
	switch (InTemplateId)
	{
	case 1000010:
		return instance->RockForSlingshotClass; // goblin sling shot's rock
	}

	return TSubclassOf<AJ1Projectile>();
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
