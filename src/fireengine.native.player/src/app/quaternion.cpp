#include "exportapi.h"

#include "math/math.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

EXPORT_API void Internal_FromEulerRad_Injected(glm::vec3 euler, glm::quat* ret)
{
    *ret = glm::quat(euler);
}

EXPORT_API void Internal_ToEulerRad_Injected(glm::quat q, glm::vec3* ret)
{
    *ret = glm::eulerAngles(q);
}

