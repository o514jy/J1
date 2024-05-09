#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"

class J1_API PacketSession : public TSharedFromThis<PacketSession> // 멤버변수로 WeakPtr를 만들어줘서 외부에서 접근이 가능하다.
{
public:
	// ClientPacketHandler::Init() 호출, GPacketHander 초기화
	PacketSession(class FSocket* Socket);
	// Disconnect()를 호출한다.
	~PacketSession();

	// RecvWorker와 SendWorker를 각각 스레드로 만든다.
	void Run();

	// Worker들이 쌓아준 RecvPacketQueue에서 패킷을 가져와서 ClientPacketHandler에게 넘긴다.
	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	// SendPacketQueue에 보낼 데이터를 쌓아둔다.
	void SendPacket(SendBufferRef SendBuffer);

	// RecvWorker와 SendWorker를 destroy한다.
	void Disconnect();
public:
	class FSocket* Socket; // 담당하는 Socket

	RecvWorkerRef RecvWorkerThread; // Recv를 담당하는 스레드
	SendWorkerRef SendWorkerThread; // Send를 담당하는 스레드

	// GameThread와 NetworkThread가 데이터 주고 받는 공용 큐.
	// 별도의 스레드를 만들고 거기서 메인 스레드의 함수로 접근하려고 하면 Crash가 난다.
	// 따라서 패킷 조립은 별도의 스레드가 해서 저장해놓고, 메인 스레드가 가져가게끔 해야한다.
	TQueue<TArray<uint8>> RecvPacketQueue;
	TQueue<SendBufferRef> SendPacketQueue;
};
