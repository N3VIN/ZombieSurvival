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


//-----------------------------------------------------------------
// Behaviors.
//-----------------------------------------------------------------

namespace BT_Behaviors
{

	Elite::BehaviorState Wander(Elite::Blackboard* pBlackboard)
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
		Elite::Vector2 target{ circleCenter.x + (std::cosf(wanderAngle) * 6.f),circleCenter.y + (std::sinf(wanderAngle) * 6.f) };
		Elite::Vector2 targetVector{ target - agentInfo.Position };

		auto nextTargetPos = pInterface->NavMesh_GetClosestPathPoint(target);

		pSteering->AutoOrient = true;
		pSteering->LinearVelocity = nextTargetPos - agentInfo.Position;
		pSteering->LinearVelocity.Normalize();	  
		pSteering->LinearVelocity *= agentInfo.MaxLinearSpeed;

		pInterface->Draw_Circle(circleCenter, 6.f, Elite::Vector3{ 0,1,0 });
		pInterface->Draw_Direction(agentInfo.Position, targetVector, 10.f, Elite::Vector3{ 0,1,0 });

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState FleeFromEnemy(Elite::Blackboard* pBlackboard)
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

		for (int i{0}; i < pEnemyInfos->size(); ++i)
		{
			enemy = pEnemyInfos->at(i);
		}

		Elite::Vector2 desiredDirection = (enemy.Location - agentInfo.Position);

		auto target = enemy.Location - desiredDirection.GetNormalized() * 20;

		auto nextTargetPos = pInterface->NavMesh_GetClosestPathPoint(target);

		pSteering->AutoOrient = false;
		pSteering->LinearVelocity = nextTargetPos - agentInfo.Position;
		pSteering->LinearVelocity.Normalize();
		pSteering->LinearVelocity *= agentInfo.MaxLinearSpeed;

		return Elite::BehaviorState::Success;

	}

}

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
			return false;
		}

		return false;
	}


}








#endif