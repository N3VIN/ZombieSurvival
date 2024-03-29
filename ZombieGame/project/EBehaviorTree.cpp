//=== General Includes ===
#include "stdafx.h"
#include "EBehaviorTree.h"
using namespace Elite;

//-----------------------------------------------------------------
// BEHAVIOR TREE COMPOSITES (IBehavior)
//-----------------------------------------------------------------
#pragma region COMPOSITES
//SELECTOR
BehaviorState BehaviorSelector::Execute(Blackboard* pBlackBoard)
{
	// Loop over all children in m_ChildBehaviors
	for (const auto& child : m_ChildBehaviors)
	{
		//Every Child: Execute and store the result in m_CurrentState
		m_CurrentState = child->Execute(pBlackBoard);

		//Check the currentstate and apply the selector Logic:
		switch (m_CurrentState)
		{
		case Elite::BehaviorState::Success:

		case Elite::BehaviorState::Running:
			return m_CurrentState;

		case Elite::BehaviorState::Failure:
			continue;
		}
	}

	//All children failed
	m_CurrentState = BehaviorState::Failure;
	return m_CurrentState;
}
//SEQUENCE
BehaviorState BehaviorSequence::Execute(Blackboard* pBlackBoard)
{
	//Loop over all children in m_ChildBehaviors
	for (const auto& child : m_ChildBehaviors)
	{
		//Every Child: Execute and store the result in m_CurrentState
		m_CurrentState = child->Execute(pBlackBoard);

		//Check the currentstate and apply the sequence Logic:
		switch (m_CurrentState)
		{

		case Elite::BehaviorState::Running:

		case Elite::BehaviorState::Failure:
			return m_CurrentState;

		case Elite::BehaviorState::Success:
			break;
		}

	}
	//All children succeeded 
	m_CurrentState = BehaviorState::Success;
	return m_CurrentState;
}
//PARTIAL SEQUENCE
BehaviorState BehaviorPartialSequence::Execute(Blackboard* pBlackBoard)
{
	while (m_CurrentBehaviorIndex < m_ChildBehaviors.size())
	{
		m_CurrentState = m_ChildBehaviors[m_CurrentBehaviorIndex]->Execute(pBlackBoard);
		switch (m_CurrentState)
		{
		case BehaviorState::Failure:
			m_CurrentBehaviorIndex = 0;
			return m_CurrentState;

		case BehaviorState::Success:
			++m_CurrentBehaviorIndex;
			m_CurrentState = BehaviorState::Running;
			return m_CurrentState;

		case BehaviorState::Running:
			return m_CurrentState;
		}
	}

	m_CurrentBehaviorIndex = 0;
	m_CurrentState = BehaviorState::Success;
	return m_CurrentState;
}

//GROUP
BehaviorState Elite::BehaviorGroup::Execute(Blackboard* pBlackBoard)
{
	std::vector<int> amount;
	for (int i{ 0 }; i < 3; ++i) amount.push_back(0);

	for (const auto& child : m_ChildBehaviors)
	{
		m_CurrentState = child->Execute(pBlackBoard);
		++amount[static_cast<int>(m_CurrentState)];
	}
	int maxEl = std::distance(amount.begin(), std::max_element(amount.begin(), amount.end()));
	return static_cast<BehaviorState>(maxEl);
}

#pragma endregion
//-----------------------------------------------------------------
// BEHAVIOR TREE CONDITIONAL (IBehavior)
//-----------------------------------------------------------------
BehaviorState BehaviorConditional::Execute(Blackboard* pBlackBoard)
{
	if (m_fpConditional == nullptr)
		return BehaviorState::Failure;

	switch (m_fpConditional(pBlackBoard))
	{
	case true:
		m_CurrentState = BehaviorState::Success;
		return m_CurrentState;
	case false:
		m_CurrentState = BehaviorState::Failure;
		return m_CurrentState;
	}

}

BehaviorState Elite::BehaviorInvertConditional::Execute(Blackboard* pBlackBoard)
{
	if (m_fpConditional == nullptr)
		return BehaviorState::Failure;

	switch (m_fpConditional(pBlackBoard))
	{
	case true:
		m_CurrentState = BehaviorState::Failure;
		return m_CurrentState;
	case false:
		m_CurrentState = BehaviorState::Success;
		return m_CurrentState;
	}
}

//-----------------------------------------------------------------
// BEHAVIOR TREE ACTION (IBehavior)
//-----------------------------------------------------------------
BehaviorState BehaviorAction::Execute(Blackboard* pBlackBoard)
{
	if (m_fpAction == nullptr)
		return BehaviorState::Failure;

	m_CurrentState = m_fpAction(pBlackBoard);
	return m_CurrentState;
}