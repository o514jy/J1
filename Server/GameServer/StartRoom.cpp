#include "pch.h"
#include "RoomBase.h"
#include "Object.h"
#include "StartRoom.h"
#include "Boss.h"
#include "Player.h"
#include "Portal.h"
#include "DungeonRoom.h"
#include "ObjectManager.h"
#include "RoomManager.h"

StartRoom::StartRoom()
{
	_sevarog = nullptr;
}

StartRoom::~StartRoom()
{
	_sevarog = nullptr;
}

StartRoomRef StartRoom::GetRoomRef()
{
	return static_pointer_cast<StartRoom>(shared_from_this());
}

void StartRoom::UpdateTick()
{
	__super::UpdateTick();

	//cout << "Update StartRoom" << "\n";
	DoTimer(TICK_COUNT, &StartRoom::UpdateTick);
}

BossRef StartRoom::GetSevarog()
{
	if (_sevarog != nullptr)
	{
		return _sevarog;
	}

	_sevarog = GObjectManager->CreateBoss(100); // sevarog
	_sevarog->posInfo->set_x(1000.0f);
	_sevarog->posInfo->set_y(0.0f);
	_sevarog->posInfo->set_z(100.0f);

	return _sevarog;
}

PortalRef StartRoom::GetPortal()
{
	if (_portal != nullptr)
	{
		return _portal;
	}

	_portal = GObjectManager->CreatePortal(0, GRoomManager->GetRoom(Protocol::RoomType::ROOM_TYPE_DUNGEON_ROOM));
	_portal->posInfo->set_x(550.f);
	_portal->posInfo->set_y(0.f);
	_portal->posInfo->set_z(100.f);

	return _portal;
}

void StartRoom::SetRoomState(Protocol::RoomState state)
{
	__super::SetRoomState(state);

	if (state == Protocol::RoomState::ROOM_STATE_BATTLE)
	{
		//SpawnBoss();
		SpawnPortal();
		//SpawnRanged();
		//SpawnDemon();
	}
}

void StartRoom::HandleTeleport(Protocol::C_TELEPORT pkt)
{
	__super::HandleTeleport(pkt);

	const uint64 objectId = pkt.info().object_id();
	if (_objects.find(objectId) == _objects.end())
		return;

	PlayerRef player = dynamic_pointer_cast<Player>(_objects[objectId]);
	if (player == nullptr)
		return;

	// 검증
	if (player->GetRoomRef()->_roomType != pkt.start_room_type())
		return;

	cout << "player " << objectId << "'s teleport request is accepted!!" << "\n";

	if (120.f >= Utils::DirectionVectorLen(player->GetPosInfo(), GetPortal()->GetPosInfo()))
	{
		//RoomBaseRef destRoom = GRoomManager->GetRoom(Protocol::RoomType::ROOM_TYPE_DUNGEON_ROOM);
		//GRoomManager->DoAsync(&RoomManager::ChangeRoom, _objects[objectId], destRoom);

		// process teleport
		{
			Protocol::S_TELEPORT teleportPkt;
			Protocol::ObjectInfo* info = teleportPkt.mutable_info();
			info->CopyFrom(*player->objectInfo);
			teleportPkt.set_start_room_type(pkt.start_room_type());
			teleportPkt.set_dest_room_type(pkt.dest_room_type());

			SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(teleportPkt);
			GameSessionRef session = player->session.lock();
			if (session != nullptr)
				session->Send(sendBuffer);
		}
	}
}

bool StartRoom::EnterRoom(ObjectRef object, bool randPos, FVector3 spawnPos)
{
	bool success = __super::EnterRoom(object, randPos);

	// temp
	// 처음으로 들어온애가 보스 소환 시키기, 나중엔 버튼이나 준비완료 이런거 만들 것
	if (_objects.size() == 1)
	{
		DoTimer(3000, &RoomBase::SetRoomState, Protocol::RoomState::ROOM_STATE_BATTLE);

		// 던전 이동을 위한 포탈 생성

	}

	// temp
	_entered.store(true);
	
	return success;
}

bool StartRoom::LeaveRoom(ObjectRef object)
{
	return __super::LeaveRoom(object);
}

void StartRoom::SpawnBoss()
{
	////test//////
	{
		//Protocol::S_SPAWN sPkt;
		//
		//for (int i = 0; i < 10; i++)
		//{
		//	BossRef sevas = GObjectManager->CreateBoss(100); // sevarog
		//	sevas->posInfo->set_x(300.0f);
		//	sevas->posInfo->set_y(i * Utils::GetRandom(-200.f, 200.f));
		//	sevas->posInfo->set_z(100.0f);
		//
		//	Protocol::ObjectInfo* objectInfo = sPkt.add_players();
		//	objectInfo->CopyFrom(*sevas->objectInfo);
		//
		//	bool flag = AddObject(sevas);
		//	if (flag == true)
		//		cout << "Boss " << sevas->objectInfo->object_id() << " is Spawned\n";
		//}
		//
		//SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(sPkt);
		//Broadcast(sendBuffer);
		
	}
	////////////////

	BossRef boss = GetSevarog();
	
	Protocol::S_SPAWN spawnPkt;
	{
		Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
		objectInfo->CopyFrom(*boss->objectInfo);
	}
	
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
	Broadcast(sendBuffer);
	
	bool flag = AddObject_internal(boss);
	
	if (flag == true)
		cout << "Boss " << boss->objectInfo->object_id() << " is Spawned\n";
}

void StartRoom::SpawnPortal()
{
	PortalRef portal = GetPortal();

	Protocol::S_SPAWN spawnPkt;
	{
		Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
		objectInfo->CopyFrom(*portal->objectInfo);
	}

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
	Broadcast(sendBuffer);

	bool flag = AddObject_internal(portal);

	if (flag == true)
		cout << "portal " << portal->objectInfo->object_id() << " is Spawned\n";
}

MonsterRef StartRoom::GetRanged()
{
	if (_monster != nullptr)
	{
		return _monster;
	}

	FVector3 spawnPos = FVector3(550.f, 300.f, 100.f);
	_monster = GObjectManager->CreateMonster(11, spawnPos, GetRoomRef());

	return _monster;
}

void StartRoom::SpawnRanged()
{
	MonsterRef ranged = GetRanged();

	Protocol::S_SPAWN spawnPkt;
	{
		Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
		objectInfo->CopyFrom(*ranged->objectInfo);
	}

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
	Broadcast(sendBuffer);

	bool flag = AddObject_internal(ranged);

	if (flag == true)
		cout << "ranged " << ranged->objectInfo->object_id() << " is Spawned\n";
}

MonsterRef StartRoom::GetDemon()
{
	if (_monster != nullptr)
	{
		return _monster;
	}

	FVector3 spawnPos = FVector3(550.f, 300.f, 100.f);
	_monster = GObjectManager->CreateMonster(12, spawnPos, GetRoomRef());

	return _monster;
}

void StartRoom::SpawnDemon()
{
	MonsterRef ranged = GetDemon();

	Protocol::S_SPAWN spawnPkt;
	{
		Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
		objectInfo->CopyFrom(*ranged->objectInfo);
	}

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
	Broadcast(sendBuffer);

	bool flag = AddObject_internal(ranged);

	if (flag == true)
		cout << "demon " << ranged->objectInfo->object_id() << " is Spawned\n";
}
