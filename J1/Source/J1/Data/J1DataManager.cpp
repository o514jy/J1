// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/J1DataManager.h"

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
