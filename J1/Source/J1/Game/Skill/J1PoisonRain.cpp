#include "J1PoisonRain.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Data/J1Data.h"
#include "Data/J1NiagaraData.h"
#include "J1/System/J1AssetManager.h"
#include "Components/SceneComponent.h"

UJ1PoisonRain::UJ1PoisonRain()
{
	NS_PoisonRain = nullptr;
}

UJ1PoisonRain::~UJ1PoisonRain()
{
	NS_PoisonRain = nullptr;
}

void UJ1PoisonRain::SetInfo(TObjectPtr<AJ1Creature> InOwner, int32 InTemplateId)
{
	Super::SetInfo(InOwner, InTemplateId);

	RandomPoses.SetNum(SkillData->ProjectileIdList.Num());

	UJ1NiagaraData* NiagaraData = UJ1AssetManager::GetAssetByName<UJ1NiagaraData>("NiagaraData");
	FJ1NiagaraEntry& Entry = NiagaraData->NameToNiagaraEntry[TEXT("NS_Poison_Rain")];
	NS_PoisonRain = Entry.Niagara;
}

void UJ1PoisonRain::DoSkill(const Protocol::S_SKILL& InSkillPkt)
{
	Super::DoSkill(InSkillPkt);

	for (int i = 0; i < RandomPoses.Num(); i++)
	{
		const Protocol::SimplePosInfo& posinfo = InSkillPkt.poison_rain_random_poses(i);
		FVector pos;
		pos.X = posinfo.x();
		pos.Y = posinfo.y();
		pos.Z = posinfo.z();
		RandomPoses[i] = pos;
	}
}

void UJ1PoisonRain::OnAttackEvent(int32 InTimeCount)
{
	Super::OnAttackEvent(InTimeCount);

	if (InTimeCount == SkillData->AnimImpactTimeList.Num())
		return;

	//UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(FireEffectMuzzle, WeaponMuzzle, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
	//NiagaraComp->SetNiagaraVariableFloat(FString("StrengthCoef"), CoefStrength);

	FVector loc = RandomPoses[InTimeCount];
	loc.Z += 100.f;

	// temp
	if (!NS_PoisonRain)
	{
		int a = 3;
	}

	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, NS_PoisonRain, loc);
	if (NiagaraComp)
	{

	}
	else
	{
		int a = 3;
	}
	//NiagaraComp->SetNiagaraVariableFloat(FString)
}
