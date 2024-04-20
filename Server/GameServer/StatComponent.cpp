#include "pch.h"
#include "StatComponent.h"
#include "Data.h"
#include "Creature.h"
#include "RoomBase.h"

StatComponent::StatComponent()
{
	_owner = nullptr;
	_statInfo = make_shared<Protocol::StatInfo>();
	_shouldRefresh = false;

	_hp = 0;

	_baseMaxHp = 0;
	_maxHp = 0;

	_baseAtk = 0;
	_atk = 0;

	_baseDef = 0;
	_def = 0;
}

StatComponent::~StatComponent()
{
	_owner = nullptr;
	_statInfo = nullptr;
}

void StatComponent::UpdateTick()
{
	if (_shouldRefresh == true)
		RefreshAll();

	DoTimer(500, &StatComponent::UpdateTick);
}

void StatComponent::SetInfo(CreatureRef owner, CreatureDataRef creatureData, Protocol::CreatureType creatureType)
{
	_owner = owner;

	_owner->objectInfo->set_allocated_stat_info(_statInfo.get());

	/* creature */
	_baseMaxHp = creatureData->MaxHp;
	SetMaxHp(_baseMaxHp);
	SetHp(_baseMaxHp);

	_baseAtk = creatureData->Atk;
	SetAtk(_baseAtk);

	_baseDef = creatureData->Def;
	SetDef(_baseDef);

	// todo : player, monster ...
}

void StatComponent::SetHp(float hp)
{
	_hp = hp;
	_statInfo->set_hp(_hp);
	_shouldRefresh = true;
}

float StatComponent::GetHp()
{
	return _hp;
}

void StatComponent::SetMaxHp(float maxHp)
{
	_maxHp = maxHp;
	_statInfo->set_max_hp(_maxHp);
	_shouldRefresh = true;
}

float StatComponent::GetMaxHp()
{
	return _maxHp;
}

void StatComponent::SetAtk(float atk)
{
	_atk = atk;
	_statInfo->set_atk(_atk);
	_shouldRefresh = true;
}

float StatComponent::GetAtk()
{
	return _atk;
}

void StatComponent::SetDef(float def)
{
	_def = def;
	_statInfo->set_def(_def);
	_shouldRefresh = true;
}

float StatComponent::GetDef()
{
	return _def;
}

void StatComponent::RefreshAll()
{
	// todo : refresh all

	// send stat packet to client
	{
		Protocol::S_STAT statPkt;
		statPkt.set_object_id(_owner->_objectId);
		
		Protocol::StatInfo* statInfo = new Protocol::StatInfo();
		statInfo->CopyFrom(*_statInfo);
		statPkt.set_allocated_stat_info(statInfo);

		RoomBaseRef room = _owner->room.load().lock();

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(statPkt);
		room->Broadcast(sendBuffer);
	}


	_shouldRefresh = false;
}
