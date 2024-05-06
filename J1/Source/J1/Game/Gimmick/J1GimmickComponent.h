#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "J1GimmickComponent.generated.h"

class AJ1Boss;
class UJ1GimmickBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class J1_API UJ1GimmickComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UJ1GimmickComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/** initialize **/
	void SetInfo(TObjectPtr<AJ1Boss> InOwner);

	void AddGimmick(int32 InTemplateId);

public:
	/** process gimmick **/
	void DoGimmick(const Protocol::S_GIMMICK& GimmickPkt);

public:
	/** handle event **/
	void HandleGameplayEvent(FGameplayTag InEventTag);

public:
	/** information **/

	UPROPERTY()
	TObjectPtr<AJ1Boss> Owner;

	FGameplayTag GimmickComponentTag;

	UPROPERTY()
	TObjectPtr<UJ1GimmickBase> ActiveGimmick;

	UPROPERTY()
	TArray<TObjectPtr<UJ1GimmickBase>> CanActiveGimmickList;
};
