#include "pch.h"
#include "RoomBase.h"
#include "Object.h"
#include "StartRoom.h"
#include "Boss.h"
#include "ObjectManager.h"

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

	// all object in start room process update tick 
	for (auto& object : _objects)
	{
		object.second->UpdateTick();
	}

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

void StartRoom::SetRoomState(Protocol::RoomState state)
{
	__super::SetRoomState(state);

	if (state == Protocol::RoomState::ROOM_STATE_BATTLE)
	{
		SpawnBoss();
	}
}

bool StartRoom::EnterRoom(ObjectRef object, bool randPos)
{
	bool success = __super::EnterRoom(object, randPos);

	// temp
	_entered.store(true);
	
	return success;
}

bool StartRoom::LeaveRoom(ObjectRef object)
{
	return false;
}

void StartRoom::SpawnBoss()
{
	////test//////
	{
		Protocol::S_SPAWN sPkt;
		
		for (int i = 0; i < 10; i++)
		{
			BossRef sevas = GObjectManager->CreateBoss(100); // sevarog
			sevas->posInfo->set_x(300.0f);
			sevas->posInfo->set_y(i * Utils::GetRandom(-200.f, 200.f));
			sevas->posInfo->set_z(100.0f);
		
			Protocol::ObjectInfo* objectInfo = sPkt.add_players();
			objectInfo->CopyFrom(*sevas->objectInfo);
		
			bool flag = AddObject(sevas);
			if (flag == true)
				cout << "Boss " << sevas->objectInfo->object_id() << " is Spawned\n";
		}
		
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(sPkt);
		Broadcast(sendBuffer);
		
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
