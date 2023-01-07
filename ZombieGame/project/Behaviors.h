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

		/*if (agentInfo.Bitten)
		{
			return true;
		}*/

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
			//for (int j{ 0 }; j < pHousesChecked->size(); ++j)
			{
				//if (pHouseInfos->at(i).Center != pHousesChecked->at(j).Center)
				{
					house = pHouseInfos->at(i);
				}
			}
		}
		
		if (Elite::Distance(agentInfo.Position, house.Center) < 1.5f)
		{
			return true;
		}

		return false;
		//return pInterface->Agent_GetInfo().IsInHouse;
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

		auto currentHealth = pInterface->Agent_GetInfo().Health;

		float medKitMin{ 6.f };

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

		auto currentEnergy = pInterface->Agent_GetInfo().Energy;

		float foodMin{ 5.f };

		if (currentEnergy <= foodMin)
		{
			if (IsFoodAvailable(pBlackboard))
			{
				return true;
			}
		}

		return false;
	}

	bool IsOutsideRange(Elite::Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };

		if (!pBlackboard->GetData("interface", pInterface) || pInterface == nullptr)
		{
			return false;
		}

		auto agentInfo = pInterface->Agent_GetInfo();
		auto worldInfo = pInterface->World_GetInfo();

		auto agentPos = agentInfo.Position;
		auto worldPos = worldInfo.Center;

		float boundRange{ 350.f };

		if (Elite::DistanceSquared(agentPos, worldPos) > boundRange * boundRange)
		{
			return true;
		}

		return false;
	}

}


//-----------------------------------------------------------------
// Behaviors.
//-----------------------------------------------------------------

namespace BT_Behaviors
{

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

		auto agentInfo = pInterface->Agent_GetInfo();

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

		Elite::Vector2 circleCenter{ agentInfo.Position + (Elite::Vector2 {std::cosf(agentInfo.Orientation), std::sinf(agentInfo.Orientation) } *10.f) };
		Elite::Vector2 target{ circleCenter.x + (std::cosf(wanderAngle) * 7.f),circleCenter.y + (std::sinf(wanderAngle) * 7.f) };
		Elite::Vector2 targetVector{ target - agentInfo.Position };

		auto nextTargetPos = pInterface->NavMesh_GetClosestPathPoint(target);

		pSteering->AutoOrient = true;
		pSteering->LinearVelocity = nextTargetPos - agentInfo.Position;
		pSteering->LinearVelocity.Normalize();	  
		pSteering->LinearVelocity *= agentInfo.MaxLinearSpeed;
		pSteering->RunMode = false;

		pInterface->Draw_Circle(circleCenter, 7.f, Elite::Vector3{ 0,1,0 });
		pInterface->Draw_Direction(agentInfo.Position, targetVector, 10.f, Elite::Vector3{ 0,1,0 });

		return Elite::BehaviorState::Success;
	}

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

		auto agentInfo = pInterface->Agent_GetInfo();

		EnemyInfo enemy{};

		for (int i{0}; i < pEnemyInfos->size(); ++i)
		{
			enemy = pEnemyInfos->at(i);
		}

		Elite::Vector2 desiredDirection = (enemy.Location - agentInfo.Position);

		//auto target = enemy.Location - desiredDirection.GetNormalized();
		auto target = enemy.Location - desiredDirection.GetNormalized() * 20.f;
		//auto target = enemy.Location - agentInfo.Position;

		auto nextTargetPos = pInterface->NavMesh_GetClosestPathPoint(target);

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

		auto agentInfo = pInterface->Agent_GetInfo();

		EnemyInfo enemy{};

		for (int i{ 0 }; i < pEnemyInfos->size(); ++i)
		{
			enemy = pEnemyInfos->at(i);
		}

		auto target = agentInfo.LinearVelocity.GetNormalized();

		pSteering->LinearVelocity = target + agentInfo.Position;
		pSteering->LinearVelocity.Normalize();
		pSteering->LinearVelocity *= agentInfo.MaxLinearSpeed;

		pSteering->AutoOrient = false;
		//pSteering->RunMode = true;
		pSteering->AngularVelocity = pInterface->Agent_GetInfo().MaxAngularSpeed;



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

		auto agentInfo = pInterface->Agent_GetInfo();

		HouseInfo house{};
		for (int i{ 0 }; i < pHouseInfos->size(); ++i)
		{
			//for (int j{ 0 }; j < pHousesChecked->size(); ++j)
			{
				//if (pHouseInfos->at(i).Center != pHousesChecked->at(j).Center)
				{
					house = pHouseInfos->at(i);
				}
			}
		}
		for (int i{ 0 }; i < pHousesChecked->size(); ++i)
		{
			if (pHousesChecked->at(i).Center == house.Center)
			{
				return Elite::BehaviorState::Failure;

			}
		}
		

		//auto target = pHouseInfos->at(0).Center;

		//auto target = pHouseInfos->begin()->Center;

		auto target = house.Center;

		auto targetDirection = target - agentInfo.Position;

		auto nextTargetPos = pInterface->NavMesh_GetClosestPathPoint(target);

		pSteering->AutoOrient = false;
		pSteering->LinearVelocity = nextTargetPos - agentInfo.Position;
		pSteering->LinearVelocity.Normalize();
		pSteering->LinearVelocity *= agentInfo.MaxLinearSpeed;
		pSteering->RunMode = false;

		targetDirection.Normalize();
		const float agentRot{ agentInfo.Orientation + 0.5f * static_cast<float>(M_PI) };
		Elite::Vector2 agentDirection{ std::cosf(agentRot),std::sinf(agentRot) };
		pSteering->AngularVelocity = (targetDirection.Dot(agentDirection)) * agentInfo.MaxAngularSpeed;

		return Elite::BehaviorState::Success;

	}

	Elite::BehaviorState ExitHouse(Elite::Blackboard* pBlackboard)
	{
		std::cout << "Exit house rn \n";

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

		/*for (int i{ 0 }; i < pHouseInfos->size(); ++i)
		{
			pHousesChecked->push_back(pHouseInfos->at(i));
		}*/

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

		auto agentInfo = pInterface->Agent_GetInfo();

		EntityInfo item{};
		for (int i{ 0 }; i < pItemsInfos->size(); ++i)
		{
			item = pItemsInfos->at(i);
		}

		auto target = item.Location;

		auto nextTargetPos = pInterface->NavMesh_GetClosestPathPoint(target);

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

		auto agentInfo = pInterface->Agent_GetInfo();

		EnemyInfo enemy{};
		for (int i{ 0 }; i < pEnemyInfos->size(); ++i)
		{
			enemy = pEnemyInfos->at(i);
		}

		Elite::Vector2 enemyDirection = (enemy.Location - agentInfo.Position);

		if (std::abs(agentInfo.Orientation - std::atan2(enemyDirection.y, enemyDirection.x)) < 0.05f)
		{
			// If we're oriented to the closest enemy, shoot it
			if (pInventory->UseGun())
			{
				return Elite::BehaviorState::Success;
			}
			return Elite::BehaviorState::Failure;
		}

		enemyDirection.Normalize();
		const float agentRot{ agentInfo.Orientation + 0.5f * static_cast<float>(M_PI) };
		Elite::Vector2 agentDirection{ std::cosf(agentRot),std::sinf(agentRot) };
		pSteering->AngularVelocity = (enemyDirection.Dot(agentDirection)) * agentInfo.MaxAngularSpeed;
		pSteering->RunMode = false;

		return Elite::BehaviorState::Success;

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

	Elite::BehaviorState MoveToCenter(Elite::Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		SteeringPlugin_Output* pSteering{};

		if (!pBlackboard->GetData("steering", pSteering) || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("interface", pInterface) || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		auto agentInfo = pInterface->Agent_GetInfo();
		auto worldInfo = pInterface->World_GetInfo();

		auto target = worldInfo.Center;

		auto nextTargetPos = pInterface->NavMesh_GetClosestPathPoint(target);

		pSteering->AutoOrient = false;
		pSteering->LinearVelocity = nextTargetPos - agentInfo.Position;
		pSteering->LinearVelocity.Normalize();
		pSteering->LinearVelocity *= agentInfo.MaxLinearSpeed;
		pSteering->RunMode = false;

		return Elite::BehaviorState::Success;

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


		auto agentInfo = pInterface->Agent_GetInfo();

		PurgeZoneInfo purgeZone{};

		for (int i{ 0 }; i < pPurgeZoneInfos->size(); ++i)
		{
			purgeZone = pPurgeZoneInfos->at(i);
		}

		//Elite::Vector2 desiredDirection = (purgeZone.Center - agentInfo.Position);

		//auto target = purgeZone.Center - desiredDirection.GetNormalized() * 20.f;
		auto target = Elite::Vector2 { purgeZone.Center.x + purgeZone.Radius, purgeZone.Center.y + purgeZone.Radius };

		auto nextTargetPos = pInterface->NavMesh_GetClosestPathPoint(target);

		pSteering->AutoOrient = false;
		pSteering->LinearVelocity = nextTargetPos - agentInfo.Position;
		pSteering->LinearVelocity.Normalize();
		pSteering->LinearVelocity *= agentInfo.MaxLinearSpeed;
		pSteering->RunMode = true;

		return Elite::BehaviorState::Success;

	}

	Elite::BehaviorState CheckRightSideHouse(Elite::Blackboard* pBlackboard)
	{
		IExamInterface* pInterface{ nullptr };
		SteeringPlugin_Output* pSteering{};
		std::vector<HouseInfo>* pHouseInfos{ nullptr };

		if (!pBlackboard->GetData("interface", pInterface) || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("steering", pSteering) || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackboard->GetData("housesInFOV", pHouseInfos) || pHouseInfos == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		auto agentInfo = pInterface->Agent_GetInfo();

		HouseInfo house{};
		for (int i{ 0 }; i < pHouseInfos->size(); ++i)
		{
			{
				house = pHouseInfos->at(i);
			}
		}

		Elite::Vector2 target = house.Center + Elite::Vector2{ house.Size.x, 0.f };

		auto nextTargetPos = pInterface->NavMesh_GetClosestPathPoint(target);

		

		pSteering->AutoOrient = false;
		pSteering->LinearVelocity = nextTargetPos - agentInfo.Position;
		pSteering->LinearVelocity.Normalize();
		pSteering->LinearVelocity *= agentInfo.MaxLinearSpeed;
		pSteering->RunMode = false;

		pInterface->Draw_Circle(target, 7.f, Elite::Vector3{ 0,1,0 });

		if (Elite::Distance(agentInfo.Position, target) > 1.5f)
		{
			return Elite::BehaviorState::Failure;
		}

		return Elite::BehaviorState::Success;
	}

}


#endif