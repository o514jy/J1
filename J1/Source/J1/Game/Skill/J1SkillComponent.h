#pragma once

#include "CoreMinimal.h"
#include "J1/Data/J1Data.h"
#include "J1Core.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "J1SkillComponent.generated.h"

class AJ1Creature;
class UJ1SkillBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class J1_API UJ1SkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJ1SkillComponent();
	~UJ1SkillComponent();

public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/** initialize **/
	void SetInfo(TObjectPtr<AJ1Creature> InOwner, TObjectPtr<UCreatureData> InCreatureData);

	void AddSkill(int32 InTemplateId, Protocol::SkillSlot InSkillSlot);

public:
	/** network for player skill **/
	void RegisterNormalAttack(FVector InLocation);
	void RegisterAuroraQ(FVector InLocation);

public:
	/** handle event **/
	void HandleGameplayEvent(FGameplayTag InEventTag);

public:
	/** process **/
	bool GetCanUseSkillBySkillSlot(const Protocol::SkillSlot& skillSlot);

	void DoSkill(const Protocol::S_SKILL& InSkillPkt);

public:
	/** information**/
	UPROPERTY()
	TObjectPtr<AJ1Creature> Owner;

	FGameplayTag SkillComponentTag;

	/** creature **/
	UPROPERTY()
	TArray<TObjectPtr<UJ1SkillBase>> SkillList;

	UPROPERTY()
	TObjectPtr<UJ1SkillBase> NormalAttackSkill;

	/** player **/
	UPROPERTY()
	TObjectPtr<UJ1SkillBase> QSkill;
	UPROPERTY()
	TObjectPtr<UJ1SkillBase> WSkill;
	UPROPERTY()
	TObjectPtr<UJ1SkillBase> ESkill;
	UPROPERTY()
	TObjectPtr<UJ1SkillBase> RSkill;
	UPROPERTY()
	TObjectPtr<UJ1SkillBase> DashSkill;

	/** monster **/
	UPROPERTY()
	TObjectPtr<UJ1SkillBase> AdvancedSkill;
};
