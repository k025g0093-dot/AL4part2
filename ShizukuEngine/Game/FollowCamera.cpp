#include "FollowCamera.h"
#include <cmath>
#include <iostream>

void FollowCamera::Initialize(Vector3 targetPosition) {

	mOffset.pos = Vector3{0.0f,30.0f,-23.0f };
	mNowPos.pos = targetPosition + mOffset.pos;
}

Vector3 FollowCamera::Update(
	Vector3 targetPosition
) {
	Vector3 pos;
	Vector3 desiredPosition;
	desiredPosition = targetPosition + mOffset.pos;
	pos.x = std::lerp(mNowPos.pos.x, desiredPosition.x, kInterpolationRate);
	pos.y = std::lerp(mNowPos.pos.y, desiredPosition.y, kInterpolationRate);
	pos.z = std::lerp(mNowPos.pos.z, desiredPosition.z, kInterpolationRate);

	mNowPos.pos = pos;

	return pos;
}