// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "Game/Controllers/J1CreatureController.h"
#include "J1PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class J1_API AJ1PlayerController : public AJ1CreatureController
{
public:
	GENERATED_BODY()
	
public:
	virtual void ProcessSkill(const Protocol::SkillSlot& slot) override;
};
