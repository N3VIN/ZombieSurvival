#include "stdafx.h"
#include "Inventory.h"

Inventory::Inventory(IExamInterface* pInterface)
	: m_pInterface{pInterface}
{
	m_Inventory.reserve(m_pInterface->Inventory_GetCapacity());

	for (int i{}; i < m_pInterface->Inventory_GetCapacity(); ++i)
	{
		m_Inventory.push_back(eItemType::RANDOM_DROP);
	}

	//m_Inventory. eItemType::RANDOM_DROP;

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
	/*for (UINT i{ 0 }; i < m_pInterface->Inventory_GetCapacity(); ++i)
	{
		if (!m_pInterface->Inventory_GetItem(i, itemInfo))
		{
			emptySlotId = i;
		}
	}*/

	emptySlotId = GetFreeItemSlot();

	m_pInterface->Inventory_AddItem(emptySlotId, itemInfo);
	m_Inventory.at(emptySlotId) = itemInfo.Type;
	return true;
}

bool Inventory::UseGun()
{
	if (!IsGunAvailable())
	{
		return false;
	}

	UINT slotID{};
	for (int i{}; i < m_Inventory.size(); ++i)
	{
		if (m_Inventory.at(i) == eItemType::SHOTGUN)
		{
			slotID = i;
		}
		else if (m_Inventory.at(i) == eItemType::PISTOL)
		{
			slotID = i;
		}
	}

	m_pInterface->Inventory_UseItem(slotID);

	ItemInfo itemInfo{};
	if (m_pInterface->Inventory_GetItem(slotID, itemInfo))
	{
		if (m_pInterface->Weapon_GetAmmo(itemInfo) <= 0)
		{
			m_pInterface->Inventory_RemoveItem(slotID);
			m_Inventory.at(slotID) = eItemType::RANDOM_DROP;
		}
	}

	return true;
}

bool Inventory::UseMedKit()
{
	if (!IsItemAvailable(eItemType::MEDKIT))
	{
		return false;
	}

	UINT slotID{};
	for (int i{}; i < m_Inventory.size(); ++i)
	{
		if (m_Inventory.at(i) == eItemType::MEDKIT)
		{
			slotID = i;
		}
	}

	m_pInterface->Inventory_UseItem(slotID);

	ItemInfo itemInfo{};
	if (m_pInterface->Inventory_GetItem(slotID, itemInfo))
	{
		if (m_pInterface->Medkit_GetHealth(itemInfo) <= 0)
		{
			m_pInterface->Inventory_RemoveItem(slotID);
			m_Inventory.at(slotID) = eItemType::RANDOM_DROP;
		}
	}

	return true;
}

bool Inventory::IsInventoryFull() const
{
	return std::all_of(m_Inventory.begin(), m_Inventory.end(), [](eItemType itemType)
		{
			return itemType != eItemType::RANDOM_DROP;
		}
	);
}

bool Inventory::IsGunAvailable() const
{
	for (int i{}; i < m_Inventory.size(); ++i)
	{
		if (m_Inventory.at(i) == eItemType::PISTOL || m_Inventory.at(i) == eItemType::SHOTGUN)
		{
			return true;
		}
	}

	return false;
}

bool Inventory::IsItemAvailable(eItemType itemType) const
{
	for (int i{}; i < m_Inventory.size(); ++i)
	{
		if (m_Inventory.at(i) == itemType)
		{
			return true;
		}
	}

	return false;
}

UINT Inventory::GetFreeItemSlot() const
{
	ItemInfo itemInfo;
	for (UINT i{ 0 }; i < m_pInterface->Inventory_GetCapacity(); ++i)
	{
		if (!m_pInterface->Inventory_GetItem(i, itemInfo))
		{
			return i;
		}
	}

	return invalid_inventory_slot;
}
