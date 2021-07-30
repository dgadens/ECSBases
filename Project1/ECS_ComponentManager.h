#pragma once

#include <unordered_map>
#include "ECS_Component.h"
#include "ECS_ComponentArray.h"

class ComponentManager
{
private:
	// Map from type index pointer to a component array
	std::vector<IComponentArray*> m_ComponentArrays;

	// Convenience function to get the statically casted pointer to 
	//the ComponentArray of type T.
	template<typename T>
	ComponentArray<T>* GetComponentArray(uint32_t id)
	{
		return (ComponentArray<T>*)m_ComponentArrays[id];
	}

public:
	~ComponentManager()
	{
		for (auto& c : m_ComponentArrays)
			delete(c);
		m_ComponentArrays.clear();
	}

	template<typename T>
	uint32_t RegisterComponent()
	{
		// Create a ComponentArray pointer and add it to the 
		//component arrays map
		m_ComponentArrays.push_back(new ComponentArray<T>());
		return m_ComponentArrays.size() - 1;
	}

	template<typename T>
	void AddComponent(ECS_Entity entity, T& component, uint32_t id)
	{
		// Add a component to the array for an entity
		GetComponentArray<T>(id)->InsertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(ECS_Entity entity, uint32_t id)
	{
		// Remove a component from the array for an entity
		GetComponentArray<T>(id)->RemoveData(entity);
	}

	template<typename T>
	T& GetComponent(ECS_Entity entity, uint32_t id)
	{
		// Get a reference to a component from the array for an entity
		return GetComponentArray<T>(id)->GetData(entity);
	}

	void EntityDestroyed(ECS_Entity entity)
	{
		// Notify each component array that an entity has been destroyed
		// If it has a component for that entity, it will remove it
		for (auto const& arr : m_ComponentArrays)
			arr->EntityDestroyed(entity);
	}

	void CloneEntity(ECS_Entity sourceEntity, ECS_Entity cloneEntity)
	{
		// clone data of source entity into clone entity
		for (auto const& arr : m_ComponentArrays)
			arr->CloneEntity(sourceEntity, cloneEntity);
	}

};
