#include "pch.h"
#include "SpawningPool.h"
#include "ObjectManager.h"
#include "RoomBase.h"
#include "Monster.h"
#include "Boss.h"

SpawningPool::SpawningPool()
{
}

SpawningPool::~SpawningPool()
{
}

void SpawningPool::SetInfo(RoomBaseRef ownerRoom)
{
	_OwnerRoom = ownerRoom;

	// 추후 데이터 시트와 연계해서 기록

	// 툴로 소환 지역을 찍을 수 있으면 좋을듯

	_maxPhaseCount = 4;
	_spawnInfos.assign(_maxPhaseCount, vector<SpawnInfo>());

	// phase 0
	_spawnInfos[0].push_back(SpawnInfo(Utils::GetRandom(10, 12), 700.f, -400.f, 100.f)); // 1
	_spawnInfos[0].push_back(SpawnInfo(Utils::GetRandom(10, 12), 800.f, -400.f, 100.f)); // 2
	_spawnInfos[0].push_back(SpawnInfo(Utils::GetRandom(10, 12), 900.f, -400.f, 100.f)); // 3
	_spawnInfos[0].push_back(SpawnInfo(Utils::GetRandom(10, 12), 1000.f, -400.f, 100.f)); // 4
	_spawnInfos[0].push_back(SpawnInfo(Utils::GetRandom(10, 12), 700.f, -500.f, 100.f)); // 5
	_spawnInfos[0].push_back(SpawnInfo(Utils::GetRandom(10, 12), 800.f, -500.f, 100.f)); // 6
	_spawnInfos[0].push_back(SpawnInfo(Utils::GetRandom(10, 12), 900.f, -500.f, 100.f)); // 7
	_spawnInfos[0].push_back(SpawnInfo(Utils::GetRandom(10, 12), 1000.f, -500.f, 100.f)); // 8
	_spawnInfos[0].push_back(SpawnInfo(Utils::GetRandom(10, 12), 700.f, -600.f, 100.f)); // 9
	_spawnInfos[0].push_back(SpawnInfo(Utils::GetRandom(10, 12), 800.f, -600.f, 100.f)); // 10
	_spawnInfos[0].push_back(SpawnInfo(Utils::GetRandom(10, 12), 900.f, -600.f, 100.f)); // 11
	_spawnInfos[0].push_back(SpawnInfo(Utils::GetRandom(10, 12), 1000.f, -600.f, 100.f)); // 12

	// phase 1
	_spawnInfos[1].push_back(SpawnInfo(Utils::GetRandom(10, 12), 1800.f, -400.f, 100.f)); // 13
	_spawnInfos[1].push_back(SpawnInfo(Utils::GetRandom(10, 12), 1900.f, -400.f, 100.f)); // 14
	_spawnInfos[1].push_back(SpawnInfo(Utils::GetRandom(10, 12), 1800.f, -300.f, 100.f)); // 15
	_spawnInfos[1].push_back(SpawnInfo(Utils::GetRandom(10, 12), 1900.f, -300.f, 100.f)); // 16
	_spawnInfos[1].push_back(SpawnInfo(Utils::GetRandom(10, 12), 1800.f, -200.f, 100.f)); // 17
	_spawnInfos[1].push_back(SpawnInfo(Utils::GetRandom(10, 12), 1900.f, -200.f, 100.f)); // 18
	_spawnInfos[1].push_back(SpawnInfo(Utils::GetRandom(10, 12), 1800.f, -100.f, 100.f)); // 19
	_spawnInfos[1].push_back(SpawnInfo(Utils::GetRandom(10, 12), 1900.f, -100.f, 100.f)); // 20

	// phase 2
	_spawnInfos[2].push_back(SpawnInfo(Utils::GetRandom(10, 12), 2430.f, 40.f, 100.f)); // 21
	_spawnInfos[2].push_back(SpawnInfo(Utils::GetRandom(10, 12), 2530.f, 40.f, 100.f)); // 22
	_spawnInfos[2].push_back(SpawnInfo(Utils::GetRandom(10, 12), 2630.f, 40.f, 100.f)); // 23
	_spawnInfos[2].push_back(SpawnInfo(Utils::GetRandom(10, 12), 2730.f, 40.f, 100.f)); // 24
	_spawnInfos[2].push_back(SpawnInfo(Utils::GetRandom(10, 12), 2430.f, 140.f, 100.f)); // 25
	_spawnInfos[2].push_back(SpawnInfo(Utils::GetRandom(10, 12), 2530.f, 140.f, 100.f)); // 26
	_spawnInfos[2].push_back(SpawnInfo(Utils::GetRandom(10, 12), 2630.f, 140.f, 100.f)); // 27
	_spawnInfos[2].push_back(SpawnInfo(Utils::GetRandom(10, 12), 2730.f, 140.f, 100.f)); // 28
	_spawnInfos[2].push_back(SpawnInfo(Utils::GetRandom(10, 12), 2430.f, 240.f, 100.f)); // 29
	_spawnInfos[2].push_back(SpawnInfo(Utils::GetRandom(10, 12), 2530.f, 240.f, 100.f)); // 30
	_spawnInfos[2].push_back(SpawnInfo(Utils::GetRandom(10, 12), 2630.f, 240.f, 100.f)); // 31
	_spawnInfos[2].push_back(SpawnInfo(Utils::GetRandom(10, 12), 2730.f, 240.f, 100.f)); // 32

	
	_spawnInfos[3].push_back(SpawnInfo(100, 3460.f, -900.f, 100.f)); // 33
	//_spawnInfos[3].push_back(SpawnInfo(100, 1800.f, -200.f, 100.f)); // 33

	for (int i = 0; i < _maxPhaseCount; i++)
	{
		_maxCount += (int32)_spawnInfos[i].size();
	}
}

void SpawningPool::AddKillCount(int32 count)
{
	_killCount += count;

	int32 pastPhase = _CurrentPhase;
	// 일정 킬 수가 넘으면 페이즈 변환
	if (_killCount >= 10 && _killCount < 18)
	{
		_CurrentPhase = 1;
	}
	else if (_killCount >= 18 && _killCount < 31)
	{
		_CurrentPhase = 2;
	}
	else if (_killCount >= 31)
	{
		_CurrentPhase = 3;
	}

	// 페이즈가 바뀌었을 경우 몬스터 소환
	if (pastPhase != _CurrentPhase)
	{
		if (RoomBaseRef room = _OwnerRoom.lock())
			room->DoTimer(1000, shared_from_this(), &SpawningPool::SpawnAndAddMonster);
	}

	// 목표 달성했을 경우 알려주기
	if (_killCount == _maxCount)
	{
		// todo
		SetFinished(true);
	}

	// 클라에게 게임 상태 변화 알리기
	SendSpawningPoolPkt();
}

void SpawningPool::SetFinished(bool flag)
{
	//if (flag == true && _finished == false)
	//{
	//	_finished = flag;
	//}

	_finished = flag;
}

void SpawningPool::SendSpawningPoolPkt()
{
	Protocol::S_SPAWNING_POOL poolPkt;
	poolPkt.set_kill_count(_killCount);
	poolPkt.set_finished(_finished);
	poolPkt.set_max_count(_maxCount);
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(poolPkt);
	if (RoomBaseRef Owner = _OwnerRoom.lock())
		Owner->Broadcast(sendBuffer);
}

void SpawningPool::SpawnAndAddMonster()
{
	RoomBaseRef OwnerRoom = _OwnerRoom.lock()->GetRoomRef();

	Protocol::S_SPAWN spawnPkt;
	vector<MonsterRef> monsters;

	if (_spawnInfos.size() <= _CurrentPhase)
		return;

	for (auto& info : _spawnInfos[_CurrentPhase])
	{
		FVector3 spawnPos = FVector3(info._spawnX, info._spawnY, info._spawnZ);
		if (_CurrentPhase == _maxPhaseCount - 1)
		{
			// boss
			BossRef boss = GObjectManager->CreateBoss(info._dataId, spawnPos, OwnerRoom);
			boss->SetOwnerSpawningPool(shared_from_this());
			bool isSpawned = OwnerRoom->AddObject(boss);
			if (isSpawned == false)
				continue;

			Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
			objectInfo->CopyFrom(*boss->objectInfo);

			cout << "Boss " << boss->objectInfo->object_id() << " is Spawned\n";
		}
		else
		{
			// monster
			MonsterRef monster = GObjectManager->CreateMonster(info._dataId, spawnPos, OwnerRoom);
			monster->SetOwnerSpawningPool(shared_from_this());
			bool isSpawned = OwnerRoom->AddObject(monster);
			if (isSpawned == false)
				continue;

			monsters.push_back(monster);

			Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
			objectInfo->CopyFrom(*monster->objectInfo);

			cout << "Monster " << monster->objectInfo->object_id() << " is Spawned\n";
		}
	}

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
	OwnerRoom->Broadcast(sendBuffer);
}
