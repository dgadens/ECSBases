#pragma once

#include <unordered_map>
#include <memory>
#include "ECS_System.h"
#include "ECS_Component.h"

class SystemManager
{
private:
	// Map from system type pointer to a signature
	std::vector<Signature> m_Signatures;

	// Map from system type pointer to a system pointer
	std::vector<std::shared_ptr<ECS_System>> m_Systems;

public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem(ECS_Orchestrator* orchestrator)
	{
		// Create a pointer to the system and return it so it 
		//can be used externally
		auto system = std::make_shared<T>(orchestrator);
		m_Systems.push_back(system);
		m_Signatures.push_back(Signature());
		auto id = m_Systems.size() - 1;
		((ECS_System*)system.get())->Id = id;
		return system;
	}

	template<typename T>
	void SetSignature(Signature signature, uint32_t id)
	{
		// Set the signature for this system
		m_Signatures[id] = signature;
	}

	void EntityDestroyed(ECS_Entity entity)
	{
		// Erase a destroyed entity from all system lists
		// Entities is a set so no check needed
		for (auto const& s : m_Systems)
		{
			s->Entities.erase(entity);
		}
	}

	void EntitySignatureChanged(ECS_Entity entity, Signature entitySignature)
	{
		// Notify each system that an entity's signature changed
		for (auto const& s : m_Systems)
		{
			auto const& systemSignature = m_Signatures[s->Id];

			// Entity signature matches system signature - insert into set
			if ((entitySignature & systemSignature) == systemSignature)
			{
				s->Entities.insert(entity);
			}
			// Entity signature does not match system signature - erase from set
			else
			{
				s->Entities.erase(entity);
			}
		}
	}
};

