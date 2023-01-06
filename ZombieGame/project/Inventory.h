#pragma once
#include <vector>
#include "Exam_HelperStructs.h"
#include "IExamInterface.h"

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

	bool IsInventoryFull() const;

private:

	IExamInterface* m_pInterface{};
	std::vector<eItemType> m_pInventory{ };

};