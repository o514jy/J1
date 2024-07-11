#include "pch.h"
#include "RoomBase.h"
#include "Player.h"
#include "GameSession.h"
#include "Monster.h"
#include "RoomManager.h"
#include "DataManager.h"
#include "ObjectManager.h"
#include "Creature.h"
#include "SkillComponent.h"
// recast-detour
#include "InputGeom.h"
#include "SampleInterfaces.h"
#include "Navigation.h"

RoomBaseRef GRoomBase = make_shared<RoomBase>();

RoomBase::RoomBase()
{
	_entered = false;

	_roomState = Protocol::RoomState::ROOM_STATE_PREPARE;
	_roomType = Protocol::RoomType::ROOOM_TYPE_NONE;
}

RoomBase::~RoomBase()
{
	
}

void RoomBase::init(string objFilePath)
{
	BuildContext* ctx = new BuildContext();
	_nav = make_shared<Navigation>(GetRoomRef());
	/** build nav mesh **/
	{
		InputGeom* geom = new InputGeom();

		// geom 설정
		_nav->handleMeshChanged(geom);
		// ctx 설정
		_nav->setContext(ctx);

		if (geom->load(ctx, objFilePath) == false)
			cout << "[ " << roomName << " ] can't load file" << "\n";
		else
			cout << "[ " << roomName << " ] load completed!!" << "\n";

		_nav->handleBuild();
	}
	//////////////////////
}

void RoomBase::UpdateTick()
{
	// all object in start room process update tick 
	for (auto& object : _objects)
	{
		object.second->UpdateTick();
	}

	// update navigation
	if (_nav != nullptr)
	{
		_nav->UpdateTick();
	}
}

bool RoomBase::EnterRoom(ObjectRef object, bool randPos /*= true*/, FVector3 spawnPos)
{
	bool success = AddObject_internal(object);

	// 랜덤 위치
	if (randPos)
	{
		object->posInfo->set_x(Utils::GetRandom(0.f, 300.f));
		object->posInfo->set_y(Utils::GetRandom(-300.f, 300.f));
		object->posInfo->set_z(100.f);
		object->posInfo->set_dest_x(object->posInfo->x());
		object->posInfo->set_dest_y(object->posInfo->y());
		object->posInfo->set_dest_z(object->posInfo->z());
		object->posInfo->set_yaw(Utils::GetRandom(0.f, 100.f));
		object->posInfo->set_state(Protocol::MoveState::MOVE_STATE_IDLE);
	}
	else
	{
		object->posInfo->set_x(spawnPos.X);
		object->posInfo->set_y(spawnPos.Y);
		object->posInfo->set_z(100.f);
		object->posInfo->set_dest_x(object->posInfo->x());
		object->posInfo->set_dest_y(object->posInfo->y());
		object->posInfo->set_dest_z(object->posInfo->z());
		object->posInfo->set_yaw(Utils::GetRandom(0.f, 100.f));
		object->posInfo->set_state(Protocol::MoveState::MOVE_STATE_IDLE);
	}

	// test for stress
	//if (randPos)
	//{
	//	//object->posInfo->set_x(Utils::GetRandom(-500.f, 300.f));
	//	//object->posInfo->set_y(Utils::GetRandom(-500.f, 700.f));
	//	//object->posInfo->set_x(Utils::GetRandom(-500.f, -480.f));
	//	//object->posInfo->set_y(Utils::GetRandom(-200.f, -190.f));
	//	object->posInfo->set_z(100.f);
	//	object->posInfo->set_dest_x(object->posInfo->x());
	//	object->posInfo->set_dest_y(object->posInfo->y());
	//	object->posInfo->set_dest_z(object->posInfo->z());
	//	object->posInfo->set_yaw(Utils::GetRandom(0.f, 100.f));
	//	object->posInfo->set_state(Protocol::MoveState::MOVE_STATE_IDLE);
	//}

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
	//if (auto player = dynamic_pointer_cast<Player>(object))
	//{
	//	Protocol::S_SPAWN spawnPkt;
	//
	//	for (auto& item : _objects)
	//	{
	//		if (item.second->IsPlayer() == false)
	//			continue;
	//
	//		Protocol::ObjectInfo* playerInfo = spawnPkt.add_players();
	//		playerInfo->CopyFrom(*item.second->objectInfo);
	//	}
	//
	//	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
	//	if (auto session = player->session.lock())
	//		session->Send(sendBuffer);
	//}

	// 기존에 있던 오브젝트 목록을 신입 플레이어한테 전송해준다
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_SPAWN spawnPkt;

		for (auto& item : _objects)
		{
			Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
			objectInfo->CopyFrom(*item.second->objectInfo);
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

		//if (auto player = dynamic_pointer_cast<Player>(object))
		//	if (auto session = player->session.lock())
		//		session->Send(sendBuffer);
	}

	//bool success = GObjectManager->RemoveObject(objectId);

	return success;
}

void RoomBase::SendSpawnPktAboutOthers(ObjectRef object)
{
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_SPAWN spawnPkt;

		for (auto& item : _objects)
		{
			if (item.second->IsPlayer() == false)
				continue;

			if (item.second->_objectId == object->_objectId)
				continue;

			Protocol::ObjectInfo* playerInfo = spawnPkt.add_players();
			playerInfo->CopyFrom(*item.second->objectInfo);
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}
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

	// 이동 사실을 알린다 (본인 빼고)
	{
		Protocol::S_MOVE movePkt;
		{
			Protocol::PosInfo* info = movePkt.mutable_info();
			info->CopyFrom(pkt.info());
		}
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
		Broadcast(sendBuffer, objectId);
	}
}

void RoomBase::HandleTeleport(Protocol::C_TELEPORT pkt)
{
	
}

void RoomBase::HandleTeleportFin(Protocol::C_TELEPORT_FIN pkt)
{
	cout << "TeleportFin Pkt is dilivered!!" << "\n";

	if (pkt.dest_room_type() == Protocol::RoomType::ROOOM_TYPE_NONE)
		return;

	RoomBaseRef destRoom = GRoomManager->GetRoom(pkt.dest_room_type());

	if (_objects.find(pkt.info().object_id()) == _objects.end())
		return;

	ObjectRef object = _objects[pkt.info().object_id()];

	if (object == nullptr)
		return;

	cout << "Player : " << object->_objectId << "'s Changing Room is registered!!" << "\n";

	GRoomManager->DoAsync(&RoomManager::ChangeRoom, object, destRoom);
}

void RoomBase::HandleNotifyPos(Protocol::C_NOTIFY_POS pkt)
{
	const uint64 objectId = pkt.info().object_id();
	if (_objects.find(objectId) == _objects.end())
		return;

	// check pos ?
	ObjectRef object = _objects[objectId];
	object->SetPosInfo(pkt.info());

	// 위치 알려주고 보정이 필요한 경우 클라에서 진행
	{
		Protocol::S_NOTIFY_POS notifyPosPkt;
		{
			notifyPosPkt.set_object_id(objectId);
			Protocol::PosInfo* posInfo = notifyPosPkt.mutable_info();
			posInfo->CopyFrom(pkt.info());
		}
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(notifyPosPkt);
		Broadcast(sendBuffer, objectId);
	}
	
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

bool RoomBase::AddObject_internal(ObjectRef object)
{
	// 있다면 문제가 있다.
	if (_objects.find(object->objectInfo->object_id()) != _objects.end())
		return false;

	cout << "object " << object->objectInfo->object_id() << " is added to Room " << _roomType << "\n";

	_objects.insert(make_pair(object->objectInfo->object_id(), object));

	// temp
	{
		// device의 통제를 받으면 추가
		if (MonsterRef monster = dynamic_pointer_cast<Monster>(object))
		{
			_AgentIdxToObject.insert({ monster->GetAgentIdx(), object });
		}
	}

	object->room.store(GetRoomRef());

	return true;
}

bool RoomBase::RemoveObject_internal(uint64 objectId)
{
	// 없다면 문제가 있다.
	if (_objects.empty() == true)
		return false;

	if (_objects.find(objectId) == _objects.end())
		return false;

	ObjectRef delObject = _objects[objectId];

	cout << "object " << objectId << " is removed to Room " << _roomType << "\n";

	_objects.erase(objectId);

	// temp
	{
		// device의 통제를 받았으면 삭제
		if (MonsterRef monster = dynamic_pointer_cast<Monster>(delObject))
		{
			_AgentIdxToObject.erase(monster->GetAgentIdx());
		}
	}

	//GObjectManager->RemoveObject(objectId);

	return true;
}

void RoomBase::Broadcast(SendBufferRef sendBuffer, uint64 exceptId)
{
	Broadcast_internal(sendBuffer, exceptId);
}

bool RoomBase::AddObject(ObjectRef object)
{
	return AddObject_internal(object);
}

bool RoomBase::RemoveObject(uint64 objectId)
{
	return RemoveObject_internal(objectId);
}

PlayerRef RoomBase::FindClosestPlayer(ObjectRef object, float maxDist, uint64 exceptId)
{
	PlayerRef closestPlayer = nullptr;
	float closestLen = 1000000;

	for (auto& item : _objects)
	{
		if (item.second == nullptr)
			continue;

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

ObjectRef RoomBase::FindObjectByAgentIdx(int32 agentIdx)
{
	if (_AgentIdxToObject.find(agentIdx) == _AgentIdxToObject.end())
		return nullptr;

	return _AgentIdxToObject[agentIdx];
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