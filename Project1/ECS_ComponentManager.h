#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include "ECS_Component.h"
#include "ECS_ComponentArray.h"

class ComponentManager
{
private:
	// Map from type index pointer to a component type
	std::unordered_map<std::type_index, ComponentType> m_ComponentTypes;

	// Map from type index pointer to a component array
	std::vector<std::shared_ptr<IComponentArray>> m_ComponentArrays;

	// The component type to be assigned to the next registered component
	ComponentType m_NextComponentType;

	// Convenience function to get the statically casted pointer to 
	//the ComponentArray of type T.
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		std::type_index typeIndex(typeid(T));
		auto componentType = m_ComponentTypes[typeIndex];
		return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[componentType]);
	}

public:
	ComponentManager()
		: m_NextComponentType(0)
	{}

	template<typename T>
	void RegisterComponent()
	{
		std::type_index typeIndex(typeid(T));

		// Add this component type to the component type map
		m_ComponentTypes.insert({ typeIndex, m_NextComponentType });

		// Create a ComponentArray pointer and add it to the component arrays map
		m_ComponentArrays.push_back(std::make_shared<ComponentArray<T>>());

		// Increment the value so that the next component registered will be different
		++m_NextComponentType;
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		std::type_index typeIndex(typeid(T));
		return m_ComponentTypes[typeIndex];
	}

	template<typename T>
	void AddComponent(ECS_Entity entity, T& component)
	{
		// Add a component to the array for an entity
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(ECS_Entity entity)
	{
		// Remove a component from the array for an entity
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<typename T>
	T& GetComponent(ECS_Entity entity)
	{
		// Get a reference to a component from the array for an entity
		return GetComponentArray<T>()->GetData(entity);
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
