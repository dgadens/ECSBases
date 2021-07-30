#pragma once

#include <set>
#include "ECS_Entity.h"

class ECS_Orchestrator;
class ECS_System
{
public:
	ECS_System(ECS_Orchestrator* orchestrator)
		: m_Orchestrator(orchestrator),
		Active(true),
		Id(0)
	{}

public:
	std::set<ECS_Entity> Entities;
	bool Active;
	uint32_t Id;

protected:
	ECS_Orchestrator* m_Orchestrator;
};
