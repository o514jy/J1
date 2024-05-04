#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "J1GimmickBase.generated.h"

class AJ1Boss;
class UGimmickData;
class UAnimMontage;

UCLASS()
class J1_API UJ1GimmickBase : public UObject
{
	GENERATED_BODY()
public:
	UJ1GimmickBase();
	~UJ1GimmickBase();

public:
	/** initialize **/
	virtual void SetInfo(TObjectPtr<AJ1Boss> InOwner, int32 InTemplateId);

public:
	/** handle event **/
	void HandleGameplayEvent(FGameplayTag InEventTag);

	virtual void OnAttackEvent(int32 InTimeCount);

	virtual void EndGimmickEvent();

public:
	/** Process Gimmick **/
	virtual void DoGimmick(const Protocol::S_GIMMICK& InGimmickPkt);

public:
	/** information **/
	UPROPERTY()
	TObjectPtr<AJ1Boss> Owner;

	FGameplayTag GimmickTag;

	UPROPERTY()
	TObjectPtr<UAnimMontage> Montage;

	UPROPERTY()
	TObjectPtr<UGimmickData> GimmickData;

	int32 TimeCount; // index of AnimImpactTime
};
