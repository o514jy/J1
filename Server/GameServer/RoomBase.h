#pragma once

#include "JobQueue.h"

class RoomBase : public JobQueue
{
public:
	RoomBase();
	virtual ~RoomBase();

public:
	// room�� �������ְ� �ٸ� player���� �˸� �� ���� player�� ����� �����ش�.
	virtual bool EnterRoom(ObjectRef object, bool randPos = true);
	// room���κ��� �����Ų��.
	virtual bool LeaveRoom(ObjectRef object);

public:
	/** setter & getter **/

public: 
	/** network **/
	//EnterRoom() ȣ��
	bool HandleEnterPlayer(PlayerRef player);
	// LeaveRoom() ȣ��
	bool HandleLeavePlayer(PlayerRef player);
	// pkt ���� object�� ��ġ�� �������ְ� �ٸ� player���� �˸���.
	void HandleMove(Protocol::C_MOVE pkt);

	void HandleNotifyPos(Protocol::C_NOTIFY_POS pkt);

	void HandleSkill(Protocol::C_SKILL pkt);

public:
	virtual void UpdateTick();

	RoomBaseRef GetRoomRef();

protected:
	bool AddObject(ObjectRef object);
	bool RemoveObject(uint64 objectId);

public:
	/** use outside **/
	void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);

	PlayerRef FindClosestPlayer(ObjectRef object, uint64 exceptId = 0);

protected:
	// exceptId�� ������ ��� object���� sendBuffer�� ������.
	void Broadcast_internal(SendBufferRef sendBuffer, uint64 exceptId = 0);

public:
	unordered_map<uint64, ObjectRef> _objects;
};