#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "Components/ActorComponent.h"
#include "J1StatComponent.generated.h"

class AJ1Creature;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class J1_API UJ1StatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UJ1StatComponent();
	~UJ1StatComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/** initialize **/
	void SetInfo(TObjectPtr<AJ1Creature> InOwner, TObjectPtr<UCreatureData> InCreatureData, Protocol::CreatureType InCreatureType);

public:
	/** setter & getter **/
	void SetHp(float InHp);
	float GetHp();

	void SetMaxHp(float InMaxHp);
	float GetMaxHp();

	void SetAtk(float InAtk);
	float GetAtk();

	void SetDef(float InDef);
	float GetDef();

public:
	/** refresh **/
	void RefreshAll();

private:	
	/** information **/
	TObjectPtr<AJ1Creature> Owner;

	/** stat **/

	/* creature */
	UPROPERTY()
	float Hp;

	UPROPERTY()
	float BaseMaxHp;
	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float BaseAtk; 
	UPROPERTY()
	float Atk;

	UPROPERTY()
	float BaseDef;
	UPROPERTY()
	float Def;

private:
	/** dirty flag to refresh stat **/
	UPROPERTY()
	bool bShouldRefresh;
		
};
