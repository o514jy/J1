#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "J1Data.generated.h"

UCLASS(BlueprintType)
class UCreatureData : public UDataAsset
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 DataId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString CreatureType;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString DescriptionTextId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float ColliderRadius;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float ColliderHalfHeight;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float MaxHp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float Atk;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float Def;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float MaxWalkSpeed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float MoveSpeedRate;
};

UCLASS(BlueprintType)
class UPlayerData : public UCreatureData
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 SkillAttackId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 SkillQId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 SkillWId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 SkillEId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 SkillRId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 SkillDashId;
};

UCLASS(BlueprintType)
class UMonsterData : public UCreatureData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float SearchMaxDistance;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float ChaseMaxDistance;
};

UCLASS(BlueprintType)
class UBossData : public UMonsterData
{
	GENERATED_BODY()
public:

};

UCLASS(BlueprintType)
class USkillData : public UDataAsset
{
public:
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 DataId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 OwnerSkillDataId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString Description;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<int32> ProjectileIdList;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float CoolTime;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float SkillDuration;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<int32> AnimImpactTimeList;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<int32> BuffIdList;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<int32> EffectIdList;
};

UCLASS(BlueprintType)
class UEffectData : public UDataAsset
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 DataId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString EffectType;
};

UCLASS(BlueprintType)
class URectangleEffectData : public UEffectData
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float ForwardLength;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float BackwardLength;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float LeftLength;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float RightLength;
};

UCLASS(BlueprintType)
class UPizzaEffectData : public UEffectData
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float Radius;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float Theta;
};

UCLASS(BlueprintType)
class UBuffData : public UDataAsset
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 DataId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString BuffType;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString BuffDurationType;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float BuffAmountRate;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float BuffDurationPeriod;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float BuffDurationMagnitude;
};

UCLASS(BlueprintType)
class J1_API UJ1GameData : public UDataAsset
{
public:
	GENERATED_BODY()
	
public:
	void ParseJsonData(const FString& path);

public:
	/** setter & getter **/

public:
	/* creature */
	UPROPERTY(BlueprintType)
	TMap<int32, TObjectPtr<UPlayerData>> PlayerData;

	UPROPERTY(BlueprintType)
	TMap<int32, TObjectPtr<UBossData>> BossData;

	/* skill */
	UPROPERTY(BlueprintType)
	TMap<int32, TObjectPtr<USkillData>> SkillData;

	/* effect */
	UPROPERTY(BlueprintType)
	TMap<int32, TObjectPtr<UEffectData>> EffectData;

	/* buff */
	UPROPERTY(BlueprintType)
	TMap<int32, TObjectPtr<UBuffData>> BuffData;
};
