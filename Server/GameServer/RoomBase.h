#pragma once

#include "JobQueue.h"

class RoomBase : public JobQueue
{
public:
	RoomBase();
	virtual ~RoomBase();

public:
	/** update tick**/
	virtual void UpdateTick();

public:
	// room�� �������ְ� �ٸ� player���� �˸� �� ���� player�� ����� �����ش�.
	virtual bool EnterRoom(ObjectRef object, bool randPos = true);
	// room���κ��� �����Ų��.
	virtual bool LeaveRoom(ObjectRef object);

public:
	/** setter & getter **/
	virtual void SetRoomState(Protocol::RoomState state);

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
	/** get **/
	RoomBaseRef GetRoomRef();

public:
	/** use outside **/
	void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);
	bool AddObject(ObjectRef object);
	bool RemoveObject(uint64 objectId);

	PlayerRef FindClosestPlayer(ObjectRef object, float maxDist, uint64 exceptId = 0);

protected:
	// exceptId�� ������ ��� object���� sendBuffer�� ������.
	void Broadcast_internal(SendBufferRef sendBuffer, uint64 exceptId = 0);
	bool AddObject_internal(ObjectRef object);
	bool RemoveObject_internal(uint64 objectId);

public:
	unordered_map<uint64, ObjectRef> _objects;
	unordered_map<int32, ObjectRef> _AgentIdxToObject;

protected:
	atomic<bool> _entered;

	Protocol::RoomState _roomState;
};