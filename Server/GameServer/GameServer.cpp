﻿#include "pch.h"
#include <iostream>
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
//#include "ClientPacketHandler.h"
#include <tchar.h>
#include "Job.h"
#include "Protocol.pb.h"
#include "RoomManager.h"
#include "StartRoom.h"
#include "DataManager.h"

#include "recastnavigation/Recast.h"

enum
{
	WORKER_TICK = 64
};

void DoWorkerJob(ServerServiceRef& service)
{
	while (true)
	{
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;

		// 네트워크 입출력 처리 -> 인게임 로직까지 (패킷 핸들러에 의해)
		service->GetIocpCore()->Dispatch(10);

		// 예약된 일감 처리
		ThreadManager::DistributeReservedJobs();

		// 글로벌 큐
		ThreadManager::DoGlobalQueueWork();
	}
}

int main()
{
	//temp
	//rcContext* ctx = new rcContext();
	//rcConfig cfg;
	//memset(&cfg, 0, sizeof(cfg));
	//temp

	ServerPacketHandler::Init();

	ServerServiceRef service = make_shared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		make_shared<IocpCore>(),
		[=]() { return make_shared<GameSession>(); },
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
			{
				DoWorkerJob(service);
			});
	}

	// Main Thread
	//DoWorkerJob(service);

	// 0) data initialize
	{
		GDataManager->Init();
	}

	// 1) initialize all room
	{
		GRoomManager->DoAsync(&RoomManager::InitializeAllRoom);
		//GRoomManager->InitializeAllRoom();
	}

	// 2) start update tick
	{
		GRoomManager->DoAsync(&RoomManager::AllUpdateTickStart);
		//GRoomManager->AllUpdateTickStart();
	}

	cout << "Game Server : Open!!" << "\n";

	while (true)
	{
		//Protocol::S_CHAT pkt;
		//pkt.set_msg("HelloWorld");
		//auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		//
		//GSessionManager.Broadcast(sendBuffer);
		//this_thread::sleep_for(1s);

		this_thread::sleep_for(0.1s);
	}

	GThreadManager->Join();
}