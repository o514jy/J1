#pragma once

#include "JobQueue.h"

class EmptyRoom;

class RoomBase : public JobQueue
{
public:
	RoomBase();
	virtual ~RoomBase();

	void init(string objFilePath);

public:
	/** update tick**/
	virtual void UpdateTick();

public:
	// room에 스폰해주고 다른 player에게 알린 뒤 기존 player의 목록을 보내준다.
	virtual bool EnterRoom(ObjectRef object, bool randPos = true, FVector3 spawnPos = FVector3());
	// room으로부터 퇴장시킨다.
	virtual bool LeaveRoom(ObjectRef object);

	virtual void SendSpawnPktAboutOthers(ObjectRef object);

public:
	/** setter & getter **/
	virtual void SetRoomState(Protocol::RoomState state);
	NavigationRef GetNav() { return _nav; }

public: 
	/** network **/
	//EnterRoom() 호출
	bool HandleEnterPlayer(PlayerRef player);
	// LeaveRoom() 호출
	bool HandleLeavePlayer(PlayerRef player);
	// pkt 내의 object의 위치를 조정해주고 다른 player에게 알린다.
	void HandleMove(Protocol::C_MOVE pkt);

	virtual void HandleTeleport(Protocol::C_TELEPORT pkt);
	virtual void HandleTeleportFin(Protocol::C_TELEPORT_FIN pkt);

	void HandleNotifyPos(Protocol::C_NOTIFY_POS pkt);

	void HandleSkill(Protocol::C_SKILL pkt);

	void HandleEquipItem(Protocol::C_EQUIP_ITEM pkt, PlayerRef player);
	void HandleUnequipItem(Protocol::C_UNEQUIP_ITEM pkt, PlayerRef player);

public:
	/** get **/
	RoomBaseRef GetRoomRef();

public:
	/** use outside **/
	template<typename T>
	void Broadcast(T& pkt, uint64 exceptId = 0)
	{
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		Broadcast(sendBuffer, exceptId);
	}

	virtual void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);
	bool AddObject(ObjectRef object);
	bool RemoveObject(uint64 objectId);

	PlayerRef FindClosestPlayer(ObjectRef object, float maxDist, uint64 exceptId = 0);
	ObjectRef FindObjectByAgentIdx(int32 agentIdx);

protected:
	// exceptId를 제외한 모든 object에게 sendBuffer를 보낸다.
	void Broadcast_internal(SendBufferRef sendBuffer, uint64 exceptId = 0);
	bool AddObject_internal(ObjectRef object);
	bool RemoveObject_internal(uint64 objectId);

public:
	/** information **/
	unordered_map<uint64, ObjectRef> _objects;
	unordered_map<int32, ObjectRef> _AgentIdxToObject;

	// Nav
	NavigationRef _nav;

	Protocol::RoomType _roomType;

	string roomName;

protected:
	atomic<bool> _entered;

	Protocol::RoomState _roomState;
};