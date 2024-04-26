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
	// room에 스폰해주고 다른 player에게 알린 뒤 기존 player의 목록을 보내준다.
	virtual bool EnterRoom(ObjectRef object, bool randPos = true);
	// room으로부터 퇴장시킨다.
	virtual bool LeaveRoom(ObjectRef object);

public:
	/** setter & getter **/

public: 
	/** network **/
	//EnterRoom() 호출
	bool HandleEnterPlayer(PlayerRef player);
	// LeaveRoom() 호출
	bool HandleLeavePlayer(PlayerRef player);
	// pkt 내의 object의 위치를 조정해주고 다른 player에게 알린다.
	void HandleMove(Protocol::C_MOVE pkt);

	void HandleNotifyPos(Protocol::C_NOTIFY_POS pkt);

	void HandleSkill(Protocol::C_SKILL pkt);

public:

	RoomBaseRef GetRoomRef();

protected:
	bool AddObject(ObjectRef object);
	bool RemoveObject(uint64 objectId);

public:
	/** use outside **/
	void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);

	PlayerRef FindClosestPlayer(ObjectRef object, float maxDist, uint64 exceptId = 0);

protected:
	// exceptId를 제외한 모든 object에게 sendBuffer를 보낸다.
	void Broadcast_internal(SendBufferRef sendBuffer, uint64 exceptId = 0);

public:
	unordered_map<uint64, ObjectRef> _objects;

protected:
	atomic<bool> _entered;
};