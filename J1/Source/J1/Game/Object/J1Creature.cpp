#include "J1Creature.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "J1/J1GameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "J1/Data/J1Data.h"
#include "J1/Data/J1NiagaraData.h"
#include "J1/Data/J1DataManager.h"
#include "J1/System/J1AssetManager.h"
#include "J1/Game/Controllers/J1CreatureController.h"
#include "J1/Game/Stat/J1StatComponent.h"
#include "J1/Game/Skill/J1SkillComponent.h"
#include "J1/Game/Skill/J1SkillBase.h"


// Sets default values
AJ1Creature::AJ1Creature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();
	
	
	/** Configure character movement **/
	{
		GetCharacterMovement()->bRequestedMoveUseAcceleration = false;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
		GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
		GetCharacterMovement()->bConstrainToPlane = true;
		GetCharacterMovement()->bSnapToPlaneAtStart = true;
		GetCharacterMovement()->bRunPhysicsWithNoController = true;
	}
	
	/** Ai Controller **/
	{
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
		AIControllerClass = AJ1CreatureController::StaticClass();
	}

	/** Add Indicator **/
	{
		//auto a = UJ1AssetManager::GetAssetByName<>("IndicatorComponent");
	}
}

AJ1Creature::~AJ1Creature()
{
	delete ObjectInfo;

	CreatureData = nullptr;
}

void AJ1Creature::BeginPlay()
{
	Super::BeginPlay();
	
	SetNowPosInfo();

	SetMoveState(Protocol::MOVE_STATE_IDLE);
}

void AJ1Creature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetNowPosInfo();
}

void AJ1Creature::SetMoveState(Protocol::MoveState State)
{
	if (PosInfo->state() == State)
		return;

	PosInfo->set_state(State);

	// idle�� ��� �����ֱ�
	if (State == Protocol::MoveState::MOVE_STATE_IDLE)
	{
		GetController()->StopMovement();
	}
}

void AJ1Creature::SetPosInfo(const Protocol::PosInfo& Info, bool Forced)
{
	if (PosInfo->object_id() != 0)
	{
		assert(PosInfo->object_id() == Info.object_id());
	}

	PosInfo->CopyFrom(Info);

	if (Forced == true)
	{
		FVector Location(Info.x(), Info.y(), Info.z());
		SetActorLocation(Location);
	}

	SetMoveState(Info.state());
}

void AJ1Creature::SetObjectInfo(const Protocol::ObjectInfo& InObjectInfo)
{
	if (InObjectInfo.object_id() == 0)
		return;

	ObjectInfo->CopyFrom(InObjectInfo);
}

void AJ1Creature::SetNowPosInfo(FVector InLocation)
{
	// ������ ��ġ�� ������Ʈ�ؾ� �ϴ� ���
	if (InLocation == FVector())
	{
		FVector Location = GetActorLocation();
		PosInfo->set_x(Location.X);
		PosInfo->set_y(Location.Y);
		PosInfo->set_z(Location.Z);
		PosInfo->set_yaw(GetControlRotation().Yaw);
	}
	else
	{
		FVector Location = GetActorLocation();
		PosInfo->set_x(Location.X);
		PosInfo->set_y(Location.Y);
		PosInfo->set_z(Location.Z);
		PosInfo->set_yaw(GetControlRotation().Yaw);
	}
}

void AJ1Creature::SetDestPosInfo(FVector InLocation)
{
	PosInfo->set_dest_x(InLocation.X);
	PosInfo->set_dest_y(InLocation.Y);
	PosInfo->set_dest_z(InLocation.Z);
}

void AJ1Creature::OnDamaged(TObjectPtr<AActor> InAttacker, TObjectPtr<UJ1SkillBase> InSkill)
{
	if (InAttacker == nullptr)
		return;

	TObjectPtr<AJ1Creature> creature = Cast<AJ1Creature>(InAttacker);
	if (creature == nullptr)
		return;


}

void AJ1Creature::HandleGameplayEvent(FGameplayTag EventTag)
{
	if (EventTag.MatchesTag(TemplateTag) == true)
	{
		if (EventTag.MatchesTag(SkillComponent->SkillComponentTag) == true)
		{
			SkillComponent->HandleGameplayEvent(EventTag);
		}
	}
}

void AJ1Creature::ProcessMove(const Protocol::PosInfo& Info)
{
	Cast<AJ1CreatureController>(Controller)->ProcessMove(Info);
}

void AJ1Creature::ProcessSkill(const Protocol::S_SKILL& InSkillPkt)
{
	SkillComponent->DoSkill(InSkillPkt);
}

void AJ1Creature::ProcessNotifyPos(const Protocol::PosInfo& Info)
{
	Cast<AJ1CreatureController>(Controller)->ProcessNotifyPos(Info);
}

void AJ1Creature::ProcessProjectile(const Protocol::ProjectileInfo& Info)
{
	// �ϴ� ���⼱ aoe ���ֱ�
	// NS_FindSafeZone_SafeZone

	UJ1NiagaraData* NiagaraData = UJ1AssetManager::GetAssetByName<UJ1NiagaraData>("NiagaraData");
	FJ1NiagaraEntry& Entry = NiagaraData->NameToNiagaraEntry[TEXT("NS_FindSafeZone_SafeZone")];
	TObjectPtr<UNiagaraSystem> niagara = Entry.Niagara;

	FVector loc;
	loc.X = Info.spawn_simple_pos_info().x();
	loc.Y = Info.spawn_simple_pos_info().y();
	loc.Z = Info.spawn_simple_pos_info().z();
	TObjectPtr<UNiagaraComponent> NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagara, loc);
}

void AJ1Creature::SetInfo(const Protocol::ObjectInfo& InObjectInfo)
{
	if (InObjectInfo.object_id() == 0)
		return;

	SetObjectInfo(InObjectInfo);

	ObjectInfo->set_allocated_pos_info(PosInfo);

	ObjectId = ObjectInfo->object_id();
	TemplateId = ObjectInfo->template_id();

	PosInfo->set_object_id(ObjectId);
	SetPosInfo(InObjectInfo.pos_info(), true);

	Protocol::ObjectType objectType = ObjectInfo->object_type();
	Protocol::CreatureType creatureType = ObjectInfo->creature_type();
	if (objectType == Protocol::OBJECT_TYPE_CREATURE)
	{
		if (creatureType == Protocol::CREATURE_TYPE_PLAYER)
		{
			CreatureData = GetManager(Data)->GameData->PlayerData[TemplateId];
		}
		else if (creatureType == Protocol::CREATURE_TYPE_MONSTER)
		{
			Protocol::MonsterType monsterType = ObjectInfo->monster_type();
			if (monsterType == Protocol::MONSTER_TYPE_BOSS)
			{
				CreatureData = GetManager(Data)->GameData->BossData[TemplateId];
			}
		}
		GetCharacterMovement()->MaxWalkSpeed = CreatureData->MaxWalkSpeed;

		TemplateTag = GetManager(Data)->SetTemplateTagByDataId(TemplateId);
	}

	/** add components **/
	/* stat component */
	StatComponent = NewObject<UJ1StatComponent>(this, UJ1StatComponent::StaticClass(), TEXT("StatComponent"));
	StatComponent->SetInfo(this, CreatureData, creatureType);
	/* skill component */
	SkillComponent = NewObject<UJ1SkillComponent>(this, UJ1SkillComponent::StaticClass(), TEXT("SkillComponent"));
	SkillComponent->SetInfo(this, CreatureData);
}
