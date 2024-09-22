#include "UI/Inventory/Item/J1ItemEntryWidget.h"
#include "Components/TextBlock.h"
#include "Inventory/Item/J1ItemBase.h"
#include "UI/Inventory/J1InventorySlotsWidget.h"
#include "UI/Inventory/J1EquipmentSlotsWidget.h"

void UJ1ItemEntryWidget::Init(UJ1InventorySlotsWidget* InSlotsWidget, UJ1ItemBase* InItemInstance, int32 InItemCount)
{
	SlotsWidget = InSlotsWidget;
	ItemInstance = InItemInstance;
	ItemCount = InItemCount;
}
