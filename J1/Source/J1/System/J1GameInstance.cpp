#include "J1GameInstance.h"
#include "J1AssetManager.h"
#include "J1NetworkManager.h"
#include "J1/Data/J1DataManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(J1GameInstance)

UJ1GameInstance::UJ1GameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UJ1GameInstance::Init()
{
	Super::Init();

	UJ1AssetManager::Initialize();
}

void UJ1GameInstance::Shutdown()
{
	Super::Shutdown();
}

void UJ1GameInstance::BeginPlay()
{
	this->GetSubsystem<UJ1DataManager>()->SetInfo();

	this->GetSubsystem<UJ1NetworkManager>()->ConnectToGameServer();
}
