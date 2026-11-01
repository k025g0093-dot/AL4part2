#pragma once
#include "Vector.h"
#include "Vector3.h"
#include<cmath>

struct AABB {
	Vector3 min;
	Vector3 max;
};

inline bool IsAABBToPointCollision(const AABB& aabb, const Vector3& point) {
	return (point.x >= aabb.min.x && point.x <= aabb.max.x) &&
		(point.y >= aabb.min.y && point.y <= aabb.max.y) &&
		(point.z >= aabb.min.z && point.z <= aabb.max.z);
}


