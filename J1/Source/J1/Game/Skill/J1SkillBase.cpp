#include "J1SkillBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimMontage.h"
#include "J1/J1GameplayTags.h"
#include "System/J1AssetManager.h"
#include "J1/Game/Object/J1Creature.h"
#include "J1/Data/J1DataManager.h"
#include "J1/Data/J1AnimData.h"
#include "J1/Game/Object/J1MyPlayer.h"


UJ1SkillBase::UJ1SkillBase()
{
	Owner = nullptr;
	SkillData = nullptr;
	bCanUseSkill = true;
	TimeCount = 0;

	ImpactPos = new Protocol::SimplePosInfo();
}

UJ1SkillBase::~UJ1SkillBase()
{
	Owner = nullptr;
	SkillData = nullptr;
	bCanUseSkill = true;

	delete ImpactPos;
}

void UJ1SkillBase::SetInfo(TObjectPtr<AJ1Creature> InOwner, int32 InTemplateId)
{
	Owner = InOwner;
	SkillData = Owner->GetManager(Data)->GameData->SkillData[InTemplateId];
	SkillTag = Owner->GetManager(Data)->SetSkillTagByDataId(InTemplateId);

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

void UJ1SkillBase::DoSkill(const Protocol::S_SKILL& InSkillPkt)
{
	// 우선 움직이고 있었다면 멈추기
	Owner->GetController()->StopMovement();
	// 내 플레이어면 움직이는거 멈추는거 추가로 해줘야됨
	if (Owner->IsMyPlayer() == true)
		Cast<AJ1MyPlayer>(Owner)->SetAutoRunning(false);

	Owner->SetMoveState(Protocol::MoveState::MOVE_STATE_SKILL);
	ImpactPos->CopyFrom(InSkillPkt.simple_pos_info());

	// 방향 전환
	FRotator lookRot = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(),
		FVector(ImpactPos->x(), ImpactPos->y(), Owner->GetActorLocation().Z));
	Owner->SetActorRotation(lookRot);

	// 애니메이션 실행
	Owner->PlayAnimMontage(Montage);
}

TObjectPtr<UNiagaraComponent> UJ1SkillBase::SpawnNiagaraAtLocation(TObjectPtr<UNiagaraSystem> InNiagaraSystem, FVector InLocation)
{
	UObject* world = Owner->GetWorld();
	return UNiagaraFunctionLibrary::SpawnSystemAtLocation(world, InNiagaraSystem, InLocation);
}

void UJ1SkillBase::HandleGameplayEvent(FGameplayTag InEventTag)
{
	OnAttackEvent(TimeCount++);
}

void UJ1SkillBase::OnAttackEvent(int32 InTimeCount)
{
	// 끝났을 때
	if (InTimeCount == SkillData->AnimImpactTimeList.Num())
	{
		EndSkillEvent();
	}
}

void UJ1SkillBase::EndSkillEvent()
{
	TimeCount = 0;

	if (Owner->GetMoveState() == Protocol::MoveState::MOVE_STATE_SKILL)
	{
		Owner->SetMoveState(Protocol::MoveState::MOVE_STATE_IDLE);
	}
}
