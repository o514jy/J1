#pragma once

#include "CoreMinimal.h"
#include "UI/J1PopupWidget.h"
#include "J1InventoryWidget.generated.h"

class UJ1EquipmentSlotsWidget;
class UJ1InventorySlotsWidget;
//class FTimerHandle;

class UCanvasPanelSlot;
class UButton;

class USizeBox;

UCLASS()
class J1_API UJ1InventoryWidget : public UJ1PopupWidget
{
	GENERATED_BODY()
public:
	/** initialize **/
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	/** drag **/

	UFUNCTION()
	void OnButtonPressed();

	UFUNCTION()
	void OnButtonReleased();

	void UpdatePosition();

	bool GetMousePositionOnViewport(FVector2D& OutPos);

	FTimerHandle UpdateHandle;

	FVector2D Position;
	FVector2D MousePos;

public:
	/** information **/
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> InventoryNameButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UJ1EquipmentSlotsWidget> EquipmentSlotsWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UJ1InventorySlotsWidget> InventorySlotsWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> RootSizeBox;
};
