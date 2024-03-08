#include "PacketSession.h"
#include "NetworkWorker.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "ClientPacketHandler.h"

PacketSession::PacketSession(class FSocket* Socket) : Socket(Socket)
{
	// 패킷을 구분할 때 switch-case를 사용하지 않고 GPacketHander라는 배열을 65535 크기로 만든다.
	// Init()을 통해 각각의 배열 안에 패킷을 처리할 함수가 담긴 함수 포인터를 담아준다.
	ClientPacketHandler::Init();
}

PacketSession::~PacketSession()
{
	Disconnect();
}

void PacketSession::Run()
{
	RecvWorkerThread = MakeShared<RecvWorker>(Socket, AsShared()); // shared_from_this
	SendWorkerThread = MakeShared<SendWorker>(Socket, AsShared());
}

void PacketSession::HandleRecvPackets()
{
	while (true)
	{
		TArray<uint8> Packet;
		if (RecvPacketQueue.Dequeue(OUT Packet) == false)
			break;

		PacketSessionRef ThisPtr = AsShared(); // 참조값을 받는 함수에게 인자로 왼값을 넘기기 위한 용도
		ClientPacketHandler::HandlePacket(ThisPtr, Packet.GetData(), Packet.Num());
	}
}

void PacketSession::SendPacket(SendBufferRef SendBuffer)
{
	SendPacketQueue.Enqueue(SendBuffer);
}

void PacketSession::Disconnect()
{
	if (RecvWorkerThread)
	{
		RecvWorkerThread->Destroy();
		RecvWorkerThread = nullptr;
	}
	
	if (SendWorkerThread)
	{
		SendWorkerThread->Destroy();
		SendWorkerThread = nullptr;
	}
}
