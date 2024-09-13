#pragma once

#include "CoreMinimal.h"
#include "Game/Inventory/Item/J1ItemBase.h"
#include "J1EquipmentItem.generated.h"


UCLASS()
class J1_API UJ1EquipmentItem : public UJ1ItemBase
{
	GENERATED_BODY()
public:
	UJ1EquipmentItem();
	~UJ1EquipmentItem();

public:
	/** setter & getter **/

public:
	/** initialize **/
	virtual void SetInfo(int32 InTemplateId, TObjectPtr<AJ1Player> InOwner, TObjectPtr<UItemData> InItemData) override;

public:
	/** information **/
	Protocol::ItemSubType EquipType;
};
