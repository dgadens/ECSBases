#pragma once

#include <unordered_map>
#include <memory>
#include "ECS_System.h"
#include "ECS_Component.h"

class SystemManager
{
private:
	// Map from system type pointer to a signature
	std::unordered_map<std::type_index, Signature> m_Signatures;

	// Map from system type pointer to a system pointer
	std::unordered_map<std::type_index, std::shared_ptr<ECS_System>> m_Systems;

public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem(ECS_Orchestrator& orchestrator)
	{
		std::type_index typeIndex(typeid(T));

		// Create a pointer to the system and return it so it can be used externally
		auto system = std::make_shared<T>(orchestrator);
		m_Systems.insert({ typeIndex, system });
		return system;
	}

	template<typename T>
	void UnregisterSystem()
	{
		std::type_index typeIndex(typeid(T));
		m_Systems.erase(typeIndex);
	}

	template<typename T>
	void SetSignature(Signature signature)
	{
		std::type_index typeIndex(typeid(T));
		// Set the signature for this system
		m_Signatures.insert({ typeIndex, signature });
	}

	void EntityDestroyed(ECS_Entity entity)
	{
		// Erase a destroyed entity from all system lists
		// Entities is a set so no check needed
		for (auto const& pair : m_Systems)
		{
			auto const& system = pair.second;

			system->Entities.erase(entity);
		}
	}

	void EntitySignatureChanged(ECS_Entity entity, Signature entitySignature)
	{
		// Notify each system that an entity's signature changed
		for (auto const& pair : m_Systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = m_Signatures[type];

			// Entity signature matches system signature - insert into set
			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->Entities.insert(entity);
			}
			// Entity signature does not match system signature - erase from set
			else
			{
				system->Entities.erase(entity);
			}
		}
	}
};