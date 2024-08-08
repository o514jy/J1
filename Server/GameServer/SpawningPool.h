#pragma once

struct SpawnInfo
{
public:
	SpawnInfo(int dataId, float x, float y, float z)
	{
		_dataId = dataId;
		_spawnX = x;
		_spawnY = y;
		_spawnZ = z;
	}

	int _dataId = 0;
	float _spawnX = 0.f;
	float _spawnY = 0.f;
	float _spawnZ = 0.f;
};

class SpawningPool : public enable_shared_from_this<SpawningPool>
{
public:
	SpawningPool();
	~SpawningPool();

public:
	/** initialize **/
	void SetInfo(RoomBaseRef ownerRoom);

public:
	/** setter & getter **/
	void AddKillCount(int32 count);

	void SetFinished(bool flag);

public:
	/** network **/
	void SendSpawningPoolPkt();

public:
	/** process **/
	void SpawnAndAddMonster();

public:
	/** information **/
	int32 _killCount = 0;
	int32 _CurrentPhase = 0;
	int32 _maxCount = 0;
	int32 _maxPhaseCount = 0;

	bool _finished = false; // 상태 기반으로 필요할 경우 교체 예정

	vector<vector<SpawnInfo>> _spawnInfos; // maxPhase == 3

	weak_ptr<RoomBase> _OwnerRoom;
};

