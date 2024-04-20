#include "J1SkillComponent.h"
#include "J1/J1GameplayTags.h"
#include "J1/Game/Object/J1Creature.h"
#include "J1/Game/Skill/J1SkillManager.h"
#include "J1/Game/Skill/J1SkillBase.h"
#include "J1/Network/J1NetworkManager.h"
#include "J1/Data/J1DataManager.h"

UJ1SkillComponent::UJ1SkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Owner = nullptr;
}

UJ1SkillComponent::~UJ1SkillComponent()
{
	Owner = nullptr;

	/** player **/
	NormalAttackSkill = nullptr;
	QSkill = nullptr;
	WSkill = nullptr;
	ESkill = nullptr;
	RSkill = nullptr;
	DashSkill = nullptr;
}


// Called when the game starts
void UJ1SkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UJ1SkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UJ1SkillComponent::SetInfo(TObjectPtr<AJ1Creature> InOwner, TObjectPtr<UCreatureData> InCreatureData)
{
	Owner = InOwner;
	SkillComponentTag = Owner->GetManager(Data)->SetSkillComponentTagByDataId(Owner->GetTemplateId());

	if (InCreatureData->CreatureType == TEXT("Player"))
	{
		TObjectPtr<UPlayerData> data = Cast<UPlayerData>(InCreatureData);
		AddSkill(data->SkillAttackId, Protocol::SkillSlot::SKILL_SLOT_ATTACK);
		AddSkill(data->SkillQId, Protocol::SkillSlot::SKILL_SLOT_Q);
		AddSkill(data->SkillWId, Protocol::SkillSlot::SKILL_SLOT_W);
		AddSkill(data->SkillEId, Protocol::SkillSlot::SKILL_SLOT_E);
		AddSkill(data->SkillRId, Protocol::SkillSlot::SKILL_SLOT_R);
		AddSkill(data->SkillDashId, Protocol::SkillSlot::SKILL_SLOT_DASH);
	}
}

void UJ1SkillComponent::AddSkill(int32 InTemplateId, Protocol::SkillSlot InSkillSlot)
{
	if (InTemplateId < 1)
	{
		return;
	}

	TObjectPtr<UJ1SkillBase> skill = Owner->GetManager(Skill)->GenerateSkillById(InTemplateId);
	if (skill == nullptr)
	{
		//ensureAlwaysMsgf(skill, TEXT("Can't find skill from [%d]."), InTemplateId);
		return;
	}

	if (InSkillSlot == Protocol::SkillSlot::SKILL_SLOT_ATTACK)
	{
		NormalAttackSkill = skill;
	}
	else if (InSkillSlot == Protocol::SkillSlot::SKILL_SLOT_Q)
	{
		QSkill = skill;
	}
	else if (InSkillSlot == Protocol::SkillSlot::SKILL_SLOT_W)
	{
		WSkill = skill;
	}
	else if (InSkillSlot == Protocol::SkillSlot::SKILL_SLOT_E)
	{
		ESkill = skill;
	}
	else if (InSkillSlot == Protocol::SkillSlot::SKILL_SLOT_R)
	{
		RSkill = skill;
	}
	else if (InSkillSlot == Protocol::SkillSlot::SKILL_SLOT_DASH)
	{
		DashSkill = skill;
	}

	skill->SetInfo(Owner, InTemplateId);
	skill->Slot = InSkillSlot;
	SkillList.Add(skill);
}

void UJ1SkillComponent::RegisterNormalAttack()
{
	Protocol::C_SKILL skillPkt;
	skillPkt.set_object_id(Owner->GetPosInfo()->object_id());
	skillPkt.set_slot(Protocol::SKILL_SLOT_ATTACK);

	Owner->GetManager(Network)->SendPacket(skillPkt);
}

void UJ1SkillComponent::HandleGameplayEvent(FGameplayTag InEventTag)
{
	for (TObjectPtr<UJ1SkillBase> skill : SkillList)
	{
		if (InEventTag.MatchesTag(skill->SkillTag) == true)
		{
			skill->HandleGameplayEvent(InEventTag);
		}
	}
}

bool UJ1SkillComponent::GetCanUseSkillBySkillSlot(const Protocol::SkillSlot& skillSlot)
{
	for (TObjectPtr<UJ1SkillBase> skill : SkillList)
	{
		if (skill->Slot == skillSlot)
		{
			return skill->GetCanUseSkill();
		}
	}

	//if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_ATTACK)
	//{
	//	return NormalAttackSkill->GetCanUseSkill();
	//}
	//else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_Q)
	//{
	//	return QSkill->GetCanUseSkill();
	//}
	//else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_W)
	//{
	//	return WSkill->GetCanUseSkill();
	//}
	//else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_E)
	//{
	//	return ESkill->GetCanUseSkill();
	//}
	//else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_R)
	//{
	//	return RSkill->GetCanUseSkill();
	//}
	//else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_DASH)
	//{
	//	return DashSkill->GetCanUseSkill();
	//}

	return false;
}

void UJ1SkillComponent::DoSkill(const Protocol::SkillSlot& skillSlot)
{
	if (GetCanUseSkillBySkillSlot(skillSlot) == false)
		return;

	for (TObjectPtr<UJ1SkillBase> skill : SkillList)
	{
		if (skill->Slot == skillSlot)
		{
			skill->DoSkill();
			return;
		}
	}

	//if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_ATTACK)
	//{
	//	return NormalAttackSkill->DoSkill();
	//}
	//else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_Q)
	//{
	//	return QSkill->DoSkill();
	//}
	//else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_W)
	//{
	//	return WSkill->DoSkill();
	//}
	//else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_E)
	//{
	//	return ESkill->DoSkill();
	//}
	//else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_R)
	//{
	//	return RSkill->DoSkill();
	//}
	//else if (skillSlot == Protocol::SkillSlot::SKILL_SLOT_DASH)
	//{
	//	return DashSkill->DoSkill();
	//}
}
