#include "pch.h"
#include "BuffComponent.h"
#include "BuffBase.h"
#include "DataManager.h"
#include "Data.h"
#include "Creature.h"
#include "RoomBase.h"

atomic<uint64> BuffComponent::_buffIdGenerator = 1;

unordered_map<Protocol::BuffType, BuffPolicyRef> BuffComponent::_policies =
{
	{Protocol::BuffType::BUFF_TYPE_HIT, make_shared<HitBuffPolicy>()},
	{Protocol::BuffType::BUFF_TYPE_HEAL, make_shared<DummyBuffPolicy>()},
	{Protocol::BuffType::BUFF_TYPE_BUFF_STAT, make_shared<DummyBuffPolicy>()}
};

BuffComponent::BuffComponent()
{
	_owner = nullptr;
}

BuffComponent::~BuffComponent()
{
	_owner = nullptr;
	ActiveBuffs.clear(); // legacy
	_buffs.clear();
}

uint64 BuffComponent::GenerateBuffId()
{
	return _buffIdGenerator++;
}

BuffBaseRef BuffComponent::SpawnBuff(uint64 buffId, int32 templateId, CreatureRef target, CreatureRef caster, BuffPolicyRef policy)
{
	BuffBaseRef buff = make_shared<BuffBase>();
	buff->SetInfo(buffId, templateId, target, caster, policy);

	return buff;
}

void BuffComponent::SetInfo(CreatureRef owner)
{
	_owner = owner;
}

void BuffComponent::Clear()
{

}

vector<BuffBaseRef> BuffComponent::GenerateBuffs(vector<int32> buffIds, SkillBaseRef skill)
{
	vector<BuffBaseRef> generatedBuffs;

	for (int32 buffId : buffIds)
	{

	}

	return generatedBuffs;
}

void BuffComponent::ApplyBuff(int32 templateId, CreatureRef caster)
{
	BuffDataRef buffData = GDataManager->GetBuffDataById(templateId);
	if (buffData == nullptr)
		return;

	ApplyBuff(buffData, caster);
}

void BuffComponent::ApplyBuff(BuffDataRef buffData, CreatureRef caster, bool send)
{
	if (buffData == nullptr)
		return;

	switch (buffData->BuffDurationType)
	{
	case Protocol::BuffDurationType::BUFF_DURATION_TYPE_INSTANT:
		ApplyInstantBuff(buffData, caster);
	case Protocol::BuffDurationType::BUFF_DURATION_TYPE_DURATION:
		ApplyDurationBuff(buffData, caster, send);
	case Protocol::BuffDurationType::BUFF_DURATION_TYPE_INFINITE:
		ApplyInfiniteBuff(buffData, caster, send);
	}
}

void BuffComponent::ApplyInstantBuff(BuffDataRef buffData, CreatureRef caster)
{
	if (buffData == nullptr)
		return;

	if (_policies.find(buffData->BuffType) == _policies.end())
		return;

	BuffPolicyRef policy = _policies[buffData->BuffType];

	// 1. 버프 적용
	policy->ApplyBuff(caster, _owner, buffData);
}

void BuffComponent::ApplyDurationBuff(BuffDataRef buffData, CreatureRef caster, bool send)
{
	if (buffData == nullptr)
		return;

	if (buffData->BuffDurationMagnitude == 0)
		return;

	if (_policies.find(buffData->BuffType) == _policies.end())
		return;

	BuffPolicyRef policy = _policies[buffData->BuffType];

	// 1. 버프 생성
	uint64 buffId = GenerateBuffId();
	BuffBaseRef buff = SpawnBuff(buffId, buffData->DataId, _owner, caster, policy);
	_buffs.insert({ buffId, buff });

	// 2. 버프 적용
	buff->ApplyBuff();

	// 3. 주변 사람에게 알림
	if (send == true)
		SendApply(buff);

	// 4. 버프 해제 예약
	_owner->GetRoomRef()->DoTimer(buffData->BuffDurationMagnitude, shared_from_this(), &BuffComponent::RemoveBuff, buff, send);
}

void BuffComponent::ApplyInfiniteBuff(BuffDataRef buffData, CreatureRef caster, bool send)
{
	if (buffData == nullptr)
		return;

	if (_policies.find(buffData->BuffType) == _policies.end())
		return;

	BuffPolicyRef policy = _policies[buffData->BuffType];

	// 1. 버프 생성
	uint64 buffId = GenerateBuffId();
	BuffBaseRef buff = SpawnBuff(buffId, buffData->DataId, _owner, caster, policy);
	_buffs.insert({ buffId, buff });

	// 2. 버프 적용
	buff->ApplyBuff();

	// 3. 주변 사람에게 알림
	if (send == true)
		SendApply(buff);
}

void BuffComponent::RemoveBuff(uint64 buffId, bool send)
{
	if (_buffs.find(buffId) == _buffs.end())
		return;

	RemoveBuff(_buffs[buffId], send);
}

void BuffComponent::RemoveBuff(BuffBaseRef buff, bool send)
{
	_buffs.erase(buff->_buffId);

	buff->RevertBuff();

	if (send == true)
		SendRemove(buff);
}

void BuffComponent::RemoveAllBuff()
{
	for (auto item : _buffs)
	{
		BuffBaseRef buff = item.second;
		RemoveBuff(buff, false);
	}

	_buffs.clear();
}

void BuffComponent::SendApply(BuffBaseRef buff)
{
	Protocol::S_APPLY_BUFF pkt;
	{
		pkt.set_object_id(_owner->_objectId);
		pkt.set_buff_template_id(buff->_templateId);
		pkt.set_buff_id(buff->_buffId);
		pkt.set_remaining_ticks(buff->GetRemainingLifetimeInTicks());
		pkt.set_state_flag(_owner->objectInfo->state_flag());
	}

	RoomBaseRef room = _owner->GetRoomRef();
	if (room == nullptr || room == GEmptyRoom)
		return;

	room->Broadcast(pkt);
	//SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	//room->Broadcast(sendBuffer);
}

void BuffComponent::SendRemove(BuffBaseRef buff)
{
	Protocol::S_REMOVE_BUFF pkt;
	{
		pkt.set_object_id(_owner->_objectId);
		pkt.set_buff_id(buff->_buffId);
		pkt.set_state_flag(_owner->objectInfo->state_flag());
	}

	RoomBaseRef room = _owner->GetRoomRef();
	if (room == nullptr || room == GEmptyRoom)
		return;

	room->Broadcast(pkt);
	//SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	//room->Broadcast(sendBuffer);
}
