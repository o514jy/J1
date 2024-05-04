#include "J1DataManager.h"
#include "J1/J1GameplayTags.h"

void UJ1DataManager::SetInfo()
{
	ParseAllJsonData();
}

void UJ1DataManager::ParseAllJsonData()
{
	GameData = NewObject<UJ1GameData>();
	FString FilePath = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("../../Common/Data/GameData.json"));
	GameData->ParseJsonData(FilePath);
}

FGameplayTag UJ1DataManager::SetTemplateTagByDataId(int32 InTemplateId)
{
	switch (InTemplateId)
	{
	case 1:
		return J1GameplayTags::Aurora;
	case 100:
		return J1GameplayTags::Sevarog;
	}

	return FGameplayTag();
}

FGameplayTag UJ1DataManager::SetSkillComponentTagByDataId(int32 InTemplateId)
{
	switch (InTemplateId)
	{
	case 1:
		return J1GameplayTags::Aurora_Skill;
	case 100:
		return J1GameplayTags::Sevarog_Skill;
	}

	return FGameplayTag();
}

FGameplayTag UJ1DataManager::SetSkillTagByDataId(int32 InTemplateId)
{
	switch (InTemplateId)
	{
	case 1000:
		return J1GameplayTags::Aurora_Skill_NormalAttack;
	case 1001:
		return J1GameplayTags::Aurora_Skill_QSkill;
	case 1100:
		return J1GameplayTags::Sevarog_Skill_NormalAttack;
	case 1101:
		return J1GameplayTags::Sevarog_Skill_PoisonRain;
	}

	return FGameplayTag();
}

FGameplayTag UJ1DataManager::SetGimmickTagByDataId(int32 InTemplateId)
{
	switch (InTemplateId)
	{
	case 2000:
		return J1GameplayTags::Sevarog_Gimmick_FindSafeZone;
	}

	return FGameplayTag();
}
