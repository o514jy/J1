// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "J1Data.h"
#include "J1Core.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "J1DataManager.generated.h"


UCLASS()
class J1_API UJ1DataManager : public UGameInstanceSubsystem
{
public:
	GENERATED_BODY()

public:
	void SetInfo();
	
public:
	void ParseAllJsonData();

public:
	UPROPERTY(BlueprintType)
	TObjectPtr<UJ1GameData> GameData;
};
