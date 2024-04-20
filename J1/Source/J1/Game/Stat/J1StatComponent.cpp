#include "J1StatComponent.h"
#include "Data/J1Data.h"
#include "J1/Game/Object/J1Creature.h"

UJ1StatComponent::UJ1StatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bShouldRefresh = false;
}

UJ1StatComponent::~UJ1StatComponent()
{
}


// Called when the game starts
void UJ1StatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UJ1StatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// temp
	if (bShouldRefresh == true)
	{
		RefreshAll();
	}
}

void UJ1StatComponent::SetInfo(TObjectPtr<AJ1Creature> InOwner, TObjectPtr<UCreatureData> InCreatureData, Protocol::CreatureType InCreatureType)
{
	Owner = InOwner;

	/* creature */
	BaseMaxHp = InCreatureData->MaxHp;
	SetMaxHp(BaseMaxHp);
	SetHp(BaseMaxHp);

	BaseAtk = InCreatureData->Atk;
	SetAtk(BaseAtk);

	BaseDef = InCreatureData->Def;
	SetDef(BaseDef);

	// todo : player, monster ...

}

void UJ1StatComponent::SetHp(float InHp)
{
	Hp = InHp;
}

float UJ1StatComponent::GetHp()
{
	return Hp;
}

void UJ1StatComponent::SetMaxHp(float InMaxHp)
{
	MaxHp = InMaxHp;
}

float UJ1StatComponent::GetMaxHp()
{
	return MaxHp;
}

void UJ1StatComponent::SetAtk(float InAtk)
{
	Atk = InAtk;
}

float UJ1StatComponent::GetAtk()
{
	return Atk;
}

void UJ1StatComponent::SetDef(float InDef)
{
	Def = InDef;
}

float UJ1StatComponent::GetDef()
{
	return Def;
}

void UJ1StatComponent::RefreshAll()
{
}

