#include "pch.h"
#include "RoomBase.h"
#include "Player.h"
#include "GameSession.h"
#include "Monster.h"
#include "DataManager.h"
#include "ObjectManager.h"
#include "Creature.h"
#include "SkillComponent.h"

RoomBaseRef GRoomBase = make_shared<RoomBase>();

RoomBase::RoomBase()
{

}

RoomBase::~RoomBase()
{

}

bool RoomBase::EnterRoom(ObjectRef object, bool randPos /*= true*/)
{
	bool success = AddObject(object);

	// ���� ��ġ
	if (randPos)
	{
		object->posInfo->set_x(Utils::GetRandom(0.f, 500.f));
		object->posInfo->set_y(Utils::GetRandom(0.f, 500.f));
		object->posInfo->set_z(100.f);
		object->posInfo->set_yaw(Utils::GetRandom(0.f, 100.f));
	}

	// ���� ����� ���� �÷��̾�� �˸���
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.set_success(success);

		Protocol::ObjectInfo* playerInfo = new Protocol::ObjectInfo();
		playerInfo->CopyFrom(*object->objectInfo);
		enterGamePkt.set_allocated_player(playerInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);

		// temp
		cout << "Main Player " << object->objectInfo->object_id() << " is Spawned\n";
	}

	// ���� ����� �ٸ� �÷��̾�� �˸���
	{
		Protocol::S_SPAWN spawnPkt;

		Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
		objectInfo->CopyFrom(*object->objectInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer, object->objectInfo->object_id());
	}

	// ���� ������ �÷��̾� ����� ���� �÷��̾����� �������ش�
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_SPAWN spawnPkt;

		for (auto& item : _objects)
		{
			if (item.second->IsPlayer() == false)
				continue;

			Protocol::ObjectInfo* playerInfo = spawnPkt.add_players();
			playerInfo->CopyFrom(*item.second->objectInfo);
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	return success;
}

bool RoomBase::LeaveRoom(ObjectRef object)
{
	if (object == nullptr)
		return false;

	const uint64 objectId = object->objectInfo->object_id();
	bool success = RemoveObject(objectId);

	// ���� ����� �����ϴ� �÷��̾�� �˸���
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// ���� ����� �˸���
	{
		Protocol::S_DESPAWN despawnPkt;
		despawnPkt.add_object_ids(objectId);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(despawnPkt);
		Broadcast(sendBuffer, objectId);

		if (auto player = dynamic_pointer_cast<Player>(object))
			if (auto session = player->session.lock())
				session->Send(sendBuffer);
	}

	return success;
}

bool RoomBase::HandleEnterPlayer(PlayerRef player)
{
	return EnterRoom(player, true);
}

bool RoomBase::HandleLeavePlayer(PlayerRef player)
{
	return LeaveRoom(player);
}

void RoomBase::HandleMove(Protocol::C_MOVE pkt)
{
	const uint64 objectId = pkt.info().object_id();
	if (_objects.find(objectId) == _objects.end())
		return;

	// ����
	PlayerRef player = dynamic_pointer_cast<Player>(_objects[objectId]);
	player->posInfo->CopyFrom(pkt.info());

	// �̵� ����� �˸��� (���� ����? ����?)
	{
		Protocol::S_MOVE movePkt;
		{
			Protocol::PosInfo* info = movePkt.mutable_info();
			info->CopyFrom(pkt.info());
		}
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
		Broadcast(sendBuffer);
	}
}

void RoomBase::HandleNotifyPos(Protocol::C_NOTIFY_POS pkt)
{
	const uint64 objectId = pkt.info().object_id();
	if (_objects.find(objectId) == _objects.end())
		return;

	// check pos ?
	PlayerRef player = dynamic_pointer_cast<Player>(_objects[objectId]);
	player->posInfo->CopyFrom(pkt.info());
	
	// log
	//cout << player->posInfo->object_id() << "�� ��ǥ : " << "( " << player->posInfo->x() << " " << player->posInfo->y() << " " << player->posInfo->z() << endl;

}

void RoomBase::HandleSkill(Protocol::C_SKILL pkt)
{
	const uint64 objectId = pkt.object_id();
	if (_objects.find(objectId) == _objects.end())
		return;
	Protocol::SkillSlot skillSlot = pkt.slot();

	CreatureRef creature = static_pointer_cast<Creature>(GObjectManager->GetObjectById(objectId));
	
	// ��ų�� �� �� ���� ���¸� ����
	if (creature->_skillComponent->GetCanUseSkillBySkillSlot(skillSlot) == false)
		return;

	// ��ų �ᵵ �ȴܴ�.
	{
		Protocol::S_SKILL skillPkt;
		skillPkt.set_slot(skillSlot);
		skillPkt.set_object_id(objectId);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(skillPkt);
		Broadcast(sendBuffer);
	}

	// Ŭ��� �˾Ƽ� �����ϰ� �ΰ� ������ ���� ����
	creature->_skillComponent->DoSkill(skillSlot);
}

void RoomBase::UpdateTick()
{
	cout << "Update RoomBase" << endl;

	DoTimer(100, &RoomBase::UpdateTick);
}

RoomBaseRef RoomBase::GetRoomRef()
{
	return static_pointer_cast<RoomBase>(shared_from_this());
}

bool RoomBase::AddObject(ObjectRef object)
{
	// �ִٸ� ������ �ִ�.
	if (_objects.find(object->objectInfo->object_id()) != _objects.end())
		return false;

	_objects.insert(make_pair(object->objectInfo->object_id(), object));

	object->room.store(GetRoomRef());

	return true;
}

bool RoomBase::RemoveObject(uint64 objectId)
{
	// ���ٸ� ������ �ִ�.
	if (_objects.find(objectId) == _objects.end())
		return false;

	ObjectRef object = _objects[objectId];
	PlayerRef player = dynamic_pointer_cast<Player>(object);
	if (player)
		player->room.store(weak_ptr<RoomBase>()); // null�� �о��ֱ�

	_objects.erase(objectId);

	return true;
}

void RoomBase::Broadcast(SendBufferRef sendBuffer, uint64 exceptId)
{
	Broadcast_internal(sendBuffer, exceptId);
}

void RoomBase::Broadcast_internal(SendBufferRef sendBuffer, uint64 exceptId)
{
	for (auto& item : _objects)
	{
		PlayerRef player = dynamic_pointer_cast<Player>(item.second);
		if (player == nullptr)
			continue;
		if (player->objectInfo->object_id() == exceptId)
			continue;

		if (GameSessionRef session = player->session.lock())
			session->Send(sendBuffer);
	}
}