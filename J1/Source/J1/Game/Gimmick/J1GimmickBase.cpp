#include "J1GimmickBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "J1/Game/Object/J1Boss.h"
#include "J1/Data/J1DataManager.h"
#include "J1/Data/J1Data.h"
#include "J1/System/J1AssetManager.h"
#include "J1/Data/J1AnimData.h"
#include "Animation/AnimMontage.h"

UJ1GimmickBase::UJ1GimmickBase()
{
	TimeCount = 0;
}

UJ1GimmickBase::~UJ1GimmickBase()
{
}

void UJ1GimmickBase::SetInfo(TObjectPtr<AJ1Boss> InOwner, int32 InTemplateId)
{
	Owner = InOwner;

	GimmickData = Owner->GetManager(Data)->GameData->GimmickData[InTemplateId];
	GimmickTag = Owner->GetManager(Data)->SetGimmickTagByDataId(InTemplateId);

	int32 OwnerDataId = Owner->GetTemplateId();
	UJ1AnimData* InputData = UJ1AssetManager::GetAssetByName<UJ1AnimData>("AnimData");
	FJ1AnimSet& AnimSet = InputData->OwnerIdToAnimSet[OwnerDataId];
	FJ1AnimEntry& AnimEntry = AnimSet.SkillIdToAnimEntry[InTemplateId];
	Montage = AnimEntry.Montage;

	if (Montage == nullptr)
	{
		return;
	}
}

void UJ1GimmickBase::HandleGameplayEvent(FGameplayTag InEventTag)
{
	OnAttackEvent(TimeCount++);
}

void UJ1GimmickBase::OnAttackEvent(int32 InTimeCount)
{
	// 끝났을 때
	if (InTimeCount == GimmickData->EventTimeList.Num())
	{
		EndGimmickEvent();
	}
}

void UJ1GimmickBase::EndGimmickEvent()
{
	TimeCount = 0;

	if (Owner->GetMoveState() == Protocol::MoveState::MOVE_STATE_GIMMICK)
	{
		Owner->SetMoveState(Protocol::MoveState::MOVE_STATE_IDLE);
	}
}

void UJ1GimmickBase::DoGimmick(const Protocol::S_GIMMICK& InGimmickPkt)
{
	// 우선 움직이고 있었다면 멈추기
	Owner->GetController()->StopMovement();

	Owner->SetMoveState(Protocol::MoveState::MOVE_STATE_GIMMICK);

	// 방향 전환 할거면 함
	FRotator lookRot = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(),
		FVector(-1, 0, 0));
	Owner->SetActorRotation(lookRot);

	// 애니메이션 실행
	Owner->PlayAnimMontage(Montage);
}
