#pragma once

#include "CoreMinimal.h"
#include "UI/J1UserWidget.h"
#include "J1InventorySlotsWidget.generated.h"

class UJ1ItemSlotWidget;
class UUniformGridPanel;

class UJ1ItemBase;
class UJ1ItemEntryWidget;
class UCanvasPanel;

UCLASS()
class J1_API UJ1InventorySlotsWidget : public UJ1UserWidget
{
	GENERATED_BODY()
public:
	/** initialize **/
	virtual void NativeConstruct() override;

protected:
	void OnInventoryEntryChanged(const FIntPoint& InItemSlotPos, TObjectPtr<UJ1ItemBase> InItem);
	
	//virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	//virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	//virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	//void FinishDrag();

public:
	/** information **/
	UPROPERTY()
	TArray<TObjectPtr<UJ1ItemSlotWidget>> SlotWidgets;

	UPROPERTY()
	TArray<TObjectPtr<UJ1ItemEntryWidget>> EntryWidgets;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> ItemSlotsGridPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> ItemEntriesCanvasPanel;


private:
	FIntPoint PrevDragOverSlotPos = FIntPoint(-1, -1);
	const int X_COUNT = 10;
	const int Y_COUNT = 5;
};
