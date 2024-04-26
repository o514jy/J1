#include "J1Creature.h"
#include "J1/J1GameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "J1/Data/J1Data.h"
#include "J1/Data/J1DataManager.h"
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

	// Set size for player capsule
	
	GetCharacterMovement()->bRequestedMoveUseAcceleration = false;
}

AJ1Creature::~AJ1Creature()
{
	delete ObjectInfo;

	CreatureData = nullptr;
}

void AJ1Creature::BeginPlay()
{
	Super::BeginPlay();
	
}

void AJ1Creature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJ1Creature::SetMoveState(Protocol::MoveState State)
{
	if (PosInfo->state() == State)
		return;

	PosInfo->set_state(State);

	// idleÀÏ °æ¿ì ¸ØÃçÁÖ±â
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
}

void AJ1Creature::ProcessSkill(const Protocol::S_SKILL& InSkillPkt)
{
	SkillComponent->DoSkill(InSkillPkt);
}

void AJ1Creature::SetInfo(const Protocol::ObjectInfo& InObjectInfo)
{
	if (InObjectInfo.object_id() == 0)
		return;

	SetObjectInfo(InObjectInfo);

	ObjectId = ObjectInfo->object_id();
	TemplateId = ObjectInfo->template_id();

	PosInfo->set_object_id(ObjectId);
	SetPosInfo(InObjectInfo.pos_info(), true);

	Protocol::ObjectType objectType = ObjectInfo->object_type();
	if (objectType == Protocol::OBJECT_TYPE_CREATURE)
	{
		Protocol::CreatureType creatureType = ObjectInfo->creature_type();
		
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

		TemplateTag = GetManager(Data)->SetTemplateTagByDataId(TemplateId);
		StatComponent = NewObject<UJ1StatComponent>(this, UJ1StatComponent::StaticClass(), TEXT("StatComponent"));
		StatComponent->SetInfo(this, CreatureData, creatureType);
	}

	// add skill component
	SkillComponent = NewObject<UJ1SkillComponent>(this, UJ1SkillComponent::StaticClass(), TEXT("SkillComponent"));
	SkillComponent->SetInfo(this, CreatureData);
}
