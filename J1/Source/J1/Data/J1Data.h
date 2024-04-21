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
	float AtkRange;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float Def;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float MaxWalkSpeed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float HpRate;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float AtkRate;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float DefRate;

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
	TArray<float> AnimImpactTimeList;

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
	float LeftUpPosY;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float LeftUpPosX;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float RightDownPosY;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float RightDownPosX;
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
	UPROPERTY(BlueprintType)
	TMap<int32, TObjectPtr<UPlayerData>> PlayerData;

	UPROPERTY(BlueprintType)
	TMap<int32, TObjectPtr<USkillData>> SkillData;

	UPROPERTY(BlueprintType)
	TMap<int32, TObjectPtr<UEffectData>> EffectData;

	UPROPERTY(BlueprintType)
	TMap<int32, TObjectPtr<UBuffData>> BuffData;
};
