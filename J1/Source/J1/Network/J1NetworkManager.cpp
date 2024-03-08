#include "J1NetworkManager.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "System/J1GameInstance.h"
#include "Common/TcpSocketBuilder.h"
#include "Kismet/GameplayStatics.h"
#include "Game/Player/J1MyPlayer.h"

void UJ1NetworkManager::ConnectToGameServer()
{
	// Create Socket
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip); // IpAddress를 Ip에 저장
	
	// InternetAddr : 붙어야 하는 서버의 주소
	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	// Ip주소와 Port번호 저장
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	// Connect Socket (Blocking)
	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));

		// Create Server Session
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run(); // SendWorker, RecvWorker 스레드 생성

		// TEMP : Lobby에서 캐릭터 선택창 등
		{
			Protocol::C_LOGIN Pkt;
			SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);
			SendPacket(SendBuffer);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

void UJ1NetworkManager::DisconnectFromGameServer()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;
	
	Protocol::C_LEAVE_GAME LeavePkt;
	SendPacket(LeavePkt);
}

void UJ1NetworkManager::HandleRecvPackets()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->HandleRecvPackets();
}

void UJ1NetworkManager::SendPacket(SendBufferRef SendBuffer)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->SendPacket(SendBuffer);
}

void UJ1NetworkManager::HandleSpawn(const Protocol::ObjectInfo& ObjectInfo, bool IsMine)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	// 중복 처리 체크
	const uint64 ObjectId = ObjectInfo.object_id();
	if (Players.Find(ObjectId) != nullptr)
		return;

	FVector SpawnLocation(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());

	if (IsMine) // MyPlayer일 경우
	{
		auto* PC = UGameplayStatics::GetPlayerController(this, 0);
		AJ1Player* Player = Cast<AJ1Player>(PC->GetPawn());
		if (Player == nullptr)
			return;

		Player->SetPlayerInfo(ObjectInfo.pos_info());
		MyPlayer = Player;
		Players.Add(ObjectInfo.object_id(), Player);
	}
	else // 내가 아닌 Player일 경우
	{
		AJ1Player* Player = Cast<AJ1Player>(World->SpawnActor(Cast<UJ1GameInstance>(GetGameInstance())->OtherPlayerClass, &SpawnLocation));
		Player->SetPlayerInfo(ObjectInfo.pos_info());
		Players.Add(ObjectInfo.object_id(), Player);
	}
}

void UJ1NetworkManager::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	HandleSpawn(EnterGamePkt.player(), true);
}

void UJ1NetworkManager::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	for (auto& Player : SpawnPkt.players())
	{
		HandleSpawn(Player, false);
	}
}

void UJ1NetworkManager::HandleDespawn(uint64 ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	AJ1Player** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	// Destroy Object
	World->DestroyActor(*FindActor);
}

void UJ1NetworkManager::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	for (auto& ObjectId : DespawnPkt.object_ids())
	{
		HandleDespawn(ObjectId);
	}
}

void UJ1NetworkManager::HandleMove(const Protocol::S_MOVE& MovePkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = MovePkt.info().object_id();
	AJ1Player** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	AJ1Player* Player = (*FindActor);
	if (Player->IsMyPlayer())
		return;

	const Protocol::PosInfo& Info = MovePkt.info();
	//Player->SetPlayerInfo(Info);
	Player->SetDestInfo(Info);
}
