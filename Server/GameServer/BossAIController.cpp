#include "pch.h"
#include "BossAIController.h"
#include "Boss.h"
#include "GimmickComponent.h"
#include "StatComponent.h"
#include "FindSafeZone.h"
#include "Player.h"

BossAIController::BossAIController()
{
	_activeGimmickId = 0;
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
	//__super::UpdateTick();

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
	if (_owner->GetStatComponent()->GetHp() == _owner->GetStatComponent()->GetMaxHp())
		return;

	// �ϴ� �÷��̾ ã������ ����
	if (_owner->GetState() == Protocol::MoveState::MOVE_STATE_RUN)
	{
		BossRef boss = static_pointer_cast<Boss>(_owner);
		// ���� ������ ����� ������ ����
		_activeGimmickId = boss->GetGimmickComponent()->PopCanActiveGimmickId();
		if (_activeGimmickId != 0)
		{
			//boss->SetState(Protocol::MoveState::MOVE_STATE_GIMMICK);
		}
	}
}

void BossAIController::UpdateRun()
{
	//__super::UpdateRun();

	BossRef ownerMonster = static_pointer_cast<Boss>(_owner);
	BossDataRef ownerData = ownerMonster->GetBossData();
	PlayerRef targetPlayer = static_pointer_cast<Player>(ownerMonster->_targetObject);

	// Ÿ���� ������� idle�� ���ư���
	if (targetPlayer == nullptr || targetPlayer->room.load().lock() == nullptr)
	{
		ownerMonster->SetTargetObject(nullptr);
		ownerMonster->SetState(Protocol::MoveState::MOVE_STATE_IDLE);
		BroadcastMove();
		return;
	}

	// Ÿ���� ������ �Ѿư��� ���� ��Ÿ� �ȿ� ������ ����
	ChaseOrAttackTarget(ownerData->ChaseMaxDistance, ownerData->DefaultAtkRange);
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
		if (_activeGimmickId != 0)
		{
			boss->GetGimmickComponent()->DoGimmick(_activeGimmickId);
		}
	}
}

void BossAIController::UpdateDead()
{
	__super::UpdateDead();
}
