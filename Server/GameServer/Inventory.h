#pragma once

class Inventory
{
public:
	Inventory();
	~Inventory();

public:
	/** process **/
	void Add(ItemBaseRef item);

public:
	/** initialize **/
	void SetInfo(PlayerRef owner);

public:
	/** network **/


public:
	/** information **/
	unordered_map<uint64, ItemBaseRef> _allItems;
	unordered_map<Protocol::ItemSlotType, EquipmentItemRef> _equippedItems;
	unordered_map<uint64, ItemBaseRef> _inventoryItems;

	PlayerRef _owner;
};

