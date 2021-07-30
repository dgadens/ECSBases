#include "Engine.h"

//define get components by id
#define GET_POSITION_COMPONENT(entity)			m_Orchestrator->GetComponent<PositionComponent>(entity, positionId);
#define GET_UNIFORMOVE_COMPONENT(entity)		m_Orchestrator->GetComponent<UniformMoveComponent>(entity, uniforMoveId);
#define GET_ACCELERATEDMOVE_COMPONENT(entity)	m_Orchestrator->GetComponent<AcceleratedMoveComponent>(entity, acceleratedId);

//
//Systems
//
void UniformMoveSystem::Update(float dt)
{
	for (auto entity : Entities)
	{
		auto& position = GET_POSITION_COMPONENT(entity);
		auto& move = GET_UNIFORMOVE_COMPONENT(entity);

		position.x += move.x * dt;
		position.y += move.y * dt;
	}
}

void AcceleratedMoveSystem::Update(float dt)
{
	for (auto entity : Entities)
	{
		auto& position = GET_POSITION_COMPONENT(entity);
		auto& move = GET_ACCELERATEDMOVE_COMPONENT(entity);

		float v =  dt + move.accel * (dt * dt) / 2.0f;
		position.x += move.x * v;
		position.y += move.y * v;
	}
}

//
//Engine
//
Engine::Engine()
{}

std::string Engine::ToString(double value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
};

void Engine::Initialize()
{
	//create orchestrator
	orchestrator = std::make_unique<ECS_Orchestrator>();

	//register components
	positionId = orchestrator->RegisterComponent<PositionComponent>();
	uniforMoveId = orchestrator->RegisterComponent<UniformMoveComponent>();
	acceleratedId = orchestrator->RegisterComponent<AcceleratedMoveComponent>();

	//register systems
	uniformMoveSystem = orchestrator->RegisterSystem<UniformMoveSystem>();
	acceleratedMoveSystem = orchestrator->RegisterSystem<AcceleratedMoveSystem>();

	//signature for UniformMoveSystem
	Signature uniformMoveSignature;
	uniformMoveSignature.set(positionId);
	uniformMoveSignature.set(uniforMoveId);
	orchestrator->SetSystemSignature<UniformMoveSystem>(uniformMoveSignature, uniformMoveSystem->Id);

	//signature for AcceleratedMoveSystem
	Signature acceleratedMoveSignature;
	acceleratedMoveSignature.set(positionId);
	acceleratedMoveSignature.set(acceleratedId);
	orchestrator->SetSystemSignature<AcceleratedMoveSystem>(acceleratedMoveSignature, acceleratedMoveSystem->Id);

	//create entities
	for (size_t i = 0; i < 2000; i++)
	{
		ECS_Entity newEntity = orchestrator->CreateEntity();
		PositionComponent e1p;
		UniformMoveComponent uniformMove;
		orchestrator->AddComponent(newEntity, e1p, positionId);
		orchestrator->AddComponent(newEntity, uniformMove, uniforMoveId);
	}

	for (size_t i = 0; i < 2000; i++)
	{
		ECS_Entity newEntity = orchestrator->CreateEntity();
		PositionComponent e2p;
		AcceleratedMoveComponent acceleratedMove;
		orchestrator->AddComponent<PositionComponent>(newEntity, e2p, positionId);
		orchestrator->AddComponent<AcceleratedMoveComponent>(newEntity, acceleratedMove, acceleratedId);
	}
}

void Engine::Run()
{
	//execute systems
	int a = 0;
	while (a < 10)
	{
		auto sequentialTime = std::chrono::steady_clock::now();

		for (size_t i = 0; i < 10000; i++)
		{
			uniformMoveSystem->Update(1.0f);
			acceleratedMoveSystem->Update(1.0f);
		}

		std::chrono::duration<double> frameTime2 = std::chrono::steady_clock::now() - sequentialTime;
		std::cout << "Sequencial Time " + ToString(frameTime2.count()) + " s" << std::endl;

		a++;
	}
}