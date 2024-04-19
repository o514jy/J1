// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Skill/J1NormalAttack.h"

UJ1NormalAttack::UJ1NormalAttack()
{
}

UJ1NormalAttack::~UJ1NormalAttack()
{
}

void UJ1NormalAttack::SetInfo(TObjectPtr<AJ1Creature> InOwner, int32 InTemplateId)
{
	Super::SetInfo(InOwner, InTemplateId);
}

void UJ1NormalAttack::DoSkill()
{
	Super::DoSkill();
}
