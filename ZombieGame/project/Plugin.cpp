#include "stdafx.h"
#include "Plugin.h"
#include "IExamInterface.h"

#include "Behaviors.h"


using namespace std;
using namespace Elite;

//#define DEMO_STEERING


//Called only once, during initialization
void Plugin::Initialize(IBaseInterface* pInterface, PluginInfo& info)
{
	//Retrieving the interface
	//This interface gives you access to certain actions the AI_Framework can perform for you
	m_pInterface = static_cast<IExamInterface*>(pInterface);

	//Bit information about the plugin
	//Please fill this in!!
	info.BotName = "MinionExam";
	info.Student_FirstName = "Nevin";
	info.Student_LastName = "Amarendranath";
	info.Student_Class = "2DAE07";

	//..

	m_pSteeringOutputData = new SteeringPlugin_Output();

	m_pItemsInFOV = new std::vector<EntityInfo>();
	m_pEnemiesInFOV = new std::vector<EnemyInfo>();
	m_pPurgeZoneInFOV = new std::vector<PurgeZoneInfo>();
	m_pHousesInFov = new std::vector<HouseInfo>();
	m_pHousesChecked = new std::vector<HouseInfo>();
	m_pHousesSearch = new std::vector<HouseSearch>();
	m_pInventory = new Inventory(m_pInterface);
	m_pBittenTimer = new Timer(2.5f, false);
	m_pGridCells = new CellSpace(m_pInterface->World_GetInfo().Dimensions.x, m_pInterface->World_GetInfo().Dimensions.y, 14, 14);


	Blackboard* pBlackboard = new Blackboard();
	pBlackboard->AddData("steering", m_pSteeringOutputData);
	pBlackboard->AddData("interface", (m_pInterface));
	pBlackboard->AddData("itemsInFOV", m_pItemsInFOV);
	pBlackboard->AddData("enemiesInFOV", m_pEnemiesInFOV);
	pBlackboard->AddData("purgeZoneInFOV", m_pPurgeZoneInFOV);
	pBlackboard->AddData("housesInFOV", m_pHousesInFov);
	pBlackboard->AddData("housesChecked", m_pHousesChecked);
	pBlackboard->AddData("housesSearch", m_pHousesSearch);
	pBlackboard->AddData("inventory", m_pInventory);
	pBlackboard->AddData("bittenTimer", m_pBittenTimer);
	pBlackboard->AddData("gridCells", m_pGridCells);

	m_pBehaviorTree = new BehaviorTree(pBlackboard, new BehaviorGroup
	(
		{
			new BehaviorSelector
			(
				{
							new BehaviorSequence
						(
							{
								// checks if enemy is in FOV.
							new BehaviorConditional(&BT_Conditions::IsEnemyInView),

								new BehaviorSelector
								(
									{
										new BehaviorSequence
										(
											{
												// If gun is available.
												new BehaviorConditional(&BT_Conditions::IsGunAvailable),
												// Shoot.
												new BehaviorAction(&BT_Behaviors::Shoot)
											}
										),
									// turn around and flee.
									new BehaviorAction(&BT_Behaviors::TurnAround)
								}
							)
							}
					),

						new BehaviorSequence
						(
						{
								// was bitten.
								new BehaviorConditional(&BT_Conditions::IsBitten),
								// turn around and flee.
								new BehaviorAction(&BT_Behaviors::TurnAround)

								}
						),

						new BehaviorSequence
						(
							{
									// checks if item is in FOV.
								new BehaviorConditional(&BT_Conditions::IsItemInView),
									// Pickup items in FOV.
								new BehaviorAction(&BT_Behaviors::PickupItemsFOV)
								}
					),

					new BehaviorSequence
					(
					{
							// checks if house is in FOV.
							new BehaviorConditional(&BT_Conditions::IsHouseInView),
							// seek house.
							//new BehaviorAction(&BT_Behaviors::SeekHouse)
							new BehaviorAction(&BT_Behaviors::SearchHouse)
						}
					),

					//new BehaviorSequence
					//(
					//{
					//			// checks if inside house.
					//			new BehaviorConditional(&BT_Conditions::IsInsideHouse),
					//			new BehaviorAction(&BT_Behaviors::SearchHouse),
					//			// Wander inside house.
					//			//new BehaviorAction(&BT_Behaviors::Wander)
					//			//new BehaviorAction(&BT_Behaviors::ExitHouse)
					//		}
					//	),

					new BehaviorSequence
					(
					{
								// checks if inside house.
								new BehaviorInvertConditional(&BT_Conditions::IsInsideHouse),
								//new BehaviorAction(&BT_Behaviors::SearchHouse),
								// Wander inside house.
								//new BehaviorAction(&BT_Behaviors::Wander)
								new BehaviorAction(&BT_Behaviors::ExitHouse)
							}
						),

					//new BehaviorSequence
					//(
					//{
					//		// checks if house is in FOV.
					//		new BehaviorConditional(&BT_Conditions::IsHouseInView),
					//		// seek house.
					//		new BehaviorAction(&BT_Behaviors::SeekHouse)
					//	}
					//),

					new BehaviorSequence
				(
					{
						// checks if its not in cell.
					new BehaviorInvertConditional(&BT_Conditions::IsInCell),
						new BehaviorSelector
						(
							{
								// Seek to nearest cell.
							new BehaviorAction(&BT_Behaviors::SeekToNearestCell),
								// Wander.
							//new BehaviorAction(&BT_Behaviors::Wander)
							}
						)
					}
				),
			}
		),
			new BehaviorSelector
			(
				{
					new BehaviorSequence
					(
						{
							// check if you need and have Food if available.
							new BehaviorConditional(&BT_Conditions::DoINeedFood),
							// if yes use food.
							new BehaviorAction(&BT_Behaviors::UseFood)
						}
					),

					new BehaviorSequence
					(
						{
							// check if you need and have medkit if available.
							new BehaviorConditional(&BT_Conditions::DoINeedMedKit),
							// if yes use medKit.
							new BehaviorAction(&BT_Behaviors::UseMedKit)
						}
					)
				}
			),

			new BehaviorSelector
			(
				{
					new BehaviorSequence
					(
					{
							// if purgeZone in view.
							new BehaviorConditional(&BT_Conditions::IsPurgeZoneInView),
							// Flee from purgeZone.
							new BehaviorAction(&BT_Behaviors::FleeFromPurgeZone)
							}
						),
				}
			)


		}
	));


}

//Called only once
void Plugin::DllInit()
{
	//Called when the plugin is loaded
}

//Called only once
void Plugin::DllShutdown()
{
	//Called wheb the plugin gets unloaded

	SAFE_DELETE(m_pBehaviorTree);
	SAFE_DELETE(m_pSteeringOutputData);
	SAFE_DELETE(m_pItemsInFOV);
	SAFE_DELETE(m_pEnemiesInFOV);
	SAFE_DELETE(m_pPurgeZoneInFOV);
	SAFE_DELETE(m_pHousesInFov);
	SAFE_DELETE(m_pHousesChecked);
	SAFE_DELETE(m_pHousesSearch);
	SAFE_DELETE(m_pInventory);
	SAFE_DELETE(m_pBittenTimer);
	SAFE_DELETE(m_pGridCells);

}

//Called only once, during initialization
void Plugin::InitGameDebugParams(GameDebugParams& params)
{
	params.AutoFollowCam = true; //Automatically follow the AI? (Default = true)
	params.RenderUI = true; //Render the IMGUI Panel? (Default = true)
	params.SpawnEnemies = true; //Do you want to spawn enemies? (Default = true)
	params.EnemyCount = 1; //How many enemies? (Default = 20)
	params.GodMode = true; //GodMode > You can't die, can be useful to inspect certain behaviors (Default = false)
	params.LevelFile = "GameLevel.gppl";
	params.AutoGrabClosestItem = true; //A call to Item_Grab(...) returns the closest item that can be grabbed. (EntityInfo argument is ignored)
	params.StartingDifficultyStage = 1;
	params.InfiniteStamina = false;
	params.SpawnDebugPistol = true;
	params.SpawnDebugShotgun = true;
	params.SpawnPurgeZonesOnMiddleClick = true;
	params.SpawnZombieOnRightClick = true;
	params.PrintDebugMessages = true;
	params.ShowDebugItemNames = true;
	params.Seed = 36;
	//params.Seed = 12345;
}

//Only Active in DEBUG Mode
//(=Use only for Debug Purposes)
void Plugin::Update(float dt)
{
	//Demo Event Code
	//In the end your AI should be able to walk around without external input
	if (m_pInterface->Input_IsMouseButtonUp(Elite::InputMouseButton::eLeft))
	{
		//Update target based on input
		Elite::MouseData mouseData = m_pInterface->Input_GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eLeft);
		const Elite::Vector2 pos = Elite::Vector2(static_cast<float>(mouseData.X), static_cast<float>(mouseData.Y));
		m_Target = m_pInterface->Debug_ConvertScreenToWorld(pos);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Space))
	{
		//m_CanRun = true;
		m_pGridCells->ResetPath();
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Left))
	{
		m_AngSpeed -= Elite::ToRadians(10);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Right))
	{
		m_AngSpeed += Elite::ToRadians(10);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_G))
	{
		m_GrabItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_U))
	{
		m_UseItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_R))
	{
		m_RemoveItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyUp(Elite::eScancode_Space))
	{
		m_CanRun = false;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Delete))
	{
		m_pInterface->RequestShutdown();
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_KP_Minus))
	{
		if (m_InventorySlot > 0)
			--m_InventorySlot;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_KP_Plus))
	{
		if (m_InventorySlot < 4)
			++m_InventorySlot;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Q))
	{
		ItemInfo info = {};
		m_pInterface->Inventory_GetItem(m_InventorySlot, info);
		std::cout << (int)info.Type << std::endl;
	}
}

//Update
//This function calculates the new SteeringOutput, called once per frame
SteeringPlugin_Output Plugin::UpdateSteering(float dt)
{
	UpdateEntitiesInFOV();
	UpdateHousesInFOV();

	SteeringPlugin_Output* pSteering;

	m_pBehaviorTree->Update(dt);
	m_pBittenTimer->Update(dt);

	if (m_pGridCells->IsAllCellInPathChecked())
	{
		m_pGridCells->ResetPath();
		m_pHousesChecked->clear();
	}

	if (m_pHousesChecked->size() >= 6000)
	{
		m_pHousesChecked->clear();
	}

	m_pBehaviorTree->GetBlackboard()->GetData("steering", pSteering);

	return *pSteering;
}

//This function should only be used for rendering debug elements
void Plugin::Render(float dt) const
{
	//This Render function should only contain calls to Interface->Draw_... functions
	m_pInterface->Draw_SolidCircle(m_Target, .7f, { 0,0 }, { 1, 0, 0 });

	// World grid
	for (const auto& cell : m_pGridCells->GetCells())
	{
		if (!cell.isCellChecked)
		{
			m_pInterface->Draw_Polygon(cell.GetRectPoints().data(), 4, { 0.0f, 0.0f, 1.0f });
		}
		
	}

	for (const auto& cell : m_pGridCells->GetPath())
	{
		if (cell.isCellChecked)
		{
			m_pInterface->Draw_Polygon(cell.GetRectPoints().data(), 4, { 0.f, 1.f, 0.f });
		}
		else
		{
			m_pInterface->Draw_Polygon(cell.GetRectPoints().data(), 4, { 1.f, 0.f, 0.f });
		}
	}

	//m_pInterface->Draw_SolidCircle(m_pInterface->World_GetInfo().Center, 325, { 0,0 }, { 0, 0, 1 });
}

vector<HouseInfo> Plugin::GetHousesInFOV() const
{
	vector<HouseInfo> vHousesInFOV = {};

	HouseInfo hi = {};
	for (int i = 0;; ++i)
	{
		if (m_pInterface->Fov_GetHouseByIndex(i, hi))
		{
			vHousesInFOV.push_back(hi);
			continue;
		}

		break;
	}

	return vHousesInFOV;
}

vector<EntityInfo> Plugin::GetEntitiesInFOV() const
{
	vector<EntityInfo> vEntitiesInFOV = {};

	EntityInfo ei = {};
	for (int i = 0;; ++i)
	{
		if (m_pInterface->Fov_GetEntityByIndex(i, ei))
		{
			vEntitiesInFOV.push_back(ei);
			continue;
		}

		break;
	}

	return vEntitiesInFOV;
}

void Plugin::UpdateEntitiesInFOV() const
{
	const std::vector<EntityInfo> entitesInFOV = GetEntitiesInFOV();

	m_pEnemiesInFOV->clear();
	m_pItemsInFOV->clear();
	m_pPurgeZoneInFOV->clear();

	for(const auto& entity : entitesInFOV)
	{
		if (entity.Type == eEntityType::ENEMY)
		{
			EnemyInfo enemyInfo{};
			m_pInterface->Enemy_GetInfo(entity, enemyInfo);
			m_pEnemiesInFOV->push_back(enemyInfo);
		}
		if(entity.Type == eEntityType::ITEM)
		{
			m_pItemsInFOV->push_back(entity);
		}
		if (entity.Type == eEntityType::PURGEZONE)
		{
			PurgeZoneInfo purgeZoneInfo{};
			m_pInterface->PurgeZone_GetInfo(entity, purgeZoneInfo);
			m_pPurgeZoneInFOV->push_back(purgeZoneInfo);
		}
	}

	m_pBehaviorTree->GetBlackboard()->ChangeData("itemsInFOV", m_pItemsInFOV);
	m_pBehaviorTree->GetBlackboard()->ChangeData("enemiesInFOV", m_pEnemiesInFOV);
	m_pBehaviorTree->GetBlackboard()->ChangeData("purgeZoneInFOV", m_pPurgeZoneInFOV);
}

void Plugin::UpdateHousesInFOV() const
{
	const std::vector<HouseInfo> housesInFOV = GetHousesInFOV();
	m_pHousesInFov->clear();

	if (!housesInFOV.empty())
	{
		m_pHousesInFov->emplace_back(housesInFOV.at(0));
		
		bool found{ false };
		for (const auto& houseSearch : *m_pHousesSearch)
		{
			if (m_pHousesInFov->at(0).Center == houseSearch.Center)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			m_pHousesSearch->emplace_back(HouseSearch{ housesInFOV.at(0) });
		}

		//std::cout << m_pHousesSearch->back().Checked << std::endl;

	}

	m_pBehaviorTree->GetBlackboard()->ChangeData("housesInFOV", m_pHousesInFov);
	m_pBehaviorTree->GetBlackboard()->ChangeData("housesSearch", m_pHousesSearch);
}
