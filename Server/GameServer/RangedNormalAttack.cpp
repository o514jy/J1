#include "pch.h"
#include "Creature.h"
#include "Projectile.h"
#include "RangedNormalAttack.h"
#include "RoomBase.h"
#include "ObjectManager.h"

RangedNormalAttack::RangedNormalAttack()
{
	_projectile = nullptr;
}

RangedNormalAttack::~RangedNormalAttack()
{
}

void RangedNormalAttack::SetInfo(CreatureRef owner, int32 templateId)
{
	__super::SetInfo(owner, templateId);
}

void RangedNormalAttack::OnAttackEvent(int32 timeCount)
{
	__super::OnAttackEvent(timeCount);

	cout << _owner->_objectId << "'s RangedNormalAttack Skill OnAttackEvent " << timeCount << "\n";

	_projectile->SpawnProjectile();

	// send packet Protocol::PosInfo* info = movePkt.mutable_info();
}

void RangedNormalAttack::DoSkill(const Protocol::C_SKILL& skillPkt, Protocol::S_SKILL& skillPktToSend)
{
	__super::DoSkill(skillPkt, skillPktToSend);

	_destPos.X = skillPkt.simple_pos_info().x();
	_destPos.Y = skillPkt.simple_pos_info().y();
	_destPos.Z = skillPkt.simple_pos_info().z();

	// projectile »ý¼º
	_projectile = GObjectManager->CreateProjectile(
		_skillData->ProjectileIdList[0],
		_owner,
		shared_from_this(),
		nullptr,
		_owner->GetPosInfo()->x(),
		_owner->GetPosInfo()->y(),
		_owner->GetPosInfo()->z(),
		_destPos
	);

	{
		Protocol::ObjectInfo* projInfo = skillPktToSend.mutable_projectile_object_info();
		projInfo->CopyFrom(*_projectile->objectInfo);
		int a = 3;
	}
}
