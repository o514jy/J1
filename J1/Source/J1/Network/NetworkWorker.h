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

	// ���ۿ� �о���� �� ���
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
	// RecvWorkerThread ����
	RecvWorker(FSocket* Socket, PacketSessionRef Session);
	~RecvWorker();

	virtual bool Init() override;
	// ������ ���� �� ȣ��Ǵ� �Լ�, loop�� ���鼭 �޾ƿ� ��Ŷ�� RecvPacketQueue�� �ִ´�.
	virtual uint32 Run() override;
	virtual void Exit() override;

	// Running�� false�� �ٲ㼭 loop�� ����
	void Destroy();

private:
	// ��Ŷ ����� �Ľ��ؼ� �ش��ϴ� ��Ŷ�� �޾ƿ´�.
	bool ReceivePacket(TArray<uint8>& OutPacket);
	//  Size�� �ش��ϴ� ��Ŷ�� ��� �����Ҷ����� Recv�� �Ѵ�.
	bool ReceiveDesiredBytes(uint8* Results, int32 Size);

protected:
	FRunnableThread* Thread = nullptr; // RecvWorkerThread
	bool Running = true;
	FSocket* Socket; // ����ϴ� Socket (����)
	TWeakPtr<PacketSession> SessionRef;
};

/*----------------
	SendWorker
-----------------*/

class J1_API SendWorker : public FRunnable
{
public:
	// SendWorkerThread ����
	SendWorker(FSocket* Socket, PacketSessionRef Session);
	~SendWorker();

	virtual bool Init() override;
	// loop�� ���鼭 SendPacketQueue�� �ִ� �����͸� ������ ������.
	virtual uint32 Run() override;
	virtual void Exit() override;

	// SendBuffer�ȿ� �ִ� �����͸� ������.
	bool SendPacket(SendBufferRef SendBuffer);

	// Running�� false�� �ٲ㼭 loop�� ����
	void Destroy();

private:
	// Size�� �ش��ϴ� ��Ŷ�� ���� ���������� Send�� �Ѵ�.
	bool SendDesiredBytes(const uint8* Buffer, int32 Size);

protected:
	FRunnableThread* Thread = nullptr;
	bool Running = true;
	FSocket* Socket; // ����ϴ� Socket (����)
	TWeakPtr<PacketSession> SessionRef;
};