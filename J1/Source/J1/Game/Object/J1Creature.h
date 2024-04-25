#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "J1/Data/J1Data.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "J1Creature.generated.h"

class Protocol::PosInfo;
class UCreatureData;
class UJ1SkillComponent;
class UJ1SkillBase;
class UJ1StatComponent;

UCLASS()
class J1_API AJ1Creature : public ACharacter
{
public:
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJ1Creature();
	~AJ1Creature();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	/** setter & getter **/
	void SetMoveState(Protocol::MoveState State);
	Protocol::MoveState GetMoveState() { return PosInfo->state(); }

	void SetPosInfo(const Protocol::PosInfo& Info, bool Forced = false);
	Protocol::PosInfo* GetPosInfo() { return PosInfo; }

	void SetObjectInfo(const Protocol::ObjectInfo& InObjectInfo);
	Protocol::ObjectInfo* GetObjectInfo() { return ObjectInfo; }

	TObjectPtr<UCreatureData> GetCreatureData() { return CreatureData; }

	uint64 GetObjectId() { return ObjectId; }
	int32 GetTemplateId() { return TemplateId; }

public:
	/** Component **/

	/* Stat */
	TObjectPtr<UJ1StatComponent> StatComponent;

	/* Skill */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UJ1SkillComponent> SkillComponent;

public:
	/** battle **/
	virtual void OnDamaged(TObjectPtr<AActor> InAttacker, TObjectPtr<UJ1SkillBase> InSkill);

public:
	/** anim event **/
	virtual void HandleGameplayEvent(FGameplayTag EventTag);

public:
	/** network **/

	virtual void ProcessMove(const Protocol::PosInfo& Info);

	virtual void ProcessSkill(const Protocol::S_SKILL& InSkillPkt);

	

public:
	/** initialize info **/
	virtual void SetInfo(const Protocol::ObjectInfo& InObjectInfo);

protected:
	/** information **/
	Protocol::ObjectInfo* ObjectInfo; // 정보

	Protocol::PosInfo* PosInfo; // 위치 정보

	uint64 ObjectId; // instance id
	int32 TemplateId; // data id

	FGameplayTag TemplateTag; // key tag for data id

	TObjectPtr<UCreatureData> CreatureData;
};
