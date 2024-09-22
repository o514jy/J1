#pragma once

#include "CoreMinimal.h"
#include "UI/J1UserWidget.h"
#include "J1EquipmentSlotsWidget.generated.h"

class UJ1ItemSlotWidget;

UCLASS()
class J1_API UJ1EquipmentSlotsWidget : public UJ1UserWidget
{
	GENERATED_BODY()
public:


public:
	/** information **/
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UJ1ItemSlotWidget> EquipSlot_HelmetWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UJ1ItemSlotWidget> EquipSlot_ShoulderWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UJ1ItemSlotWidget> EquipSlot_ChestWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UJ1ItemSlotWidget> EquipSlot_LegWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UJ1ItemSlotWidget> EquipSlot_ShoesWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UJ1ItemSlotWidget> EquipSlot_WeaponWidget;
};
