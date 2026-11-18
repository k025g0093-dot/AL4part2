#include "PlayerBullet.h"
#include <algorithm>
#include <cmath>

void PlayerBullet::Initialize(int model, const Vector3& position,const Vector3&direction ) {
	mModel = model;
	mPosition = position;
	isAcctive = true;
	mDirection = direction;
}

void PlayerBullet::Update(float dt) {
	if (--mDeathTime <= 0) {
		isAcctive = false;
	}
	mPosition.x += mDirection.x * (mSpeed * dt);
	mPosition.z+= mDirection.z * (mSpeed * dt);
}
