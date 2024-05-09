#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "Containers/Queue.h"
#include "Utils/J1NetworkUtils.h"

class FSocket;
class PacketSession;

/*------------------
	PacketHeader
-------------------*/

struct J1_API FPacketHeader // [Size][ID]
{
	FPacketHeader() : PacketSize(0), PacketID(0)
	{
	}

	FPacketHeader(uint16 PacketSize, uint16 PacketID) : PacketSize(PacketSize), PacketID(PacketID)
	{
	}

	// 버퍼에 밀어넣을 때 사용
	friend FArchive& operator<<(FArchive& Ar, FPacketHeader& Header)
	{
		Ar << Header.PacketSize;
		Ar << Header.PacketID;
		return Ar;
	}

	uint16 PacketSize; // Header + Payload
	uint16 PacketID;
};

/*----------------
	RecvWorker
-----------------*/

class J1_API RecvWorker : public FRunnable
{
public:
	// RecvWorkerThread 생성
	RecvWorker(FSocket* Socket, PacketSessionRef Session);
	~RecvWorker();

	virtual bool Init() override;
	// 스레드 실행 시 호출되는 함수, loop를 돌면서 받아온 패킷을 RecvPacketQueue에 넣는다.
	virtual uint32 Run() override;
	virtual void Exit() override;

	// Running을 false로 바꿔서 loop를 종료
	void Destroy();

private:
	// 패킷 헤더를 파싱해서 해당하는 패킷을 받아온다.
	bool ReceivePacket(TArray<uint8>& OutPacket);
	//  Size에 해당하는 패킷이 모두 도착할때까지 Recv를 한다.
	bool ReceiveDesiredBytes(uint8* Results, int32 Size);

protected:
	FRunnableThread* Thread = nullptr; // RecvWorkerThread
	bool Running = true;
	FSocket* Socket; // 담당하는 Socket (서버)
	TWeakPtr<PacketSession> SessionRef;
};

/*----------------
	SendWorker
-----------------*/

class J1_API SendWorker : public FRunnable
{
public:
	// SendWorkerThread 생성
	SendWorker(FSocket* Socket, PacketSessionRef Session);
	~SendWorker();

	virtual bool Init() override;
	// loop를 돌면서 SendPacketQueue에 있는 데이터를 꺼내서 보낸다.
	virtual uint32 Run() override;
	virtual void Exit() override;

	// SendBuffer안에 있는 데이터를 보낸다.
	bool SendPacket(SendBufferRef SendBuffer);

	// Running을 false로 바꿔서 loop를 종료
	void Destroy();

private:
	// Size에 해당하는 패킷을 전부 보낼때까지 Send를 한다.
	bool SendDesiredBytes(const uint8* Buffer, int32 Size);

protected:
	FRunnableThread* Thread = nullptr;
	bool Running = true;
	FSocket* Socket; // 담당하는 Socket (서버)
	TWeakPtr<PacketSession> SessionRef;
};