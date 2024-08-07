#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "J1SkillBase.generated.h"

class AJ1Creature;
class USkillData;
class UAnimMontage;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class J1_API UJ1SkillBase : public UObject
{
	GENERATED_BODY()
public:
	UJ1SkillBase();
	~UJ1SkillBase();

public:
	/** initialize **/
	virtual void SetInfo(TObjectPtr<AJ1Creature> InOwner, int32 InTemplateId);

public:
	/** setter & geter **/
	void SetCanUseSkill(bool bFlag);
	bool GetCanUseSkill();

public:
	/** Process Skill **/
	virtual void DoSkill(const Protocol::S_SKILL& InSkillPkt);

	virtual TObjectPtr<UNiagaraComponent> SpawnNiagaraAtLocation(TObjectPtr<UNiagaraSystem> InNiagaraSystem, FVector InLocation);

public:
	/** handle event **/
	void HandleGameplayEvent(FGameplayTag InEventTag);

	virtual void OnAttackEvent(int32 InTimeCount);

	virtual void EndSkillEvent();

public:
	/** information **/
	UPROPERTY()
	TObjectPtr<AJ1Creature> Owner;

	FGameplayTag SkillTag;

	UPROPERTY()
	TObjectPtr<USkillData> SkillData;

	UPROPERTY()
	TObjectPtr<UAnimMontage> Montage;

	Protocol::SkillSlot Slot;

	int32 TimeCount; // index of AnimImpactTime

	bool bCanUseSkill;

	Protocol::SimplePosInfo* ImpactPos;
};
