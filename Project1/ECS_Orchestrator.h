#pragma once

#include "ECS_EntityManager.h"
#include "ECS_ComponentManager.h"
#include "ECS_SystemManager.h"

class ECS_Orchestrator
{
private:
	std::unique_ptr<ComponentManager> m_ComponentManager;
	std::unique_ptr<EntityManager> m_EntityManager;
	std::unique_ptr<SystemManager> m_SystemManager;

public:
	ECS_Orchestrator()
	{
		// Create pointers to each manager
		m_ComponentManager = std::make_unique<ComponentManager>();
		m_EntityManager = std::make_unique<EntityManager>();
		m_SystemManager = std::make_unique<SystemManager>();
	}

	// Entity methods
	ECS_Entity CreateEntity()
	{
		return m_EntityManager->CreateEntity();
	}

	ECS_Entity CloneEntity(ECS_Entity entity)
	{
		auto clone = CreateEntity();
		CloneComponents(entity, clone);
		return clone;
	}

	void DestroyEntity(ECS_Entity entity)
	{
		m_EntityManager->DestroyEntity(entity);
		m_ComponentManager->EntityDestroyed(entity);
		m_SystemManager->EntityDestroyed(entity);
	}

	void DestroyAllEntities()
	{
		for (ECS_Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			//if entity have some signature is a value entity
			if (m_EntityManager->m_Signatures[entity] != 0)
				DestroyEntity(entity);
		}
	}

	// Component methods
	template<typename T>
	uint32_t RegisterComponent()
	{
		return m_ComponentManager->RegisterComponent<T>();
	}

	template<typename T>
	void AddComponent(ECS_Entity entity, T& component, uint32_t id)
	{
		//if already contains component don't make nothing
		if (!ContainsComponent<T>(entity, id))
		{
			m_ComponentManager->AddComponent<T>(entity, component, id);

			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(id, true);
			m_EntityManager->SetSignature(entity, signature);
			//update systems
			m_SystemManager->EntitySignatureChanged(entity, signature);
		}
		else
		{
			//if already exist only set values
			m_ComponentManager->GetComponent<T>(entity, id) = component;
		}
	}

	template<typename T>
	bool ContainsComponent(ECS_Entity entity, uint32_t id)
	{
		auto signature = m_EntityManager->GetSignature(entity);
		return signature[id];
	}

	template<typename T>
	void RemoveComponent(ECS_Entity entity, uint32_t id)
	{
		if (ContainsComponent<T>(entity, id))
		{
			m_ComponentManager->RemoveComponent<T>(entity);

			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(id, false);
			m_EntityManager->SetSignature(entity, signature);
			//update systems
			m_SystemManager->EntitySignatureChanged(entity, signature);
		}
	}

	void CloneComponents(ECS_Entity sourceEntity, ECS_Entity cloneEntity)
	{
		//clone entity data
		m_ComponentManager->CloneEntity(sourceEntity, cloneEntity);
		//clone entity signature
		auto signature = m_EntityManager->GetSignature(sourceEntity);
		m_EntityManager->SetSignature(cloneEntity, signature);
		//update systems
		m_SystemManager->EntitySignatureChanged(cloneEntity, signature);
	}

	template<typename T>
	T& GetComponent(ECS_Entity entity, uint32_t id)
	{
		return m_ComponentManager->GetComponent<T>(entity, id);
	}

	// System methods
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return m_SystemManager->RegisterSystem<T>(this);
	}

	template<typename T>
	void SetSystemSignature(Signature signature, uint32_t systemId)
	{
		m_SystemManager->SetSignature<T>(signature, systemId);
	}
};