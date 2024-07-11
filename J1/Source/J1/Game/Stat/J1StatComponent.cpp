#include "J1StatComponent.h"
#include "Components/WidgetComponent.h"
#include "Data/J1Data.h"
#include "J1/Game/Object/J1Creature.h"
#include "J1/UI/J1HpBarWidget.h"

UJ1StatComponent::UJ1StatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bShouldRefresh = false;
	StatInfo = new Protocol::StatInfo();
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
}

void UJ1StatComponent::SetInfo(TObjectPtr<AJ1Creature> InOwner, TObjectPtr<UCreatureData> InCreatureData, Protocol::CreatureType InCreatureType)
{
	Owner = InOwner;

	Owner->GetObjectInfo()->set_allocated_stat_info(StatInfo);

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
	StatInfo->set_hp(InHp);
	Hp = InHp;

	if (Owner->HpBarComponent)
	{
		float Ratio = static_cast<float>(Hp) / MaxHp; 
		UJ1HpBarWidget* HpBar = Cast<UJ1HpBarWidget>(Owner->HpBarComponent->GetUserWidgetObject());
		if (HpBar)
			HpBar->SetHpRatio(Ratio);
	}
	
	if (Hp == 0.f)
	{
		if (Owner->PosInfo->state() != Protocol::MoveState::MOVE_STATE_DEAD)
			Owner->SetMoveState(Protocol::MoveState::MOVE_STATE_DEAD);
	}

	ScreenDebugMessageString(FString::Printf(TEXT("%d hp update : %f"), Owner->GetObjectId(), Hp));
}

float UJ1StatComponent::GetHp()
{
	return Hp;
}

void UJ1StatComponent::SetMaxHp(float InMaxHp)
{
	StatInfo->set_max_hp(InMaxHp);
	MaxHp = InMaxHp;
}

float UJ1StatComponent::GetMaxHp()
{
	return MaxHp;
}

void UJ1StatComponent::SetAtk(float InAtk)
{
	StatInfo->set_atk(InAtk);
	Atk = InAtk;
}

float UJ1StatComponent::GetAtk()
{
	return Atk;
}

void UJ1StatComponent::SetDef(float InDef)
{
	StatInfo->set_def(InDef);
	Def = InDef;
}

float UJ1StatComponent::GetDef()
{
	return Def;
}

void UJ1StatComponent::ProcessStat(const Protocol::StatInfo& InStatInfo)
{
	StatInfo->CopyFrom(InStatInfo);

	SetHp(InStatInfo.hp());
	SetMaxHp(InStatInfo.max_hp());
	SetAtk(InStatInfo.atk());
	SetDef(InStatInfo.def());

	RefreshAll();
}

void UJ1StatComponent::RefreshAll()
{
	// 계산 및 ui 초기화
}

