#include "pch.h"
#include "Monster.h"
#include "Player.h"
#include "DungeonRoom.h"
#include "ObjectManager.h"
#include "SpawningPool.h"

DungeonRoom::DungeonRoom()
{
}

DungeonRoom::~DungeonRoom()
{
}

DungeonRoomRef DungeonRoom::GetRoomRef()
{
	return static_pointer_cast<DungeonRoom>(shared_from_this());
}

void DungeonRoom::UpdateTick()
{
	__super::UpdateTick();

	//cout << "Update StartRoom" << "\n";
	DoTimer(TICK_COUNT, &DungeonRoom::UpdateTick);
}

void DungeonRoom::SetInfo()
{
	_spawningPool = make_shared<SpawningPool>();
	_spawningPool->SetInfo(GetRoomRef());
}

void DungeonRoom::HandleTeleport(Protocol::C_TELEPORT pkt)
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

bool DungeonRoom::EnterRoom(ObjectRef object, bool randPos, FVector3 spawnPos)
{
	bool success = __super::EnterRoom(object, randPos);

	// temp
	// 처음으로 들어온애가 보스 소환 시키기, 나중엔 버튼이나 준비완료 이런거 만들 것
	if (_objects.size() == 1)
	{
		DoTimer(2000, &RoomBase::SetRoomState, Protocol::RoomState::ROOM_STATE_BATTLE);

		// 던전 이동을 위한 포탈 생성

	}

	// temp
	_entered.store(true);

	return success;
}

void DungeonRoom::SetRoomState(Protocol::RoomState state)
{
	if (_roomState != state && state == Protocol::RoomState::ROOM_STATE_BATTLE)
	{
		//SpawnBoss();
		//SpawnMonster();
		_spawningPool->SpawnAndAddMonster();

		cout << "Battle State started!!" << "\n";
	}

	__super::SetRoomState(state);
}

MonsterRef DungeonRoom::GetMonster()
{
	if (_monster != nullptr)
	{
		return _monster;
	}
	FVector3 spawnPos = FVector3(800.f, -400.f, 100.f);
	_monster = GObjectManager->CreateMonster(10, spawnPos, GetRoomRef()); // sevarog

	AddObject(_monster);

	return _monster;
}

vector<MonsterRef> DungeonRoom::GetMonsters()
{
	vector<MonsterRef> v;

	{
		FVector3 spawnPos = FVector3(740.f, -400.f, 100.f);
		MonsterRef mon = GObjectManager->CreateMonster(10, spawnPos, GetRoomRef());
		AddObject(mon);
		v.push_back(mon);
	}

	{
		FVector3 spawnPos = FVector3(800.f, -400.f, 100.f);
		MonsterRef mon = GObjectManager->CreateMonster(10, spawnPos, GetRoomRef());
		AddObject(mon);
		v.push_back(mon);
	}

	{
		FVector3 spawnPos = FVector3(860.f, -400.f, 100.f);
		MonsterRef mon = GObjectManager->CreateMonster(10, spawnPos, GetRoomRef());
		AddObject(mon);
		v.push_back(mon);
	}

	{
		FVector3 spawnPos = FVector3(740.f, -500.f, 100.f);
		MonsterRef mon = GObjectManager->CreateMonster(11, spawnPos, GetRoomRef());
		AddObject(mon);
		v.push_back(mon);
	}

	{
		FVector3 spawnPos = FVector3(800.f, -500.f, 100.f);
		MonsterRef mon = GObjectManager->CreateMonster(11, spawnPos, GetRoomRef());
		AddObject(mon);
		v.push_back(mon);
	}

	{
		FVector3 spawnPos = FVector3(860.f, -500.f, 100.f);
		MonsterRef mon = GObjectManager->CreateMonster(11, spawnPos, GetRoomRef());
		AddObject(mon);
		v.push_back(mon);
	}

	{
		FVector3 spawnPos = FVector3(740.f, -600.f, 100.f);
		MonsterRef mon = GObjectManager->CreateMonster(10, spawnPos, GetRoomRef());
		AddObject(mon);
		v.push_back(mon);
	}

	{
		FVector3 spawnPos = FVector3(800.f, -600.f, 100.f);
		MonsterRef mon = GObjectManager->CreateMonster(10, spawnPos, GetRoomRef());
		AddObject(mon);
		v.push_back(mon);
	}

	{
		FVector3 spawnPos = FVector3(860.f, -600.f, 100.f);
		MonsterRef mon = GObjectManager->CreateMonster(12, spawnPos, GetRoomRef());
		AddObject(mon);
		v.push_back(mon);
	}

	return v;
}

void DungeonRoom::SpawnMonster()
{
	//MonsterRef boss = GetMonster();
	//
	//Protocol::S_SPAWN spawnPkt;
	//{
	//	Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
	//	objectInfo->CopyFrom(*boss->objectInfo);
	//}

	vector<MonsterRef> v = GetMonsters();
	Protocol::S_SPAWN spawnPkt;
	{
		for (int i = 0; i < v.size(); i++)
		{
			Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
			objectInfo->CopyFrom(*v[i]->objectInfo);
		}
	}

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
	Broadcast(sendBuffer);

	//bool flag = AddObject_internal(boss);
	//if (flag == true)
	//	cout << "Boss " << boss->objectInfo->object_id() << " is Spawned\n";

	for (int i = 0; i < v.size(); i++)
	{
		bool flag = AddObject_internal(v[i]);
		if (flag == true)
			cout << "Boss " << v[i]->objectInfo->object_id() << " is Spawned\n";
	}

	
}
