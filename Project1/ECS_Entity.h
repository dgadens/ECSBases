#pragma once

#include <inttypes.h>
#include <bitset>

//entity definitions
using ECS_Entity = uint32_t;
const ECS_Entity MAX_ENTITIES = 10000;

//Components definitions
using ComponentType = uint32_t;
const ComponentType MAX_COMPONENTS = 64;
using Signature = std::bitset<MAX_COMPONENTS>;
