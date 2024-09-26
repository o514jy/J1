#include "UI/J1UIManager.h"
#include "J1PopupWidget.h"
#include "Scene/J1StartRoomSceneWidget.h"
#include "J1SceneWidget.h"
#include "Inventory/J1InventoryWidget.h"
#include "Scene/J1StartRoomSceneWidget.h"
#include "System/J1GameInstance.h"
#include "J1DungeonClearWidget.h"
#include "Inventory/J1EquipmentSlotsWidget.h"
#include "Inventory/J1InventorySlotsWidget.h"
#include "Inventory/Item/J1ItemSlotWidget.h"
#include "Inventory/Item/J1ItemEntryWidget.h"
#include "Inventory/Item/Drag/J1ItemDragWidget.h"
#include "J1PopupPosWidget.h"

void UJ1UIManager::SetInfo()
{
	// popup 위젯들의 위치를 들고있는 위젯 먼저 생성
	PopupPosWidgetInstance = Cast<UJ1PopupPosWidget>(CreateWidget<UJ1PopupPosWidget>(GetWorld(), GetWidgetClass(EUIType::PopupPos)));

	CacheAllPopups();
}

void UJ1UIManager::CacheAllPopups()
{
	CreatePopupWidget(EUIType::Inventory)->SetbIsOpened(false);
	CreatePopupWidget(EUIType::DungeonClear)->SetbIsOpened(false);

}

TObjectPtr<UJ1PopupWidget> UJ1UIManager::CreatePopupWidget(EUIType InType)
{
	TObjectPtr<UJ1PopupWidget> widget = nullptr;

	widget = Cast<UJ1PopupWidget>(CreateWidgetExt(InType));
	if (widget == nullptr)
		return nullptr;

	TObjectPtr<UJ1PopupWidget>* widgetPtr = Popups.Find(InType);
	if (widgetPtr == nullptr)
		Popups.Add(InType, widget);

	if (PopupPosWidgetInstance != nullptr)
		widget->defaultOpenPos = PopupPosWidgetInstance->GetPopupWidgetPos(InType);

	return widget;
}

TObjectPtr<UJ1PopupWidget> UJ1UIManager::ShowPopupWidget(EUIType InType)
{
	TObjectPtr<UJ1PopupWidget> widget = GetPopupWidget(InType);

	if (widget == nullptr)
		return nullptr;

	widget->SetbIsOpened(true);
	widget->AddToViewport();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	//FInputModeGameAndUI InputMode;
	//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//InputMode.SetHideCursorDuringCapture(false);
	//if (PlayerController)
	//	PlayerController->SetInputMode(InputMode);
	//widget->SetPositionInViewport(widget->defaultOpenPos, false);

	PopupStack.Add(widget);

	return widget;
}

TObjectPtr<UJ1SceneWidget> UJ1UIManager::ShowSceneWidget(EUIType InType)
{
	TObjectPtr<UJ1SceneWidget> widget = nullptr;

	switch (InType)
	{
	case EUIType::StartRoom:
		widget = CreateWidget<UJ1StartRoomSceneWidget>(GetWorld(), UJ1StartRoomSceneWidget::StaticClass());
		break;
	default:
		break;
	}

	widget->AddToViewport();
	SceneWidget = widget;

	return widget;
}

TObjectPtr<UJ1UserWidget> UJ1UIManager::CreateWidgetExt(EUIType InType)
{
	TObjectPtr<UJ1UserWidget> widget = nullptr;

	switch (InType)
	{
	case EUIType::PopupPos:
		widget = CreateWidget<UJ1PopupPosWidget>(GetWorld(), GetWidgetClass(InType));
		break;
	case EUIType::Inventory:
		widget = CreateWidget<UJ1InventoryWidget>(GetWorld(), GetWidgetClass(InType));
		break;
	case EUIType::DungeonClear:
		widget = CreateWidget<UJ1DungeonClearWidget>(GetWorld(), GetWidgetClass(InType));
		break;
	case EUIType::StartRoom:
		widget = CreateWidget<UJ1StartRoomSceneWidget>(GetWorld(), GetWidgetClass(InType));
		break;
	case EUIType::InventorySlots:
		widget = CreateWidget<UJ1InventorySlotsWidget>(GetWorld(), GetWidgetClass(InType));
		break;
	case EUIType::EquipmentSlots:
		widget = CreateWidget<UJ1EquipmentSlotsWidget>(GetWorld(), GetWidgetClass(InType));
		break;
	case EUIType::ItemSlot:
		widget = CreateWidget<UJ1ItemSlotWidget>(GetWorld(), GetWidgetClass(InType));
		break;
	case EUIType::ItemEntry:
		widget = CreateWidget<UJ1ItemEntryWidget>(GetWorld(), GetWidgetClass(InType));
		break;
	case EUIType::ItemDrag:
		widget = CreateWidget<UJ1ItemDragWidget>(GetWorld(), GetWidgetClass(InType));
		break;
	default:
		break;
	}

	if (widget == nullptr)
		return nullptr;

	widget->UIType = InType;

	return widget;
}

TObjectPtr<UJ1PopupWidget> UJ1UIManager::GetPopupWidget(EUIType InType)
{
	TObjectPtr<UJ1PopupWidget> widget = nullptr;

	TObjectPtr<UJ1PopupWidget>* widgetPtr = Popups.Find(InType);
	if (widgetPtr == nullptr)
		return nullptr;

	widget = *widgetPtr;

	return widget;
}

bool UJ1UIManager::ClosePopupWidget(EUIType InType)
{
	TObjectPtr<UJ1PopupWidget> widget = GetPopupWidget(InType);
	if (widget == nullptr)
		return false;

	if (widget->GetbIsOpened() == false)
		return false;

	widget->SetbIsOpened(false);

	PopupStack.Remove(widget);

	return true;
}

TSubclassOf<UJ1UserWidget> UJ1UIManager::GetWidgetClass(EUIType InType)
{
	TSubclassOf<UJ1UserWidget> widgetClass = TSubclassOf<UJ1UserWidget>();

	TObjectPtr<UJ1GameInstance> j1gi = Cast<UJ1GameInstance>(GetGameInstance());

	switch (InType)
	{
	case EUIType::PopupPos:
		widgetClass = j1gi->PopupPosWidgetClass;
		break;
	case EUIType::Inventory:
		widgetClass = j1gi->InventoryWidgetClass;
		break;
	case EUIType::DungeonClear:
		widgetClass = j1gi->DungeonClearWidgetClass;
		break;
	case EUIType::StartRoom:
		widgetClass = j1gi->StartRoomWidgetClass;
		break;
	case EUIType::InventorySlots:
		widgetClass = j1gi->InventorySlotsWidgetClass;
		break;
	case EUIType::EquipmentSlots:
		widgetClass = j1gi->EquipmentSlotsWidgetClass;
		break;
	case EUIType::ItemSlot:
		widgetClass = j1gi->ItemSlotWidgetClass;
		break;
	case EUIType::ItemEntry:
		widgetClass = j1gi->ItemEntryWidgetClass;
		break;
	case EUIType::ItemDrag:
		widgetClass = j1gi->ItemDragWidgetClass;
		break;
	default:
		break;
	}

	return widgetClass;
}
