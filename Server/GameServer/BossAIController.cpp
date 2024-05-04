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

	// ��� ���� �������� ��� �ٷ� ����
	// �ϴ� �÷��̾ ã������ ����
	if (_owner->GetState() == Protocol::MoveState::MOVE_STATE_RUN)
	{
		BossRef boss = static_pointer_cast<Boss>(_owner);
		// ���� ������ ����� ������ ����
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

	// �̹� ��� �������̸� ���
	if (ownerBoss->GetGimmickComponent()->GetActiveGimmick() != nullptr)
	{
		return;
	}

	// todo : ��� ���� ���� �����ֱ�

	// ��� ����
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
