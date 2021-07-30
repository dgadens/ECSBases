#include <chrono>
#include <sstream>
#include <iostream>
#include "ECS_orchestrator.h"

static uint32_t positionId;
static uint32_t uniforMoveId;
static uint32_t acceleratedId;

//
//Systems
//
class UniformMoveSystem : public ECS_System
{
public:
	UniformMoveSystem(ECS_Orchestrator* orchestrator) :
		ECS_System(orchestrator)
	{}

	void Update(float dt);
};

class AcceleratedMoveSystem : public ECS_System
{
public:
	AcceleratedMoveSystem(ECS_Orchestrator* orchestrator) :
		ECS_System(orchestrator)
	{}

	void Update(float dt);
};

//
//Engine
//
class Engine
{
	std::unique_ptr<ECS_Orchestrator> orchestrator;

	//register systems
	std::shared_ptr<UniformMoveSystem> uniformMoveSystem;
	std::shared_ptr<AcceleratedMoveSystem> acceleratedMoveSystem;

	std::string ToString(double value);

public:
	Engine();
	void Initialize();
	void Run();
};