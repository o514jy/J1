#include "J1GameInstance.h"
#include "J1AssetManager.h"
#include "J1NetworkManager.h"
#include "J1/Data/J1DataManager.h"
#include "J1/UI/J1UIManager.h"
#include "Blueprint/UserWidget.h"
#include "J1/UI/J1SceneWidget.h"
#include "J1/UI/J1DungeonClearWidget.h"
#include "J1/UI/J1DungeonStatusWidget.h"
#include "J1/UI/Scene/J1StartRoomSceneWidget.h"
#include "J1/UI/Inventory/J1InventoryWidget.h"
#include "J1/UI/Inventory/J1EquipmentSlotsWidget.h"
#include "J1/UI/Inventory/J1InventorySlotsWidget.h"
#include "J1/UI/Inventory/Item/J1ItemSlotWidget.h"
#include "J1/UI/Inventory/Item/J1ItemEntryWidget.h"
#include "J1/UI/Inventory/Item/Drag/J1ItemDragWidget.h"

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

	this->GetSubsystem<UJ1UIManager>()->SetInfo();

	this->GetSubsystem<UJ1NetworkManager>()->ConnectToGameServer();

	// temp
	//SceneWidgetInstance = CreateWidget<UJ1SceneWidget>(GetWorld(), SceneWidgetClass);
}
