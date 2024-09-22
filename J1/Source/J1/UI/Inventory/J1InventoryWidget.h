#pragma once

#include "CoreMinimal.h"
#include "UI/J1PopupWidget.h"
#include "J1InventoryWidget.generated.h"

class UJ1EquipmentSlotsWidget;
class UJ1InventorySlotsWidget;

class UButton;

UCLASS()
class J1_API UJ1InventoryWidget : public UJ1PopupWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void OnInventoryNameButtonPressed();
	UFUNCTION(BlueprintCallable)
	void OnInventoryNameButtonReleased();

	// 마우스 위치를 계속 업데이트하는 함수
	UFUNCTION()
	void UpdateLocation();

private:
	/** helper **/
	UFUNCTION()
	void StartUpdate();

	UFUNCTION()
	void StopUpdate();

public:
	/** information **/
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> InventoryNameButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UJ1EquipmentSlotsWidget> EquipmentSlotsWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UJ1InventorySlotsWidget> InventorySlotsWidget;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanelSlot* MainCanvasPanel;

private:
	FTimerHandle UpdateLocationTimerHandle;
};
