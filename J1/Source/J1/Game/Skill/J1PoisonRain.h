#pragma once

#include "CoreMinimal.h"
#include "Game/Skill/J1SkillBase.h"
#include "J1PoisonRain.generated.h"
// NS_Poison_Rain

class UNiagaraSystem;

UCLASS()
class J1_API UJ1PoisonRain : public UJ1SkillBase
{
	GENERATED_BODY()
public:
	UJ1PoisonRain();
	~UJ1PoisonRain();

public:
	/** initialize **/
	virtual void SetInfo(TObjectPtr<AJ1Creature> InOwner, int32 InTemplateId);

public:
	/** Process Skill **/
	virtual void DoSkill(const Protocol::S_SKILL& InSkillPkt) override;

public:
	/** handle event **/
	virtual void OnAttackEvent(int32 InTimeCount) override;

public:
	/** information **/
	UPROPERTY(BlueprintType, VisibleAnywhere)
	TArray<FVector> RandomPoses;

	UPROPERTY()
	TObjectPtr<UNiagaraSystem> NS_PoisonRain;

	UPROPERTY()
	TObjectPtr<UNiagaraSystem> NS_PoisonRain_Fin;
};
