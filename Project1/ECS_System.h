#pragma once

#include <set>
#include <typeindex>
#include "ECS_Entity.h"

class ECS_Orchestrator;
class ECS_System
{
public:
	ECS_System(ECS_Orchestrator& orchestrator)
		: m_Orchestrator(orchestrator),
		Active(true)
	{}

public:
	std::set<ECS_Entity> Entities;
	bool Active;

protected:
	ECS_Orchestrator& m_Orchestrator;
};
