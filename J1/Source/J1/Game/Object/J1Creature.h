#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "J1Creature.generated.h"

class Protocol::PosInfo;
class UCreatureData;
class UJ1SkillComponent;

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
	TSharedPtr<Protocol::PosInfo> GetPosInfo() { return PosInfo; }

	void SetObjectInfo(const Protocol::ObjectInfo& InObjectInfo);
	TSharedPtr<Protocol::ObjectInfo> GetObjectInfo() { return ObjectInfo; }

	TObjectPtr<UCreatureData> GetCreatureData() { return CreatureData; }

	uint64 GetObjectId() { return ObjectId; }
	int32 GetTemplateId() { return TemplateId; }

public:
	/* Skill */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UJ1SkillComponent> SkillComponent;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBaseAttackEvent();

public:
	/** anim event **/
	virtual void HandleGameplayEvent(FGameplayTag EventTag);

public:
	/** network **/

	virtual void ProcessMove(const Protocol::PosInfo& Info);

	virtual void ProcessSkill(const Protocol::SkillSlot& Slot);

public:
	/** initialize info **/
	virtual void SetInfo(const Protocol::ObjectInfo& InObjectInfo);

protected:
	/** information **/
	TSharedPtr<Protocol::ObjectInfo> ObjectInfo; // 정보

	TSharedPtr<Protocol::PosInfo> PosInfo; // 위치 정보

	uint64 ObjectId; // instance id
	int32 TemplateId; // data id

	FGameplayTag TemplateTag; // key tag for data id

	TObjectPtr<UCreatureData> CreatureData;
};
