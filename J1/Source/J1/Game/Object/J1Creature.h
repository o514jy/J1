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
class UWidgetComponent;

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

	virtual void SetPosInfo(const Protocol::PosInfo& Info, bool Forced = false);
	Protocol::PosInfo* GetPosInfo() { return PosInfo; }

	void SetObjectInfo(const Protocol::ObjectInfo& InObjectInfo);
	Protocol::ObjectInfo* GetObjectInfo() { return ObjectInfo; }

	void SetNowPosInfo(FVector InLocation = FVector());
	void SetDestPosInfo(FVector InLocation);

	TObjectPtr<UCreatureData> GetCreatureData() { return CreatureData; }

	uint64 GetObjectId() { return ObjectId; }
	int32 GetTemplateId() { return TemplateId; }

public:
	/** Component **/

	/* Stat */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UJ1StatComponent> StatComponent;

	/* Skill */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UJ1SkillComponent> SkillComponent;

	/* UI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> HpBarComponent;

public:
	/** battle **/
	virtual void OnDamaged(TObjectPtr<AActor> InAttacker, TObjectPtr<UJ1SkillBase> InSkill);

public:
	/** anim event **/
	virtual void HandleGameplayEvent(FGameplayTag EventTag);

public:
	/** temp **/
	void destroyProjectile();

public:
	/** network **/

	virtual void ProcessMove(const Protocol::PosInfo& Info);

	virtual void ProcessSkill(const Protocol::S_SKILL& InSkillPkt);

	virtual void ProcessNotifyPos(const Protocol::PosInfo& Info);

	virtual void ProcessProjectile(const Protocol::ProjectileInfo& Info);

public:
	/** initialize info **/
	virtual void SetInfo(const Protocol::ObjectInfo& InObjectInfo);

public:
	/** state **/
	virtual void OnDead();

public:
	/** helper **/
	virtual bool IsMyPlayer();

public:
	/** information **/
	Protocol::ObjectInfo* ObjectInfo; // 정보

	Protocol::PosInfo* PosInfo; // 위치 정보

	uint64 ObjectId; // instance id
	int32 TemplateId; // data id

	FGameplayTag TemplateTag; // key tag for data id

	TObjectPtr<UCreatureData> CreatureData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	// temp
	AActor* UpZone;
	AActor* LeftZone;
	AActor* DownZone;
	AActor* RightZone;
};
