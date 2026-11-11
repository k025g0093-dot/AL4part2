#pragma once
#include "../ShizukuEngine/Engine/Math/Vector3.h"

struct FollowCameraParams
{
	Vector3 pos;
};

class FollowCamera
{
public:

	void Initialize(Vector3 targetPosition);

	Vector3 Update(Vector3 targetPosition);

private:
	FollowCameraParams mOffset;
	FollowCameraParams mNowPos;
	float kInterpolationRate = 0.02f;
};

