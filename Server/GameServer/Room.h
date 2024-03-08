#pragma once
#include "JobQueue.h"

class Room : public JobQueue
{
public:
	Room();
	virtual ~Room();

public:
	// room에 스폰해주고 다른 player에게 알린 뒤 기존 player의 목록을 보내준다.
	bool EnterRoom(ObjectRef object, bool randPos = true);
	// room으로부터 퇴장시킨다.
	bool LeaveRoom(ObjectRef object);

public:
	//EnterRoom() 호출
	bool HandleEnterPlayer(PlayerRef player);
	// LeaveRoom() 호출
	bool HandleLeavePlayer(PlayerRef player);
	// pkt 내의 object의 위치를 조정해주고 다른 player에게 알린다.
	void HandleMove(Protocol::C_MOVE pkt);

public:
	void UpdateTick();

	RoomRef GetRoomRef();

private:
	bool AddObject(ObjectRef object);
	bool RemoveObject(uint64 objectId);

private:
	// exceptId를 제외한 모든 object에게 sendBuffer를 보낸다.
	void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);

private:
	unordered_map<uint64, ObjectRef> _objects;
};

extern RoomRef GRoom;