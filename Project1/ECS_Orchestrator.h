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
	void Init()
	{
		// Create pointers to each manager
		m_ComponentManager = std::make_unique<ComponentManager>();
		m_EntityManager = std::make_unique<EntityManager>();
		m_SystemManager = std::make_unique<SystemManager>();
	}

	void Term()
	{
		m_SystemManager = nullptr;
		m_EntityManager = nullptr;
		m_ComponentManager = nullptr;
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
	void RegisterComponent()
	{
		m_ComponentManager->RegisterComponent<T>();
	}

	template<typename T>
	void AddComponent(ECS_Entity entity, T& component)
	{
		//if already contains component don't make nothing
		if (!ContainsComponent<T>(entity))
		{
			m_ComponentManager->AddComponent<T>(entity, component);

			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentType<T>(), true);
			m_EntityManager->SetSignature(entity, signature);
			//update systems
			m_SystemManager->EntitySignatureChanged(entity, signature);
		}
		else
		{
			//if already exist only set values
			m_ComponentManager->GetComponent<T>(entity) = component;
		}
	}

	template<typename T>
	bool ContainsComponent(ECS_Entity entity)
	{
		auto signature = m_EntityManager->GetSignature(entity);
		return signature[m_ComponentManager->GetComponentType<T>()];
	}

	template<typename T>
	void RemoveComponent(ECS_Entity entity)
	{
		if (ContainsComponent<T>(entity))
		{
			m_ComponentManager->RemoveComponent<T>(entity);

			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentType<T>(), false);
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
	T& GetComponent(ECS_Entity entity)
	{
		return m_ComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		return m_ComponentManager->GetComponentType<T>();
	}


	// System methods
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return m_SystemManager->RegisterSystem<T>(*this);
	}

	template<typename T>
	void UnregisterSystem()
	{
		return m_SystemManager->UnregisterSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		m_SystemManager->SetSignature<T>(signature);
	}
};