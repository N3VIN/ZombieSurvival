#include "stdafx.h"
#include "Inventory.h"

Inventory::Inventory(IExamInterface* pInterface)
	: m_pInterface{pInterface}
{
	m_pInventory.reserve(m_pInterface->Inventory_GetCapacity());

	for (int i{}; i < m_pInterface->Inventory_GetCapacity(); ++i)
	{
		m_pInventory.push_back(eItemType::RANDOM_DROP);
	}

	//m_pInventory. eItemType::RANDOM_DROP;

}

bool Inventory::PickupItem(EntityInfo entityInfo) 
{
	ItemInfo itemInfo{};
	m_pInterface->Item_GetInfo(entityInfo, itemInfo);

	if (IsInventoryFull())
	{
		return false;
	}

	if (itemInfo.Type == eItemType::GARBAGE)
	{
		m_pInterface->Item_Destroy(entityInfo);
		return true;
	}

	if (!m_pInterface->Item_Grab(entityInfo, itemInfo))
	{
		return false;
	}

	UINT emptySlotId{};
	for (UINT i{ 0 }; i < m_pInterface->Inventory_GetCapacity(); ++i)
	{
		if (!m_pInterface->Inventory_GetItem(i, itemInfo))
		{
			emptySlotId = i;
		}
	}

	m_pInterface->Inventory_AddItem(emptySlotId, itemInfo);
	m_pInventory.at(emptySlotId) = itemInfo.Type;
	return true;
}

bool Inventory::IsInventoryFull() const
{
	return std::all_of(m_pInventory.begin(), m_pInventory.end(), [](eItemType itemType)
		{
			return itemType != eItemType::RANDOM_DROP;
		}
	);
}