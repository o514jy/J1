#include "pch.h"
#include "RoomBase.h"
#include "Object.h"
#include "StartRoom.h"
#include "Boss.h"
#include "ObjectManager.h"

StartRoom::StartRoom()
{
	
}

StartRoom::~StartRoom()
{
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

bool StartRoom::EnterRoom(ObjectRef object, bool randPos)
{
	bool success = __super::EnterRoom(object, randPos);

	// 보스 몬스터 소환
	{
		Protocol::S_SPAWN spawnPkt;
		
		Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
		BossRef boss = GObjectManager->CreateBoss(100);
		objectInfo->CopyFrom(*boss->objectInfo);
		object->posInfo->set_x(1000.0f);
		object->posInfo->set_y(0.0f);
		object->posInfo->set_x(100.0f);
		
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer);
		
		AddObject(boss);
	}

	// temp
	_entered.store(true);
	
	return success;
}

bool StartRoom::LeaveRoom(ObjectRef object)
{
	return false;
}
