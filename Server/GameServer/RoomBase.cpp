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
	_entered = false;

	_roomState = Protocol::RoomState::ROOM_STATE_PREPARE;
}

RoomBase::~RoomBase()
{
	
}

void RoomBase::UpdateTick()
{
	//cout << "Update RoomBase" << endl;

	//DoTimer(100, &RoomBase::UpdateTick);

	// 존재하는 object들에게 서있는 곳 알려달라고 보내기
	if (_entered.load() == true)
	{
		Protocol::S_NOTIFY_POS notifyPkt;
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(notifyPkt);
		Broadcast(sendBuffer);
	}
}

bool RoomBase::EnterRoom(ObjectRef object, bool randPos /*= true*/)
{
	bool success = AddObject(object);

	// temp
	// 처음으로 들어온애가 보스 소환 시키기, 나중엔 버튼이나 준비완료 이런거 만들 것
	if (_objects.size() == 1)
	{
		DoTimer(3000, &RoomBase::SetRoomState, Protocol::RoomState::ROOM_STATE_BATTLE);
	}

	// 랜덤 위치
	if (randPos)
	{
		object->posInfo->set_x(Utils::GetRandom(0.f, 100.f));
		object->posInfo->set_y(Utils::GetRandom(0.f, 500.f));
		object->posInfo->set_z(100.f);
		object->posInfo->set_yaw(Utils::GetRandom(0.f, 100.f));
	}

	// 입장 사실을 신입 플레이어에게 알린다
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

	// 입장 사실을 다른 플레이어에게 알린다
	{
		Protocol::S_SPAWN spawnPkt;

		Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
		objectInfo->CopyFrom(*object->objectInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer, object->objectInfo->object_id());
	}

	// 기존 입장한 플레이어 목록을 신입 플레이어한테 전송해준다
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

	// 퇴장 사실을 퇴장하는 플레이어에게 알린다
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 퇴장 사실을 알린다
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

void RoomBase::SetRoomState(Protocol::RoomState state)
{
	_roomState = state;

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

	// 적용
	PlayerRef player = dynamic_pointer_cast<Player>(_objects[objectId]);
	player->posInfo->CopyFrom(pkt.info());

	// 이동 사실을 알린다 (본인 포함? 빼고?)
	{
		Protocol::S_MOVE movePkt;
		{
			Protocol::PosInfo* info = movePkt.mutable_info();
			info->CopyFrom(pkt.info());
			info->set_state(Protocol::MoveState::MOVE_STATE_RUN);
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
	ObjectRef object = _objects[objectId];
	object->SetPosInfo(pkt.info());
	
	// log
	//cout << player->posInfo->object_id() << "의 좌표 : " << "( " << player->posInfo->x() << " " << player->posInfo->y() << " " << player->posInfo->z() << endl;

}

void RoomBase::HandleSkill(Protocol::C_SKILL pkt)
{
	const uint64 objectId = pkt.object_id();
	if (_objects.find(objectId) == _objects.end())
		return;
	Protocol::SkillSlot skillSlot = pkt.slot();

	CreatureRef creature = static_pointer_cast<Creature>(GObjectManager->GetObjectById(objectId));
	
	// 스킬을 쓸 수 없는 상태면 종료
	if (creature->_skillComponent->GetCanUseSkillBySkillSlot(skillSlot) == false)
		return;

	// 이미 스킬을 쓰고 있는 상태면 종료
	if (creature->GetState() == Protocol::MoveState::MOVE_STATE_SKILL)
		return;

	// 스킬 써도 된단다. <--- 이거 그냥 skill component가 하는걸로 바꿈
	//{
	//	Protocol::S_SKILL skillPkt;
	//	skillPkt.set_slot(skillSlot);
	//	skillPkt.set_object_id(objectId);
	//	Protocol::SimplePosInfo* simplePosInfo = skillPkt.mutable_simple_pos_info();
	//	simplePosInfo->CopyFrom(pkt.simple_pos_info());
	//
	//	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(skillPkt);
	//	Broadcast(sendBuffer);
	//}

	// 클라는 알아서 실행하게 두고 서버도 같이 실행
	creature->_skillComponent->DoSkill(pkt);
}

RoomBaseRef RoomBase::GetRoomRef()
{
	return static_pointer_cast<RoomBase>(shared_from_this());
}

bool RoomBase::AddObject(ObjectRef object)
{
	// 있다면 문제가 있다.
	if (_objects.find(object->objectInfo->object_id()) != _objects.end())
		return false;

	_objects.insert(make_pair(object->objectInfo->object_id(), object));

	object->room.store(GetRoomRef());

	return true;
}

bool RoomBase::RemoveObject(uint64 objectId)
{
	// 없다면 문제가 있다.
	if (_objects.find(objectId) == _objects.end())
		return false;

	ObjectRef object = _objects[objectId];
	PlayerRef player = dynamic_pointer_cast<Player>(object);
	if (player)
		player->room.store(weak_ptr<RoomBase>()); // null로 밀어주기

	_objects.erase(objectId);

	return true;
}

void RoomBase::Broadcast(SendBufferRef sendBuffer, uint64 exceptId)
{
	Broadcast_internal(sendBuffer, exceptId);
}

PlayerRef RoomBase::FindClosestPlayer(ObjectRef object, float maxDist, uint64 exceptId)
{
	PlayerRef closestPlayer = nullptr;
	float closestLen = 1000000;

	for (auto& item : _objects)
	{
		PlayerRef player = dynamic_pointer_cast<Player>(item.second);
		if (player == nullptr)
			continue;
		if (player->objectInfo->object_id() == exceptId)
			continue;

		// check distance
		float len = Utils::DirectionVectorLen(object->posInfo, player->posInfo);

		if (len > maxDist)
			continue;

		if (len < closestLen)
		{
			closestPlayer = player;
			closestLen = len;
		}
	}
	
	return closestPlayer;
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