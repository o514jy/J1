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
	}

	return FGameplayTag();
}

FGameplayTag UJ1DataManager::SetSkillComponentTagByDataId(int32 InTemplateId)
{
	switch (InTemplateId)
	{
	case 1:
		return J1GameplayTags::Aurora_Skill;
	}

	return FGameplayTag();
}

FGameplayTag UJ1DataManager::SetSkillTagByDataId(int32 InTemplateId)
{
	switch (InTemplateId)
	{
	case 1000:
		return J1GameplayTags::Aurora_Skill_NormalAttack;
	}

	return FGameplayTag();
}
