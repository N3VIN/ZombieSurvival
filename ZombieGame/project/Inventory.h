#pragma once
#include <vector>
#include "Exam_HelperStructs.h"
#include "IExamInterface.h"

#define invalid_inventory_slot 4294967295U

class Inventory
{
public:

	Inventory(IExamInterface* pInterface);
	~Inventory() = default;

	Inventory(const Inventory& Inventory) = delete;
	Inventory(Inventory&& Inventory) = delete;
	Inventory& operator=(const Inventory& Inventory) = delete;
	Inventory& operator=(Inventory&& Inventory) = delete;

	bool PickupItem(EntityInfo entityInfo);
	bool UseGun();

	bool IsInventoryFull() const;
	bool IsGunAvailable() const;

	UINT GetFreeItemSlot() const;

private:

	IExamInterface* m_pInterface{};
	std::vector<eItemType> m_Inventory{ };

};