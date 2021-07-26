#include "ECS_Orchestrator.h"

class UniformMoveSystem : public ECS_System
{
public:
	UniformMoveSystem(ECS_Orchestrator& orchestrator) :
		ECS_System(orchestrator)
	{}

	void Update(float dt)
	{
		for (auto entity : Entities)
		{
			auto& position = m_Orchestrator.GetComponent<PositionComponent>(entity);
			auto& move = m_Orchestrator.GetComponent<UniformMoveComponent>(entity);

			position.x += move.x * dt;
			position.y += move.y * dt;
		}
	}
};

class AcceleratedMoveSystem : public ECS_System
{
public:
	AcceleratedMoveSystem(ECS_Orchestrator& orchestrator) :
		ECS_System(orchestrator)
	{}

	void Update(float dt)
	{
		for (auto entity : Entities)
		{
			auto& position = m_Orchestrator.GetComponent<PositionComponent>(entity);
			auto& move = m_Orchestrator.GetComponent<AcceleratedMoveComponent>(entity);

			float v = dt + move.accel * (dt * dt) / 2.0f;
			position.x += move.x * v;
			position.y += move.y * v;
		}
	}
};


int main()
{
	//create orchestrator
	ECS_Orchestrator orchestrator;
	orchestrator.Init();
	
	//register components
	orchestrator.RegisterComponent<PositionComponent>();
	orchestrator.RegisterComponent<UniformMoveComponent>();
	orchestrator.RegisterComponent<AcceleratedMoveComponent>();

	//register systems
	auto uniformMoveSystem = orchestrator.RegisterSystem<UniformMoveSystem>();
	auto acceleratedMoveSystem = orchestrator.RegisterSystem<AcceleratedMoveSystem>();

	//signature for UniformMoveSystem
	Signature uniformMoveSignature;
	uniformMoveSignature.set(orchestrator.GetComponentType<PositionComponent>());
	uniformMoveSignature.set(orchestrator.GetComponentType<UniformMoveComponent>());
	orchestrator.SetSystemSignature<UniformMoveSystem>(uniformMoveSignature);

	//signature for AcceleratedMoveSystem
	Signature acceleratedMoveSignature;
	acceleratedMoveSignature.set(orchestrator.GetComponentType<PositionComponent>());
	acceleratedMoveSignature.set(orchestrator.GetComponentType<AcceleratedMoveComponent>());
	orchestrator.SetSystemSignature<AcceleratedMoveSystem>(acceleratedMoveSignature);

	//create entities
	for (size_t i = 0; i < 2000; i++)
	{
		ECS_Entity newEntity = orchestrator.CreateEntity();
		PositionComponent e1p;
		UniformMoveComponent uniformMove;
		orchestrator.AddComponent(newEntity, e1p);
		orchestrator.AddComponent(newEntity, uniformMove);
	}

	for (size_t i = 0; i < 2000; i++)
	{
		ECS_Entity newEntity = orchestrator.CreateEntity();
		PositionComponent e2p;
		AcceleratedMoveComponent acceleratedMove;
		orchestrator.AddComponent<PositionComponent>(newEntity, e2p);
		orchestrator.AddComponent<AcceleratedMoveComponent>(newEntity, acceleratedMove);
	}

	//execute systems
	for (size_t i = 0; i < 1000; i++)
	{
		uniformMoveSystem->Update(1.0f);
		acceleratedMoveSystem->Update(1.0f);
	}

	//destroy 
	orchestrator.Term();

	return 0;
}
