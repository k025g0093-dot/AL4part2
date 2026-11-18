#pragma once
#include "../../ShizukuEngine/Engine/Math/Vector3.h"

class PlayerBullet
{
public:

	PlayerBullet()=default;
	~PlayerBullet()=default;

	void Initialize(int model, const Vector3& position, const Vector3& direction);

	void Update(float dt);

	const Vector3& GetPosition() const { return mPosition; }
	float GetRotationY() const { return mRotationY; }
	int GetModel() const { return mModel; }
	bool IsAcctiveFrame() const { return isAcctive; }

private:

	static const int32_t kLifeTime = 60 * 5;
	int32_t mDeathTime = kLifeTime;

	int mModel = -1;
	Vector3 mPosition{};
	Vector3 mDirection{};
	float mRotationY = 0.0f;

	float mSpeed = 10.0f;
	float mFireInterval = 0.1f;
	float mFireTimer = 0.0f;
	bool isAcctive = false;

};

