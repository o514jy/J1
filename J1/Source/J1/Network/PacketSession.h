#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"

class J1_API PacketSession : public TSharedFromThis<PacketSession> // ��������� WeakPtr�� ������༭ �ܺο��� ������ �����ϴ�.
{
public:
	// ClientPacketHandler::Init() ȣ��, GPacketHander �ʱ�ȭ
	PacketSession(class FSocket* Socket);
	// Disconnect()�� ȣ���Ѵ�.
	~PacketSession();

	// RecvWorker�� SendWorker�� ���� ������� �����.
	void Run();

	// Worker���� �׾��� RecvPacketQueue���� ��Ŷ�� �����ͼ� ClientPacketHandler���� �ѱ��.
	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	// SendPacketQueue�� ���� �����͸� �׾Ƶд�.
	void SendPacket(SendBufferRef SendBuffer);

	// RecvWorker�� SendWorker�� destroy�Ѵ�.
	void Disconnect();
public:
	class FSocket* Socket; // ����ϴ� Socket

	RecvWorkerRef RecvWorkerThread; // Recv�� ����ϴ� ������
	SendWorkerRef SendWorkerThread; // Send�� ����ϴ� ������

	// GameThread�� NetworkThread�� ������ �ְ� �޴� ���� ť.
	// ������ �����带 ����� �ű⼭ ���� �������� �Լ��� �����Ϸ��� �ϸ� Crash�� ����.
	// ���� ��Ŷ ������ ������ �����尡 �ؼ� �����س���, ���� �����尡 �������Բ� �ؾ��Ѵ�.
	TQueue<TArray<uint8>> RecvPacketQueue;
	TQueue<SendBufferRef> SendPacketQueue;
};
