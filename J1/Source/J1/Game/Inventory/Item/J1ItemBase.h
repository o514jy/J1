#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "UObject/NoExportTypes.h"
#include "J1ItemBase.generated.h"

class AJ1Player;
class UItemData;

UCLASS()
class J1_API UJ1ItemBase : public UObject
{
	GENERATED_BODY()
public:
	UJ1ItemBase();
	~UJ1ItemBase();

	/** static **/
	static TObjectPtr<UJ1ItemBase> MakeItem(const Protocol::ItemInfo& InItemInfo, TObjectPtr<AJ1Player> InOwner);

public:
	/** initialize **/
	virtual void SetInfo(int32 InTemplateId, TObjectPtr<AJ1Player> InOwner, TObjectPtr<UItemData> InItemData);

public:
	/** setter & getter **/
	TObjectPtr<UItemData> GetItemData() { return ItemData; }

public:
	/** information **/
	TObjectPtr<UItemData> ItemData;

	int32 TemplateId;
	int32 ItemId;

	TObjectPtr<AJ1Player> Owner;
};
