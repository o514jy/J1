// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Skill/J1SkillBase.h"
#include "J1NormalAttack.generated.h"

/**
 * 
 */
UCLASS()
class J1_API UJ1NormalAttack : public UJ1SkillBase
{
	GENERATED_BODY()
public:
	UJ1NormalAttack();
	~UJ1NormalAttack();

public:
	/** initialize **/
	virtual void SetInfo(TObjectPtr<AJ1Creature> InOwner, int32 InTemplateId);

public:
	/** Process Skill **/
	virtual void DoSkill() override;

public:
	/** handle event **/
	virtual void OnAttackEvent(int32 InTimeCount) override;

};
