#pragma once
#include "JobQueue.h"

class Room : public JobQueue
{
public:
	Room();
	virtual ~Room();

public:
	// room�� �������ְ� �ٸ� player���� �˸� �� ���� player�� ����� �����ش�.
	bool EnterRoom(ObjectRef object, bool randPos = true);
	// room���κ��� �����Ų��.
	bool LeaveRoom(ObjectRef object);

public:
	//EnterRoom() ȣ��
	bool HandleEnterPlayer(PlayerRef player);
	// LeaveRoom() ȣ��
	bool HandleLeavePlayer(PlayerRef player);
	// pkt ���� object�� ��ġ�� �������ְ� �ٸ� player���� �˸���.
	void HandleMove(Protocol::C_MOVE pkt);

public:
	void UpdateTick();

	RoomRef GetRoomRef();

private:
	bool AddObject(ObjectRef object);
	bool RemoveObject(uint64 objectId);

private:
	// exceptId�� ������ ��� object���� sendBuffer�� ������.
	void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);

private:
	unordered_map<uint64, ObjectRef> _objects;
};

extern RoomRef GRoom;