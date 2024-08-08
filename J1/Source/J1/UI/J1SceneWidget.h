// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "J1SceneWidget.generated.h"

class UJ1DungeonStatusWidget;

UCLASS()
class J1_API UJ1SceneWidget : public UUserWidget
{
	GENERATED_BODY()
public:


	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UJ1DungeonStatusWidget> DungeonStatusWBP;

};
