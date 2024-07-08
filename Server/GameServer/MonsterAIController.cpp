#include "pch.h"
#include "MonsterAIController.h"
#include "Player.h"
#include "Monster.h"
#include "Data.h"
#include "StartRoom.h"
#include "SkillComponent.h"

MonsterAIController::MonsterAIController()
{
}

MonsterAIController::~MonsterAIController()
{
}

void MonsterAIController::SetInfo(ObjectRef owner)
{
	__super::SetInfo(owner);
}

void MonsterAIController::UpdateTick()
{
	__super::UpdateTick();

	if (GetShouldSyncPos() == true)
		BroadcastNowPos();
}

void MonsterAIController::UpdateIdle()
{
	__super::UpdateIdle();

	MonsterRef _ownerMonster = static_pointer_cast<Monster>(_owner);

	RoomBaseRef room = _owner->room.load().lock();
	PlayerRef player = nullptr;
	//if (_ownerMonster->_targetObject == nullptr)
	player = room->FindClosestPlayer(_owner, _ownerMonster->GetMonsterData()->SearchMaxDistance);

	if (player != nullptr)
	{
		//_ownerMonster->_targetObject = player;
		_ownerMonster->SetTargetObject(player);
		_owner->SetState(Protocol::MoveState::MOVE_STATE_RUN);
	}
}

void MonsterAIController::UpdateRun()
{
	__super::UpdateRun();

	MonsterRef ownerMonster = static_pointer_cast<Monster>(_owner);
	MonsterDataRef ownerData = ownerMonster->GetMonsterData();
	PlayerRef targetPlayer = static_pointer_cast<Player>(ownerMonster->_targetObject);

	// 타겟이 사라지면 idle로 돌아가기
	if (targetPlayer == nullptr || targetPlayer->room.load().lock() == nullptr)
	{
		ownerMonster->SetTargetObject(nullptr);
		ownerMonster->SetState(Protocol::MoveState::MOVE_STATE_IDLE);
		//BroadcastMove();
		return;
	}

	// 타겟이 있으면 쫓아가서 공격 사거리 안에 들어오면 공격
	ChaseOrAttackTarget(ownerData->ChaseMaxDistance, ownerData->DefaultAtkRange);
	
	//// 타겟이 있었으나 추격거리를 벗어난 경우 idle로 돌아가기
	//float dist = Utils::DirectionVectorLen(_owner->posInfo, targetPlayer->posInfo);
	//if (dist > ownerData->ChaseMaxDistance)
	//{
	//	ownerMonster->SetTargetObject(nullptr);
	//	ownerMonster->SetState(Protocol::MoveState::MOVE_STATE_IDLE);
	//	BroadcastMove();
	//	return;
	//}
	//
	//// 기본 스킬 사거리 안으로 들어왔으면 skill로 넘어가기
	//if (dist < ownerData->DefaultAtkRange)
	//{
	//	ownerMonster->SetState(Protocol::MoveState::MOVE_STATE_SKILL);
	//	return;
	//}

	// 그 외에는 그냥 적을 따라서 이동하는 상태
	//BroadcastMove();
}

void MonsterAIController::UpdateSkill()
{
	__super::UpdateSkill();

	MonsterRef ownerMonster = static_pointer_cast<Monster>(_owner);
	MonsterDataRef ownerData = ownerMonster->GetMonsterData();
	PlayerRef targetPlayer = static_pointer_cast<Player>(ownerMonster->_targetObject);

	// 이미 스킬 사용중이면 통과
	if (ownerMonster->GetSkillComponent()->GetActiveSkill() != nullptr)
	{
		return;
	}

	// 타겟이 사라지면 idle로 돌아가기
	if (targetPlayer == nullptr || targetPlayer->room.load().lock() == nullptr)
	{
		ownerMonster->SetTargetObject(nullptr);
		ownerMonster->SetState(Protocol::MoveState::MOVE_STATE_IDLE);
		//BroadcastMove();
		return;
	}

	// 아직 스킬 사용을 못하면 idle로 돌아가기
	bool canUseAdvancedSkill = ownerMonster->GetSkillComponent()->GetCanUseSkillBySkillSlot(Protocol::SKILL_SLOD_ADVANCED);
	if (canUseAdvancedSkill == true)
	{
		Protocol::C_SKILL skillPkt;
		{
			Protocol::SimplePosInfo* posPkt = new Protocol::SimplePosInfo();
			posPkt->set_x(targetPlayer->posInfo->x());
			posPkt->set_y(targetPlayer->posInfo->y());
			posPkt->set_z(targetPlayer->posInfo->z());
			skillPkt.set_allocated_simple_pos_info(posPkt);
			Protocol::PosInfo* posInfo = skillPkt.mutable_pos_info();
			posInfo->CopyFrom(*(_owner->posInfo));
			skillPkt.set_slot(Protocol::SKILL_SLOD_ADVANCED);
			skillPkt.set_object_id(ownerMonster->_objectId);
		}

		ownerMonster->GetSkillComponent()->DoSkill(skillPkt);
		return;
	}

	bool canUseDefaultAttack = ownerMonster->GetSkillComponent()->GetCanUseSkillBySkillSlot(Protocol::SKILL_SLOT_ATTACK);
	//canUseDefaultAttack = false; // temp
	if (canUseDefaultAttack == true)
	{
		Protocol::C_SKILL skillPkt;
		{
			Protocol::SimplePosInfo* posPkt = new Protocol::SimplePosInfo();
			posPkt->set_x(targetPlayer->posInfo->x());
			posPkt->set_y(targetPlayer->posInfo->y());
			posPkt->set_z(targetPlayer->posInfo->z());
			skillPkt.set_allocated_simple_pos_info(posPkt);
			skillPkt.set_slot(Protocol::SKILL_SLOT_ATTACK);
			skillPkt.set_object_id(ownerMonster->_objectId);
		}

		ownerMonster->GetSkillComponent()->DoSkill(skillPkt);
		return;
	}

	// 스킬 사용중인 상태니까 끝날때까지 이 상태 유지
}

void MonsterAIController::UpdateDead()
{
	__super::UpdateDead();
}
