#include "pch.h"
#include "BuffInstant.h"
#include "Object.h"
#include "Creature.h"
#include "SkillBase.h"
#include "RoomBase.h"

BuffInstant::BuffInstant()
{
}

BuffInstant::~BuffInstant()
{
}

void BuffInstant::ApplyBuff()
{
	if (_owner == nullptr)
		return;

	_owner->OnDamaged(static_pointer_cast<Object>(_ownerSkill->_owner), static_pointer_cast<BuffBase>(shared_from_this()));

	{
		Protocol::S_BUFF buffPkt;
		buffPkt.set_object_id(_owner->_objectId);

		Protocol::BuffInfo* buffInfo = new Protocol::BuffInfo();
		buffInfo->CopyFrom(*_buffInfo);
		buffPkt.set_allocated_buff_info(buffInfo);

		RoomBaseRef room = _owner->room.load().lock();

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(buffPkt);
		room->Broadcast(sendBuffer);
	}
}