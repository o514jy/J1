#include "J1GimmickComponent.h"
#include "J1GimmickBase.h"
#include "J1/Game/Object/J1Boss.h"
#include "J1/Game/Skill/J1SkillManager.h"

UJ1GimmickComponent::UJ1GimmickComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UJ1GimmickComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UJ1GimmickComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UJ1GimmickComponent::SetInfo(TObjectPtr<AJ1Boss> InOwner)
{
	Owner = InOwner;

	AddGimmick(2000);
}

void UJ1GimmickComponent::AddGimmick(int32 InTemplateId)
{
	if (InTemplateId < 1)
	{
		return;
	}

	TObjectPtr<UJ1GimmickBase> gimmick = Owner->GetManager(Skill)->GenerateGimmickById(InTemplateId);
	if (gimmick == nullptr)
	{
		//ensureAlwaysMsgf(gimmick, TEXT("Can't find gimmick from [%d]."), InTemplateId);
		return;
	}

	CanActiveGimmickList.Add(gimmick);
}

void UJ1GimmickComponent::DoGimmick(const Protocol::S_GIMMICK& GimmickPkt)
{
	for (TObjectPtr<UJ1GimmickBase> gimmick : CanActiveGimmickList)
	{
		if (gimmick->GimmickData->DataId == GimmickPkt.gimmick_data_id())
		{
			gimmick->DoGimmick(GimmickPkt);
			return;
		}
	}
}

