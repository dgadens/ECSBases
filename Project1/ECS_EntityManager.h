#pragma once

#include <stack>
#include <array>
#include "ECS_Entity.h"

class EntityManager
{
	friend class ECS_Orchestrator;

private:
	//stack of unused entity IDs, stack beacuse this is Command 
	//Pattern Compatible container
	std::stack<ECS_Entity> m_AvailableEntities;

	//Array of signatures where the index corresponds to the entity ID
	std::array<Signature, MAX_ENTITIES> m_Signatures;

	//Total living entities - used to keep limits on how many exist
	uint32_t m_LivingEntityCount;

public:
	EntityManager()
	{
		m_LivingEntityCount = 0;

		//Initialize stack with all possible entity IDs
		ECS_Entity entity = MAX_ENTITIES;
		do
		{
			entity--;
			m_AvailableEntities.push(entity);
		} while (entity > 0);
	}

	inline ECS_Entity CreateEntity()
	{
		ECS_Entity id = m_AvailableEntities.top();
		m_AvailableEntities.pop();
		++m_LivingEntityCount;

		return id;
	}

	inline void DestroyEntity(ECS_Entity entity)
	{
		// Invalidate the destroyed entity's signature
		m_Signatures[entity].reset();
		m_AvailableEntities.push(entity);
		--m_LivingEntityCount;
	}

	inline void SetSignature(ECS_Entity entity, Signature signature)
	{
		m_Signatures[entity] = signature;
	}

	inline Signature GetSignature(ECS_Entity entity) const
	{
		return m_Signatures[entity];
	}
};