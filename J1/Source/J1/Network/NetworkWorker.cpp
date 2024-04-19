#include "NetworkWorker.h"
#include "Sockets.h"
#include "PacketSession.h"
#include "Serialization/ArrayWriter.h"

/*----------------
	RecvWorker
-----------------*/

RecvWorker::RecvWorker(FSocket* Socket, PacketSessionRef Session) : Socket(Socket), SessionRef(Session)
{
	Thread = FRunnableThread::Create(this, TEXT("RecvWorkerThread"));
}

RecvWorker::~RecvWorker()
{

}

bool RecvWorker::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Thread Init")));
	return true;
}

uint32 RecvWorker::Run()
{
	while (Running)
	{
		TArray<uint8> Packet;

		if (ReceivePacket(OUT Packet))
		{
			if (PacketSessionRef Session = SessionRef.Pin())
			{
				Session->RecvPacketQueue.Enqueue(Packet);
			}
		}
	}

	return 0;
}

void RecvWorker::Exit()
{

}

void RecvWorker::Destroy()
{
	Running = false;
}

bool RecvWorker::ReceivePacket(TArray<uint8>& OutPacket)
{
	// ��Ŷ ��� �Ľ�
	const int32 HeaderSize = sizeof(FPacketHeader);
	TArray<uint8> HeaderBuffer;
	HeaderBuffer.AddZeroed(HeaderSize); // ������ �̸� Ȯ��

	// ��� ��ŭ�� �ݵ�� �޾ƿ;� ���� �� �ִ�.
	if (ReceiveDesiredBytes(HeaderBuffer.GetData(), HeaderSize) == false)
		return false;

	// ID, Size ����
	FPacketHeader Header;
	{
		FMemoryReader Reader(HeaderBuffer);
		Reader << Header; // Reader�� ���� Header�� ���� �����´�.
		UE_LOG(LogTemp, Log, TEXT("Recv PacketID : %d, PacketSize : %d"), Header.PacketID, Header.PacketSize);
	}

	// ��Ŷ ��� ����
	OutPacket = HeaderBuffer;

	// ��Ŷ ���� �Ľ�
	TArray<uint8> PayloadBuffer;
	const int32 PayloadSize = Header.PacketSize - HeaderSize;
	if (PayloadSize == 0)
		return true;

	OutPacket.AddZeroed(PayloadSize);

	// ��� ������ Payload �����͸� �޾ƿ´�.
	if (ReceiveDesiredBytes(&OutPacket[HeaderSize], PayloadSize))
		return true;

	return false;
}

bool RecvWorker::ReceiveDesiredBytes(uint8* Results, int32 Size)
{
	uint32 PendingDataSize;
	// ���� ���� �� Packet Size�� 0���� �´�.
	if (Socket->HasPendingData(PendingDataSize) == false || PendingDataSize <= 0)
		return false;

	int32 Offset = 0; // ���� ���� �������� ��ġ�� ������Ʈ�ϴ� �뵵

	while (Size > 0)
	{
		int32 NumRead = 0; // �󸶸�ŭ �޾Ҵ���
		Socket->Recv(Results + Offset, Size, OUT NumRead);
		check(NumRead <= Size); // �ƹ��� ���� �о���� Size���� ������ �������� ��Ȳ

		if (NumRead <= 0) // ���Ḧ �ǹ�
			return false;

		// Size��ŭ ���� �� ���� ���� ���, ���� ��ŭ Offset�� �����ش�.
		Offset += NumRead; // �ٽ� Recv�Ҷ� Results + Offset���� ����
		Size -= NumRead; // ������ŭ ������ ���� Size�� �ٿ��ش�.
	}

	return true;
}

/*----------------
	SendWorker
-----------------*/

SendWorker::SendWorker(FSocket* Socket, PacketSessionRef Session) : Socket(Socket), SessionRef(Session)
{
	Thread = FRunnableThread::Create(this, TEXT("SendWorkerThread"));
}

SendWorker::~SendWorker()
{
}

bool SendWorker::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Send Thread Init")));

	return true;
}

uint32 SendWorker::Run()
{
	while (Running)
	{
		SendBufferRef SendBuffer;

		if (PacketSessionRef Session = SessionRef.Pin())
		{
			if (Session->SendPacketQueue.Dequeue(OUT SendBuffer))
			{
				SendPacket(SendBuffer);
			}
		}

		// Sleep?
	}

	return 0;
}

void SendWorker::Exit()
{
}

bool SendWorker::SendPacket(SendBufferRef SendBuffer)
{
	if (SendDesiredBytes(SendBuffer->Buffer(), SendBuffer->WriteSize()) == false)
		return false;

	return true;
}

void SendWorker::Destroy()
{
	Running = false;
}

bool SendWorker::SendDesiredBytes(const uint8* Buffer, int32 Size)
{
	while (Size > 0)
	{
		int32 BytesSent = 0; // ���ݱ��� ���� �������� ũ��
		if (Socket->Send(Buffer, Size, BytesSent) == false)
			return false;

		Size -= BytesSent;
		Buffer += BytesSent;
	}

	return true;
}
