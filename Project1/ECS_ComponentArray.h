#pragma once

#include <array>
#include <unordered_map>
#include "ECS_Entity.h"

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void CloneEntity(ECS_Entity sourceEntity, ECS_Entity cloneEntity) = 0;
	virtual void EntityDestroyed(ECS_Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
private:
	std::array<T, MAX_ENTITIES> m_ComponentArray;
	std::unordered_map<ECS_Entity, size_t> m_EntityToIndexMap;
	std::unordered_map<size_t, ECS_Entity> m_IndexToEntityMap;

	//Total size of valid entries in the array
	size_t m_Size;

public:
	void InsertData(ECS_Entity entity, T& component)
	{
		// Put new entry at end and update the maps
		size_t newIndex = m_Size;
		m_EntityToIndexMap[entity] = newIndex;
		m_IndexToEntityMap[newIndex] = entity;
		m_ComponentArray[newIndex] = component;
		++m_Size;
	}

	void RemoveData(ECS_Entity entity)
	{
		// Copy element at end into deleted element's place to maintain density
		size_t indexOfRemovedEntity = m_EntityToIndexMap[entity];
		size_t indexOfLastElement = m_Size - 1;
		if (indexOfRemovedEntity != indexOfLastElement)
			m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];

		//clean last component with defaut data
		m_ComponentArray[indexOfLastElement] = m_ComponentArray[m_Size];

		// Update map to point to moved spot
		ECS_Entity entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
		m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		m_EntityToIndexMap.erase(entity);
		m_IndexToEntityMap.erase(indexOfLastElement);

		--m_Size;
	}

	T& GetData(ECS_Entity entity)
	{
		// Return a reference to the entity's component
		return m_ComponentArray[m_EntityToIndexMap[entity]];
	}

	void EntityDestroyed(ECS_Entity entity) override
	{
		if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end())
		{
			// Remove the entity's component if it existed
			RemoveData(entity);
		}
	}

	void CloneEntity(ECS_Entity entitySource, ECS_Entity entityClone) override
	{
		if (m_EntityToIndexMap.find(entitySource) != m_EntityToIndexMap.end())
		{
			auto& source = GetData(entitySource);
			InsertData(entityClone, source);
		}
	}
};

