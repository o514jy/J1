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
	// room�� �������ְ� �ٸ� player���� �˸� �� ���� player�� ����� �����ش�.
	virtual bool EnterRoom(ObjectRef object, bool randPos = true, FVector3 spawnPos = FVector3());
	// room���κ��� �����Ų��.
	virtual bool LeaveRoom(ObjectRef object);

	virtual void SendSpawnPktAboutOthers(ObjectRef object);

public:
	/** setter & getter **/
	virtual void SetRoomState(Protocol::RoomState state);
	NavigationRef GetNav() { return _nav; }

public: 
	/** network **/
	//EnterRoom() ȣ��
	bool HandleEnterPlayer(PlayerRef player);
	// LeaveRoom() ȣ��
	bool HandleLeavePlayer(PlayerRef player);
	// pkt ���� object�� ��ġ�� �������ְ� �ٸ� player���� �˸���.
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
	virtual void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);
	bool AddObject(ObjectRef object);
	bool RemoveObject(uint64 objectId);

	PlayerRef FindClosestPlayer(ObjectRef object, float maxDist, uint64 exceptId = 0);
	ObjectRef FindObjectByAgentIdx(int32 agentIdx);

protected:
	// exceptId�� ������ ��� object���� sendBuffer�� ������.
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