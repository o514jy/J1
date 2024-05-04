#include "pch.h"
#include "BossAIController.h"
#include "Boss.h"
#include "GimmickComponent.h"
#include "FindSafeZone.h"

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
	if (_owner->GetState() == Protocol::MoveState::MOVE_STATE_IDLE)
	{
		UpdateIdle();
	}
	else if (_owner->GetState() == Protocol::MoveState::MOVE_STATE_RUN)
	{
		UpdateRun();
	}
	else if (_owner->GetState() == Protocol::MoveState::MOVE_STATE_SKILL)
	{
		UpdateSkill();
	}
	else if (_owner->GetState() == Protocol::MoveState::MOVE_STATE_GIMMICK)
	{
		UpdateGimmick();
	}
	else if (_owner->GetState() == Protocol::MoveState::MOVE_STATE_DEAD)
	{
		UpdateDead();
	}
}

void BossAIController::UpdateIdle()
{
	__super::UpdateIdle();

	// 기믹 조건 맞춰지면 기믹 바로 시작
	// 일단 플레이어를 찾았으면 실행
	if (_owner->GetState() == Protocol::MoveState::MOVE_STATE_RUN)
	{
		BossRef boss = static_pointer_cast<Boss>(_owner);
		// 실행 가능한 기믹이 있으면 실행
		queue<int32> gimmickQueue = boss->GetGimmickComponent()->_gimmickIdQueue;
		if (gimmickQueue.empty() == false)
		{
			_owner->SetState(Protocol::MoveState::MOVE_STATE_GIMMICK);
		}
	}
}

void BossAIController::UpdateRun()
{
	__super::UpdateRun();
}

void BossAIController::UpdateSkill()
{
	__super::UpdateSkill();
}

void BossAIController::UpdateGimmick()
{
	BossRef ownerBoss = static_pointer_cast<Boss>(_owner);
	BossDataRef ownerData = ownerBoss->GetBossData();

	// 이미 기믹 진행중이면 통과
	if (ownerBoss->GetGimmickComponent()->GetActiveGimmick() != nullptr)
	{
		return;
	}

	// todo : 기믹 시작 조건 맞춰주기

	// 기믹 시작
	{
		//ownerBoss->GetGimmickComponent()->DoGimmick(FIND_SAFE_ZONE_DATA_ID);
		BossRef boss = static_pointer_cast<Boss>(_owner);
		queue<int32> gimmickIdQueue = boss->GetGimmickComponent()->_gimmickIdQueue;
		if (gimmickIdQueue.empty() == false)
		{
			boss->GetGimmickComponent()->DoGimmick(gimmickIdQueue.front());
			gimmickIdQueue.pop();
		}
	}
}

void BossAIController::UpdateDead()
{
	__super::UpdateDead();
}
