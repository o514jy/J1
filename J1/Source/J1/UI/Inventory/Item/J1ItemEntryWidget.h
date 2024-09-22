#pragma once

#include "CoreMinimal.h"
#include "UI/J1UserWidget.h"
#include "J1ItemEntryWidget.generated.h"

class UTextBlock;
class UJ1InventorySlotsWidget;
class UJ1ItemBase;

UCLASS()
class J1_API UJ1ItemEntryWidget : public UJ1UserWidget
{
	GENERATED_BODY()
public:

public:
	/** initialize **/
	void Init(UJ1InventorySlotsWidget* InSlotsWidget, UJ1ItemBase* InItemInstance, int32 InItemCount);


public:
	/** information **/
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> CountText;

	UPROPERTY()
	TObjectPtr<UJ1InventorySlotsWidget> SlotsWidget;

	UPROPERTY()
	TObjectPtr<UJ1ItemBase> ItemInstance;

private:
	/** cache **/
	FIntPoint CachedFromSlotPos = FIntPoint::ZeroValue;
	FVector2D CachedDeltaWidgetPos = FVector2D::ZeroVector;
	int32 ItemCount = 0;
};
