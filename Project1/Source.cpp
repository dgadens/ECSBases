#include "Engine.h"

int main()
{
	auto engine = std::make_unique<Engine>();
	engine->Initialize();
	engine->Run();
	return 0;
}

//#include <chrono>
//#include <sstream>
//#include <iostream>
//#include "ECS_orchestrator.h"
//
//uint32_t positionId;
//uint32_t uniforMoveId;
//uint32_t acceleratedId;
//std::unique_ptr<ECS_Orchestrator> orchestrator;
//
////define get components by id
//#define GET_POSITION_COMPONENT(entity)			orchestrator->GetComponent<PositionComponent>(entity, positionId);
//#define GET_UNIFORMOVE_COMPONENT(entity)		orchestrator->GetComponent<UniformMoveComponent>(entity, uniforMoveId);
//#define GET_ACCELERATEDMOVE_COMPONENT(entity)	orchestrator->GetComponent<AcceleratedMoveComponent>(entity, acceleratedId);
//
////
////Systems
////
//class UniformMoveSystem : public ECS_System
//{
//public:
//	UniformMoveSystem(ECS_Orchestrator* orchestrator) :
//		ECS_System(orchestrator)
//	{}
//
//	void Update(float dt)
//	{
//		for (auto entity : Entities)
//		{
//			auto& position = GET_POSITION_COMPONENT(entity);
//			auto& move = GET_UNIFORMOVE_COMPONENT(entity);
//
//			position.x += move.x * dt;
//			position.y += move.y * dt;
//		}
//	}
//};
//
//class AcceleratedMoveSystem : public ECS_System
//{
//public:
//	AcceleratedMoveSystem(ECS_Orchestrator* orchestrator) :
//		ECS_System(orchestrator)
//	{}
//
//	void Update(float dt)
//	{
//		for (auto entity : Entities)
//		{
//			auto& position = GET_POSITION_COMPONENT(entity);
//			auto& move = GET_ACCELERATEDMOVE_COMPONENT(entity);
//
//			float v = dt + move.accel * (dt * dt) / 2.0f;
//			position.x += move.x * v;
//			position.y += move.y * v;
//		}
//	}
//};
//
//std::string ToString(double value)
//{
//	std::stringstream ss;
//	ss << value;
//	return ss.str();
//};
//
////
////Main
////
//int main()
//{
//	//create orchestrator
//	orchestrator = std::make_unique<ECS_Orchestrator>();
//
//	//register components
//	positionId = orchestrator->RegisterComponent<PositionComponent>();
//	uniforMoveId = orchestrator->RegisterComponent<UniformMoveComponent>();
//	acceleratedId = orchestrator->RegisterComponent<AcceleratedMoveComponent>();
//
//	//register systems
//	auto uniformMoveSystem = orchestrator->RegisterSystem<UniformMoveSystem>();
//	auto acceleratedMoveSystem = orchestrator->RegisterSystem<AcceleratedMoveSystem>();
//
//	//signature for UniformMoveSystem
//	Signature uniformMoveSignature;
//	uniformMoveSignature.set(positionId);
//	uniformMoveSignature.set(uniforMoveId);
//	orchestrator->SetSystemSignature<UniformMoveSystem>(uniformMoveSignature, uniformMoveSystem->Id);
//
//	//signature for AcceleratedMoveSystem
//	Signature acceleratedMoveSignature;
//	acceleratedMoveSignature.set(positionId);
//	acceleratedMoveSignature.set(acceleratedId);
//	orchestrator->SetSystemSignature<AcceleratedMoveSystem>(acceleratedMoveSignature, acceleratedMoveSystem->Id);
//
//	//create entities
//	for (size_t i = 0; i < 2000; i++)
//	{
//		ECS_Entity newEntity = orchestrator->CreateEntity();
//		PositionComponent e1p;
//		UniformMoveComponent uniformMove;
//		orchestrator->AddComponent(newEntity, e1p, positionId);
//		orchestrator->AddComponent(newEntity, uniformMove, uniforMoveId);
//	}
//
//	for (size_t i = 0; i < 2000; i++)
//	{
//		ECS_Entity newEntity = orchestrator->CreateEntity();
//		PositionComponent e2p;
//		AcceleratedMoveComponent acceleratedMove;
//		orchestrator->AddComponent<PositionComponent>(newEntity, e2p, positionId);
//		orchestrator->AddComponent<AcceleratedMoveComponent>(newEntity, acceleratedMove, acceleratedId);
//	}
//
//	auto sequentialTime = std::chrono::steady_clock::now();
//
//	//run systems
//	uniformMoveSystem->Update(1.0f);
//	acceleratedMoveSystem->Update(1.0f);
//
//	std::chrono::duration<double> frameTime2 = std::chrono::steady_clock::now() - sequentialTime;
//	std::cout << "Sequencial Time " + ToString(frameTime2.count()) + " s" << std::endl;
//
//	int a;
//	std::cin >> a;
//	return 0;
//}