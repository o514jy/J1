#include "UI/Inventory/J1InventorySlotsWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/Inventory/Item/J1ItemSlotWidget.h"
#include "UI/Inventory/Item/J1ItemEntryWidget.h"
#include "Inventory/J1InventoryManager.h"
#include "UI/J1UIManager.h"

void UJ1InventorySlotsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SlotWidgets.SetNum(X_COUNT * Y_COUNT);

	for (int32 y = 0; y < Y_COUNT; y++)
	{
		for (int32 x = 0; x < X_COUNT; x++)
		{
			int32 index = y * X_COUNT + x;

			UJ1ItemSlotWidget* SlotWidget = Cast<UJ1ItemSlotWidget>(GetManager(UI)->CreateWidgetExt(EUIType::ItemSlot));
			SlotWidgets[index] = SlotWidget;
			ItemSlotsGridPanel->AddChildToUniformGrid(SlotWidget, y, x);
		}
	}

	EntryWidgets.SetNum(X_COUNT * Y_COUNT);

	const TArray<TObjectPtr<UJ1ItemBase>>& Items = GetManager(Inventory)->GetAllInventoryItemArray();

	for (int32 i = 0; i < Items.Num(); i++)
	{
		const TObjectPtr<UJ1ItemBase>& Item = Items[i];
		FIntPoint ItemSlotPos = FIntPoint(i % X_COUNT, i / X_COUNT);
		OnInventoryEntryChanged(ItemSlotPos, Item);
	}
}

void UJ1InventorySlotsWidget::OnInventoryEntryChanged(const FIntPoint& InItemSlotPos, TObjectPtr<UJ1ItemBase> InItem)
{
	int32 SlotIndex = InItemSlotPos.Y * X_COUNT + InItemSlotPos.X;

	if (UJ1ItemEntryWidget* EntryWidget = EntryWidgets[SlotIndex])
	{
		if (InItem == nullptr)
		{
			ItemEntriesCanvasPanel->RemoveChild(EntryWidget);
			EntryWidgets[SlotIndex] = nullptr;
		}
	}
	else
	{
		EntryWidget = Cast<UJ1ItemEntryWidget>(GetManager(UI)->CreateWidgetExt(EUIType::ItemEntry));
		
		EntryWidgets[SlotIndex] = EntryWidget;

		UCanvasPanelSlot* CanvasPanelSlot = ItemEntriesCanvasPanel->AddChildToCanvas(EntryWidget);
		CanvasPanelSlot->SetAutoSize(true);
		CanvasPanelSlot->SetPosition(FVector2D(InItemSlotPos.X * 50, InItemSlotPos.Y * 50));

		// TODO
		EntryWidget->Init(this, InItem, 1);
	}
}
