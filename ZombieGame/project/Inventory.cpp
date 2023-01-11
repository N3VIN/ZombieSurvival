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
	if (IsInventoryFull())
	{
		return false;
	}

	/*if(IsItemAvailable(eItemType::SHOTGUN))
	{
		return false;
	}

	if (IsItemAvailable(eItemType::PISTOL))
	{
		return false;
	}*/

	ItemInfo itemInfo{};
	m_pInterface->Item_GetInfo(entityInfo, itemInfo);

	UINT emptySlotId{};

	switch (itemInfo.Type)
	{
	case 0 :
		emptySlotId = 0U;
		break;
	case 1:
		emptySlotId = 1U;
		break;
	case 2:
		emptySlotId = 2U;
		break;
	case 3:
		return PickupMedkit(entityInfo);
	}

	if (itemInfo.Type == eItemType::GARBAGE)
	{
		m_pInterface->Item_Destroy(entityInfo);
		return true;
	}

	if (m_pInterface->Item_Grab(entityInfo, itemInfo))
	{
		

		//emptySlotId = GetFreeItemSlot();

		m_pInterface->Inventory_AddItem(emptySlotId, itemInfo);

		m_Inventory.at(emptySlotId) = itemInfo.Type;
		return true;
	}

	return false;
	
}

bool Inventory::PickupMedkit(EntityInfo entityInfo)
{
	ItemInfo itemInfo{};
	m_pInterface->Item_GetInfo(entityInfo, itemInfo);

	UINT slot3 = 3U;
	UINT slot4 = 4U;
	if (m_Inventory.at(slot3) != eItemType::FOOD)
	{
		if (m_pInterface->Item_Grab(entityInfo, itemInfo))
		{
			m_pInterface->Inventory_AddItem(slot3, itemInfo);

			m_Inventory.at(slot3) = itemInfo.Type;
			return true;
		}
	}
	else
	{
		if (m_pInterface->Item_Grab(entityInfo, itemInfo))
		{
			m_pInterface->Inventory_AddItem(slot4, itemInfo);

			m_Inventory.at(slot4) = itemInfo.Type;
			return true;
		}
	}
	

	return false;
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

bool Inventory::UseFood()
{
	if (!IsItemAvailable(eItemType::FOOD))
	{
		return false;
	}

	UINT slotID{};
	for (int i{}; i < m_Inventory.size(); ++i)
	{
		if (m_Inventory.at(i) == eItemType::FOOD)
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
	for (int i{}; i < m_Inventory.size(); ++i)
	{
		if (m_Inventory.at(i) == eItemType::RANDOM_DROP)
		{
			return false;
		}
	}

	return true;
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
