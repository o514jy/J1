#include "pch.h"
#include "BossAIController.h"

BossAIController::BossAIController()
{
}

BossAIController::~BossAIController()
{
}

void BossAIController::SetInfo(ObjectRef owner)
{
	__super::SetInfo(owner);
}

void BossAIController::UpdateTick()
{

	__super::UpdateTick();
}

void BossAIController::UpdateIdle()
{
	__super::UpdateIdle();
}

void BossAIController::UpdateRun()
{
	__super::UpdateRun();
}

void BossAIController::UpdateSkill()
{
	__super::UpdateSkill();
}

void BossAIController::UpdateDead()
{
	__super::UpdateDead();
}
