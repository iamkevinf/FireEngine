#include "exportapi.h"

#include "math/math.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <bx/math.h>

EXPORT_API void Internal_FromEulerRad_Injected(glm::vec3 euler, glm::quat* ret)
{
	*ret = glm::normalize(glm::quat(euler));
}

EXPORT_API void Internal_ToEulerRad_Injected(glm::quat q, glm::vec3* ret)
{
	*ret = glm::eulerAngles(q);
}



glm::quat AngleAxis(float angle, const glm::vec3& axis)
{
	const float theta = angle * Deg2Rad * 0.5f;
	const float sinTheta = sinf(theta);

	// normalise x,y,z
	glm::vec3 v = axis;
	v = glm::normalize(v);
	v *= sinTheta;

	glm::quat q;
	// assign to me 
	q.x = v.x;
	q.y = v.y;
	q.z = v.z;
	q.w = cosf(theta);

	return q;
}

EXPORT_API glm::quat EulerToQuaternion(const glm::vec3& euler)
{
	glm::quat around_x = AngleAxis(euler.x, glm::vec3(1, 0, 0));
	glm::quat around_y = AngleAxis(euler.y, glm::vec3(0, 1, 0));
	glm::quat around_z = AngleAxis(euler.z, glm::vec3(0, 0, 1));

	return glm::normalize(around_y * around_x * around_z);

	//float cX(cos(euler.x / 2.0f));
	//float sX(sin(euler.x / 2.0f));

	//float cY(cos(euler.y / 2.0f));
	//float sY(sin(euler.y / 2.0f));

	//float cZ(cos(euler.z / 2.0f));
	//float sZ(sin(euler.z / 2.0f));

	//glm::quat qX(0.0f, 0.0f, cX, sX);
	//glm::quat qY(sY, 0.0f, cY, 0.0f);
	//glm::quat qZ(0.0f, sZ, cZ, 0.0f);
	//glm::quat q = qY * qX * qZ;
	//q = glm::normalize(q);
	//return q;
}


inline void MakePositive(glm::vec3& euler)
{
	const float negativeFlip = -0.0001F;
	const float positiveFlip = (PI_FULL * 2.0F) - 0.0001F;

	if (euler.x < negativeFlip)
		euler.x += 2.0 * PI_FULL;
	else if (euler.x > positiveFlip)
		euler.x -= 2.0 * PI_FULL;

	if (euler.y < negativeFlip)
		euler.y += 2.0 * PI_FULL;
	else if (euler.y > positiveFlip)
		euler.y -= 2.0 * PI_FULL;

	if (euler.z < negativeFlip)
		euler.z += 2.0 * PI_FULL;
	else if (euler.z > positiveFlip)
		euler.z -= 2.0 * PI_FULL;
}

inline const glm::vec3& applyToUnitAxisVector(const glm::mat4& m, int axis)
{
	return *reinterpret_cast<const glm::vec3*>(&m[axis]);
}

/**
 *	Return the pitch of the rotation part of this matrix
 */
inline float yaw(const glm::mat4& m)
{
	glm::vec3 zdir = applyToUnitAxisVector(m, 2);
	zdir = glm::normalize(zdir);
	return atan2f(zdir.x, zdir.z);
}


/**
 *	Return the pitch of the rotation part of this matrix
 */
inline float pitch(const glm::mat4& m)
{
	glm::vec3 zdir = applyToUnitAxisVector(m, 2);
	zdir = glm::normalize(zdir);
	return -asinf(FireEngine::clamp(-1.0f, zdir.y, +1.0f));
}


/**
 *	Return the roll of the rotation part of this matrix
 */
inline float roll(const glm::mat4& m)
{
	float roll;

	glm::vec3 xdir = applyToUnitAxisVector(m, 0);
	glm::vec3 zdir = applyToUnitAxisVector(m, 2);
	xdir = glm::normalize(xdir);
	zdir = glm::normalize(zdir);

	const float zdirxzlen = sqrtf(zdir.z * zdir.z + zdir.x * zdir.x);
	const float acarg = (zdir.z * xdir.x - zdir.x * xdir.z) / zdirxzlen;
	if (acarg <= -1.0f) return MATH_PI;
	if (zdirxzlen == 0.f || acarg >= 1.f) return 0.f;
	roll = acosf(FireEngine::clamp(-1.0f, acarg, +1.0f));

	return xdir.y < 0.f ? -roll : roll;
}


EXPORT_API glm::vec3 QuaternionToEuler(const glm::quat q)
{
	const glm::vec3 v(q.x, q.y, q.z);

	//calculate coefficents
	float xx = v[0] * v[0] * 2.f;
	float xy = v[0] * v[1] * 2.f;
	float xz = v[0] * v[2] * 2.f;

	float yy = v[1] * v[1] * 2.f;
	float yz = v[1] * v[2] * 2.f;

	float zz = v[2] * v[2] * 2.f;

	float wx = q.w * v[0] * 2.f;
	float wy = q.w * v[1] * 2.f;
	float wz = q.w * v[2] * 2.f;

	glm::mat4 m;
	m[0][0] = 1.f - (yy + zz);
	m[1][0] = xy - wz;
	m[2][0] = xz + wy;

	m[0][1] = xy + wz;
	m[1][1] = 1.f - (xx + zz);
	m[2][1] = yz - wx;

	m[0][2] = xz - wy;
	m[1][2] = yz + wx;
	m[2][2] = 1.f - (xx + yy);

	glm::vec3 euler = { pitch(m), yaw(m), roll(m) };
	MakePositive(euler);
	return euler;

	//glm::mat3 m;
	//glm::vec3 v;

	//float x = q.x * 2.0F;
	//float y = q.y * 2.0F;
	//float z = q.z * 2.0F;
	//float xx = q.x * x;
	//float yy = q.y * y;
	//float zz = q.z * z;
	//float xy = q.x * y;
	//float xz = q.x * z;
	//float yz = q.y * z;
	//float wx = q.w * x;
	//float wy = q.w * y;
	//float wz = q.w * z;

	//// Calculate 3x3 matrix from orthonormal basis
	//m[0][0] = 1.0f - (yy + zz);
	//m[0][1] = xy + wz;
	//m[0][2] = xz - wy;

	//m[1][0] = xy - wz;
	//m[1][1] = 1.0f - (xx + zz);
	//m[1][2] = yz + wx;

	//m[2][0] = xz + wy;
	//m[2][1] = yz - wx;
	//m[2][2] = 1.0f - (xx + yy);

	//if (m[1][2] < 0.999F)
	//{
	//    if (m[1][2] > -0.999F)
	//    {
	//        v.x = asin(-m[1][2]);
	//        v.y = atan2(m[0][2], m[2][2]);
	//        v.z = atan2(m[1][0], m[1][1]);
	//        MakePositive(v);
	//        return v;
	//    }
	//    else
	//    {
	//        v.x = PI_HALF;
	//        v.y = atan2(m[0][1], m[0][0]);
	//        v.z = 0.0F;
	//        MakePositive(v);
	//        return v;
	//    }
	//}
	//else
	//{
	//    v.x = -PI_HALF;
	//    v.y = atan2(-m[0][1], m[0][0]);
	//    v.z = 0.0F;
	//    MakePositive(v);
	//    return v;
	//}
}
