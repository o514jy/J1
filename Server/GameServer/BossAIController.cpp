#include "pch.h"
#include "BossAIController.h"
#include "Boss.h"
#include "GimmickComponent.h"

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
	PlayerRef targetPlayer = static_pointer_cast<Player>(ownerBoss->_targetObject);

	// �̹� ��� �������̸� ���
	if (ownerBoss->GetGimmickComponent()->GetActiveGimmick() != nullptr)
	{
		return;
	}

	// todo : ��� ���� ���� �����ֱ�

	// ��� ����
	{

	}
}

void BossAIController::UpdateDead()
{
	__super::UpdateDead();
}
