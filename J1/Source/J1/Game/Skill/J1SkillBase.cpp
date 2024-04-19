#include "J1SkillBase.h"
#include "Animation/AnimMontage.h"
#include "System/J1AssetManager.h"
#include "J1/Game/Object/J1Creature.h"
#include "J1/Data/J1DataManager.h"
#include "J1/Data/J1AnimData.h"

UJ1SkillBase::UJ1SkillBase()
{
	Owner = nullptr;
	SkillData = nullptr;
	bCanUseSkill = true;
}

UJ1SkillBase::~UJ1SkillBase()
{
	Owner = nullptr;
	SkillData = nullptr;
	bCanUseSkill = true;
}

void UJ1SkillBase::SetInfo(TObjectPtr<AJ1Creature> InOwner, int32 InTemplateId)
{
	Owner = InOwner;
	SkillData = Owner->GetManager(Data)->GameData->SkillData[InTemplateId];

	int32 OwnerDataId = Owner->GetTemplateId();
	UJ1AnimData* InputData = UJ1AssetManager::GetAssetByName<UJ1AnimData>("AnimData");
	FJ1AnimSet& AnimSet = InputData->OwnerIdToAnimSet[OwnerDataId];
	FJ1AnimEntry& AnimEntry = AnimSet.SkillIdToAnimEntry[InTemplateId];
	Montage = AnimEntry.Montage;

	if (Montage == nullptr)
	{
		return;
	}

	// start cooltime
	SetCanUseSkill(true);
}

void UJ1SkillBase::SetCanUseSkill(bool bFlag)
{
	bCanUseSkill = bFlag;
}

bool UJ1SkillBase::GetCanUseSkill()
{
	return bCanUseSkill;
}

void UJ1SkillBase::DoSkill()
{
	Owner->SetMoveState(Protocol::MoveState::MOVE_STATE_SKILL);

	Owner->PlayAnimMontage(Montage);
}
