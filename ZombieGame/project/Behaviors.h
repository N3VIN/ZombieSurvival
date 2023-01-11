/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
/*=============================================================================*/
// Behaviors.h: Implementation of certain reusable behaviors for the BT version of the Agario Game
/*=============================================================================*/
#ifndef ELITE_APPLICATION_BEHAVIOR_TREE_BEHAVIORS
#define ELITE_APPLICATION_BEHAVIOR_TREE_BEHAVIORS
//-----------------------------------------------------------------
// Includes & Forward Declarations.
//-----------------------------------------------------------------
#include "../inc/EliteMath/EMath.h"
#include "IExamPlugin.h"
#include "IExamInterface.h"
#include "Exam_HelperStructs.h"
#include "EBehaviorTree.h"
#include "Timer.h"
#include "Inventory.h"

//-----------------------------------------------------------------
// Conditions.
//-----------------------------------------------------------------

namespace BT_Conditions
{
	bool IsEnemyInView(Elite::Blackboard* pBlackboard)
	{
		std::vector<EnemyInfo>* pEnemyInfos{ nullptr };

		if (!pBlackboard->GetData("enemiesInFOV", pEnemyInfos) || pEnemyInfos == nullptr)
		{
			return false;
		}

		if (!pEnemyInfos->empty())
		{
			return true;
		}

		return false;
	}

	bool IsPurgeZoneInView(Elite::Blackboard* pBlackboard)
	{
		std::vector<PurgeZoneInfo>* pPurgeZoneInfos{ nullptr };

		if (!pBlackboard->GetData("purgeZoneInFOV", pPurgeZoneInfos) || pPurgeZoneInfos == nullptr)
		{
			return false;
		}

		if (!pPurgeZoneInfos->empty())
		{
			return true;
		}

		return false;
	}


	bool IsHouseInView(Elite::Blackboard* pBlackboard)
	{
		std::vector<HouseInfo>* pHouseInfos{ nullptr };

		if (!pBlackboard->GetData("housesInFOV", pHouseInfos) || pHouseInfos == nullptr)
		{
			return false;
		}

		if (!pHouseInfos->empty())
		{
			return true;
		}

		return false;
	}

	bool IsItemInView(Elite::Blackboard* pBlackboard)
	{
		std::vector<EntityInfo>* pItemInfos{ nullptr };

		if (!pBlackboard->GetData("itemsInFOV", pItemInfos) || pItemInfos == nullptr)
		{
			return false;
		}

		if (!pItemInfos->empty())
		{
			return true;
		}

		return false;
	}

	bool IsBitten(Elite::Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		Timer* pBittenTimer{ nullptr };
		
		if (!pBlackboard->GetData("interface", pInterface) || pInterface == nullptr)
		{
			return false;
		}

		if (!pBlackboard->GetData("bittenTimer", pBittenTimer) || pBittenTimer == nullptr)
		{
			return false;
		}

		auto agentInfo = pInterface->Agent_GetInfo();

		if (agentInfo.Bitten)
		{
			pBittenTimer->Enable();
		}

		if (pBittenTimer->IsEnabled() && !pBittenTimer->IsDone())
		{
			return true;
		}

		return false;
	}

	bool IsInsideHouse(Elite::Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		std::vector<HouseInfo>* pHousesChecked{ nullptr };
		std::vector<HouseInfo>* pHouseInfos{ nullptr };


		if (!pBlackboard->GetData("interface", pInterface) || pInterface == nullptr)
		{
			return false;
		}

		if (!pBlackboard->GetData("housesInFOV", pHouseInfos) || pHouseInfos == nullptr)
		{
			return false;
		}

		if (!pBlackboard->GetData("housesChecked", pHousesChecked) || pHousesChecked == nullptr)
		{
			return false;
		}

		auto agentInfo = pInterface->Agent_GetInfo();

		HouseInfo house{};
		for (int i{ 0 }; i < pHouseInfos->size(); ++i)
		{

			house = pHouseInfos->at(i);
		}
		
		if (Elite::Distance(agentInfo.Position, house.Center) < 1.5f)
		{
			if (pHousesChecked->empty())
			{
				pHousesChecked->push_back(house);
			}

			for (int i{ 0 }; i < pHousesChecked->size(); ++i)
			{
				if (house.Center != pHousesChecked->at(i).Center)
				{
					pHousesChecked->push_back(house);
				}
			}

			return true;
		}

		return false;
	}

	bool IsGunAvailable(Elite::Blackboard* pBlackboard)
	{
		Inventory* pInventory{ nullptr };

		if (!pBlackboard->GetData("inventory", pInventory) || pInventory == nullptr)
		{
			return false;
		}

		return pInventory->IsGunAvailable();

	}

	bool IsMedKitAvailable(Elite::Blackboard* pBlackboard)
	{
		Inventory* pInventory{ nullptr };

		if (!pBlackboard->GetData("inventory", pInventory) || pInventory == nullptr)
		{
			return false;
		}

		return pInventory->IsItemAvailable(eItemType::MEDKIT);

	}

	bool IsFoodAvailable(Elite::Blackboard* pBlackboard)
	{
		Inventory* pInventory{ nullptr };

		if (!pBlackboard->GetData("inventory", pInventory) || pInventory == nullptr)
		{
			return false;
		}

		return pInventory->IsItemAvailable(eItemType::FOOD);

	}

	bool DoINeedMedKit(Elite::Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		Inventory* pInventory{ nullptr };

		if (!pBlackboard->GetData("interface", pInterface) || pInterface == nullptr)
		{
			return false;
		}

		if (!pBlackboard->GetData("inventory", pInventory) || pInventory == nullptr)
		{
			return false;
		}

		const auto currentHealth = pInterface->Agent_GetInfo().Health;

		constexpr float medKitMin{ 6.f };

		if (currentHealth <= medKitMin)
		{
			if (IsMedKitAvailable(pBlackboard))
			{
				return true;
			}
		}

		return false;
	}

	bool DoINeedFood(Elite::Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		Inventory* pInventory{ nullptr };

		if (!pBlackboard->GetData("interface", pInterface) || pInterface == nullptr)
		{
			return false;
		}

		if (!pBlackboard->GetData("inventory", pInventory) || pInventory == nullptr)
		{
			return false;
		}

		const auto currentEnergy = pInterface->Agent_GetInfo().Energy;

		constexpr float foodMin{ 5.f };

		if (currentEnergy <= foodMin)
		{
			if (IsFoodAvailable(pBlackboard))
			{
				return true;
			}
		}

		return false;
	}

	bool IsInCell(Elite::Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		SteeringPlugin_Output* pSteering{};
		CellSpace* pCellSpace{ nullptr };
		Cell* pCell{ nullptr };

		if (!pBlackboard->GetData("interface", pInterface) || pInterface == nullptr)
		{
			return false;
		}

		if (!pBlackboard->GetData("steering", pSteering) || pSteering == nullptr)
		{
			return false;
		}

		if (!pBlackboard->GetData("gridCells", pCellSpace) || pCellSpace == nullptr)
		{
			return false;
		}

		if (!pBlackboard->GetData("cells", pCell) || pCell == nullptr)
		{
			return false;
		}

		const auto agentInfo = pInterface->Agent_GetInfo();

		auto pathCells = pCellSpace->GetPath();
		auto otherCells = pCellSpace->GetCells();

		for (int i = 0; i < pathCells.size(); ++i)
		{
			if (Elite::Distance(agentInfo.Position, pathCells.at(i).center) <= 20.f)
			{
				pCellSpace->CheckedCellInPath(i);
				return true;
			}
			pInterface->Draw_Circle(pathCells.at(i).center, 20.f, Elite::Vector3{ 0,1,1 });

		}

		return false;

	}

	
}


//-----------------------------------------------------------------
// Behaviors.
//-----------------------------------------------------------------

namespace BT_Behaviors
{

	Elite::BehaviorState FleeFromEnemy(Elite::Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		SteeringPlugin_Output* pSteering{};
		std::vector<EnemyInfo>* pEnemyInfos{ nullptr };
		Timer* pBittenTimer{ nullptr };


		if (!pBlackboard->GetData("steering", pSteering) || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("interface", pInterface) || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("enemiesInFOV", pEnemyInfos) || pEnemyInfos == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}


		if (!pBlackboard->GetData("bittenTimer", pBittenTimer) || pBittenTimer == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		pBittenTimer->ResetTimer();
		pBittenTimer->Disable();

		const auto agentInfo = pInterface->Agent_GetInfo();

		EnemyInfo enemy{};

		for (int i{0}; i < pEnemyInfos->size(); ++i)
		{
			enemy = pEnemyInfos->at(i);
		}

		const Elite::Vector2 fleeDirection = (enemy.Location - agentInfo.Position);

		const auto target = enemy.Location - fleeDirection.GetNormalized() * 20.f;

		const auto nextTargetPos = pInterface->NavMesh_GetClosestPathPoint(target);

		pSteering->AutoOrient = false;
		pSteering->LinearVelocity = nextTargetPos - agentInfo.Position;
		pSteering->LinearVelocity.Normalize();
		pSteering->LinearVelocity *= agentInfo.MaxLinearSpeed;
		pSteering->RunMode = false;

		return Elite::BehaviorState::Success;

	}

	Elite::BehaviorState TurnAround(Elite::Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		SteeringPlugin_Output* pSteering{};
		CellSpace* pCellSpace{ nullptr };
		std::vector<EnemyInfo>* pEnemyInfos{ nullptr };


		if (!pBlackboard->GetData("steering", pSteering) || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("interface", pInterface) || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("enemiesInFOV", pEnemyInfos) || pEnemyInfos == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("gridCells", pCellSpace) || pCellSpace == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		const auto agentInfo = pInterface->Agent_GetInfo();

		EnemyInfo enemy{};

		for (int i{ 0 }; i < pEnemyInfos->size(); ++i)
		{
			enemy = pEnemyInfos->at(i);
		}

		const auto cell = pCellSpace->GetNearestCellInPath(agentInfo.Position);

		const auto target = cell.center;

		const auto nextTargetPos = pInterface->NavMesh_GetClosestPathPoint(target);

		pSteering->LinearVelocity = nextTargetPos - agentInfo.Position;
		pSteering->LinearVelocity.Normalize();
		pSteering->LinearVelocity *= agentInfo.MaxLinearSpeed;

		auto backDirection = -agentInfo.LinearVelocity;
		backDirection.Normalize();

		pSteering->AutoOrient = false;
		const float agentRotation{ agentInfo.Orientation + 0.5f * M_PI };

		const Elite::Vector2 agentDirection{ std::cosf(agentRotation),std::sinf(agentRotation) };
		pSteering->AngularVelocity = (backDirection.Dot(agentDirection)) * agentInfo.MaxAngularSpeed;

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState SeekHouse(Elite::Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		SteeringPlugin_Output* pSteering{};
		std::vector<HouseInfo>* pHouseInfos{ nullptr };
		std::vector<HouseInfo>* pHousesChecked{ nullptr };

		if (!pBlackboard->GetData("steering", pSteering) || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("interface", pInterface) || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("housesInFOV", pHouseInfos) || pHouseInfos == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("housesChecked", pHousesChecked) || pHousesChecked == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		const auto agentInfo = pInterface->Agent_GetInfo();

		HouseInfo house{};
		for (int i{ 0 }; i < pHouseInfos->size(); ++i)
		{
			house = pHouseInfos->at(i);
		}

		for (int i{ 0 }; i < pHousesChecked->size(); ++i)
		{
			if (pHousesChecked->at(i).Center == house.Center)
			{
				return Elite::BehaviorState::Failure;

			}
		}

		const auto target = house.Center;

		auto targetDirection = target - agentInfo.Position;

		const auto nextTargetPos = pInterface->NavMesh_GetClosestPathPoint(target);

		pSteering->AutoOrient = false;
		pSteering->LinearVelocity = nextTargetPos - agentInfo.Position;
		pSteering->LinearVelocity.Normalize();
		pSteering->LinearVelocity *= agentInfo.MaxLinearSpeed;
		pSteering->RunMode = false;

		targetDirection.Normalize();
		const float agentRotation{ agentInfo.Orientation + 0.5f * M_PI };

		const Elite::Vector2 agentDirection{ std::cosf(agentRotation),std::sinf(agentRotation) };
		pSteering->AngularVelocity = (targetDirection.Dot(agentDirection)) * agentInfo.MaxAngularSpeed;

		return Elite::BehaviorState::Running;

	}

	Elite::BehaviorState Wander(Elite::Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		SteeringPlugin_Output* pSteering{};
		Timer* pBittenTimer{ nullptr };


		if (!pBlackboard->GetData("steering", pSteering) || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("interface", pInterface) || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("bittenTimer", pBittenTimer) || pBittenTimer == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		pBittenTimer->ResetTimer();
		pBittenTimer->Disable();

		const auto agentInfo = pInterface->Agent_GetInfo();

		float wanderAngle{ agentInfo.Orientation };
		const float randomAngle{ Elite::ToRadians(static_cast<float>(rand() % static_cast<int>(Elite::ToDegrees(10.f)))) };
		float sign{};
		if (rand() % 2 == 0)
		{
			sign = -1;
		}
		else
		{
			sign = 1;
		}
		const float nextAngle{ wanderAngle + (sign * randomAngle) };
		wanderAngle = nextAngle;

		const Elite::Vector2 circleCenter{
			agentInfo.Position + (Elite::Vector2{
				std::cosf(agentInfo.Orientation), std::sinf(agentInfo.Orientation)
			} * 10.f)
		};
		const Elite::Vector2 target{
			circleCenter.x + (std::cosf(wanderAngle) * 7.f), circleCenter.y + (std::sinf(wanderAngle) * 7.f)
		};
		const Elite::Vector2 targetVector{ target - agentInfo.Position };

		const auto nextTargetPos = pInterface->NavMesh_GetClosestPathPoint(target);

		pSteering->AutoOrient = true;
		pSteering->LinearVelocity = nextTargetPos - agentInfo.Position;
		pSteering->LinearVelocity.Normalize();
		pSteering->LinearVelocity *= agentInfo.MaxLinearSpeed;
		pSteering->RunMode = false;

		pInterface->Draw_Circle(circleCenter, 7.f, Elite::Vector3{ 0,1,0 });
		pInterface->Draw_Direction(agentInfo.Position, targetVector, 10.f, Elite::Vector3{ 0,1,0 });

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState AddToHousesChecked(Elite::Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		std::vector<HouseInfo>* pHouseInfos{ nullptr };
		std::vector<HouseInfo>* pHousesChecked{ nullptr };

		if (!pBlackboard->GetData("housesInFOV", pHouseInfos) || pHouseInfos == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("housesChecked", pHousesChecked) || pHousesChecked == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		for (int i{ 0 }; i < pHouseInfos->size(); ++i)
		{
			if (pHousesChecked->empty())
			{
				pHousesChecked->push_back(pHouseInfos->at(i));

			}

			for (int j{ 0 }; j < pHousesChecked->size(); ++j)
			{
				if (pHouseInfos->at(i).Center != pHousesChecked->at(j).Center)
				{
					pHousesChecked->push_back(pHouseInfos->at(i));
				}
			}

		}

		return Elite::BehaviorState::Success;

	}

	Elite::BehaviorState PickupItemsFOV(Elite::Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		SteeringPlugin_Output* pSteering{};
		std::vector<EntityInfo>* pItemsInfos{ nullptr };
		Inventory* pInventory{ nullptr };

		if (!pBlackboard->GetData("steering", pSteering) || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("interface", pInterface) || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("itemsInFOV", pItemsInfos) || pItemsInfos == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("inventory", pInventory) || pInventory == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		const auto agentInfo = pInterface->Agent_GetInfo();

		EntityInfo item{};
		for (int i{ 0 }; i < pItemsInfos->size(); ++i)
		{
			item = pItemsInfos->at(i);
		}

		const auto target = item.Location;

		const auto nextTargetPos = pInterface->NavMesh_GetClosestPathPoint(target);

		pSteering->AutoOrient = true;
		pSteering->LinearVelocity = nextTargetPos - agentInfo.Position;
		pSteering->LinearVelocity.Normalize();
		pSteering->LinearVelocity *= agentInfo.MaxLinearSpeed;
		pSteering->RunMode = false;

		if ((target - agentInfo.Position).Magnitude() < agentInfo.GrabRange)
		{
			if (!pInventory->PickupItem(item))
			{
				return Elite::BehaviorState::Failure;
			}
		}

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState Shoot(Elite::Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		SteeringPlugin_Output* pSteering{};
		std::vector<EnemyInfo>* pEnemyInfos{ nullptr };
		Inventory* pInventory{ nullptr };

		if (!pBlackboard->GetData("steering", pSteering) || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("interface", pInterface) || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("enemiesInFOV", pEnemyInfos) || pEnemyInfos == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("inventory", pInventory) || pInventory == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		const auto agentInfo = pInterface->Agent_GetInfo();

		EnemyInfo enemy{};
		for (int i{ 0 }; i < pEnemyInfos->size(); ++i)
		{
			enemy = pEnemyInfos->at(i);
		}

		Elite::Vector2 enemyDirection = (enemy.Location - agentInfo.Position);
		enemyDirection.Normalize();
		const float agentRotation{ agentInfo.Orientation + 0.5f * static_cast<float>(M_PI) };

		const auto orientation = std::abs(agentInfo.Orientation - std::atan2(enemyDirection.y, enemyDirection.x));
		const Elite::Vector2 agentDirection{ std::cosf(agentRotation),std::sinf(agentRotation) };
		pSteering->AngularVelocity = (enemyDirection.Dot(agentDirection)) * agentInfo.MaxAngularSpeed;
		pSteering->RunMode = false;

		if (orientation < 0.05f)
		{
			if (pInventory->UseGun())
			{
				return Elite::BehaviorState::Success;
			}
		}

		return Elite::BehaviorState::Running;

	}

	Elite::BehaviorState UseMedKit(Elite::Blackboard* pBlackboard)
	{
		Inventory* pInventory{ nullptr };

		if (!pBlackboard->GetData("inventory", pInventory) || pInventory == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pInventory->UseMedKit())
		{
			return Elite::BehaviorState::Success;
		}

		return Elite::BehaviorState::Failure;
	}

	Elite::BehaviorState UseFood(Elite::Blackboard* pBlackboard)
	{
		Inventory* pInventory{ nullptr };

		if (!pBlackboard->GetData("inventory", pInventory) || pInventory == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pInventory->UseFood())
		{
			return Elite::BehaviorState::Success;
		}

		return Elite::BehaviorState::Failure;
	}

	Elite::BehaviorState FleeFromPurgeZone(Elite::Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		SteeringPlugin_Output* pSteering{};
		std::vector<PurgeZoneInfo>* pPurgeZoneInfos{ nullptr };

		if (!pBlackboard->GetData("steering", pSteering) || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("interface", pInterface) || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("purgeZoneInFOV", pPurgeZoneInfos) || pPurgeZoneInfos == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}


		const auto agentInfo = pInterface->Agent_GetInfo();

		PurgeZoneInfo purgeZone{};

		for (int i{ 0 }; i < pPurgeZoneInfos->size(); ++i)
		{
			purgeZone = pPurgeZoneInfos->at(i);
		}

		const Elite::Vector2 desiredDirection = (purgeZone.Center + agentInfo.Position);

		const auto target = -desiredDirection.GetNormalized() * 20.f;

		const auto nextTargetPos = pInterface->NavMesh_GetClosestPathPoint(target);

		pSteering->AutoOrient = false;
		pSteering->LinearVelocity = nextTargetPos - agentInfo.Position;
		pSteering->LinearVelocity.Normalize();
		pSteering->LinearVelocity *= agentInfo.MaxLinearSpeed;
		pSteering->RunMode = true;

		return Elite::BehaviorState::Success;

	}

	Elite::BehaviorState SeekToNearestCell(Elite::Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		SteeringPlugin_Output* pSteering{};
		CellSpace* pCellSpace{ nullptr };
		Cell* pCell{ nullptr };
		Timer* pBittenTimer{ nullptr };


		if (!pBlackboard->GetData("interface", pInterface) || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("steering", pSteering) || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("gridCells", pCellSpace) || pCellSpace == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("cells", pCell) || pCell == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("bittenTimer", pBittenTimer) || pBittenTimer == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		pBittenTimer->ResetTimer();
		pBittenTimer->Disable();

		const auto agentInfo = pInterface->Agent_GetInfo();

		const auto cell = pCellSpace->GetNearestCellInPath(agentInfo.Position);

		const auto target = cell.center;

		const auto nextTargetPos = pInterface->NavMesh_GetClosestPathPoint(target);

		if (agentInfo.Stamina <= 3.f)
		{
			pSteering->RunMode = false;

		}
		else
		{
			pSteering->RunMode = true;
		}

		pSteering->AutoOrient = true;
		pSteering->LinearVelocity = nextTargetPos - agentInfo.Position;
		pSteering->LinearVelocity.Normalize();
		pSteering->LinearVelocity *= agentInfo.MaxLinearSpeed;

		return Elite::BehaviorState::Success;
	}


}


#endif