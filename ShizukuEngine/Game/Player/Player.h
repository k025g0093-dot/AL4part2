#pragma once
#include "PlayerCommand.h"

class Player
{
public:

	void Initialize(int model, const Vector3& position);

	void Update(const PlayerCommand& cmd, float dt);

	const Vector3& GetPosition() const { return mPosition; }
	float GetRotationY() const { return mRotationY; }
	int GetModel() const { return mModel; }
	bool IsShotThisFrame() const { return mShotThisFrame; }

private:

	int mModel = -1;
	Vector3 mPosition{};
	float mRotationY = 0.0f;

	float mSpeed = 10.0f;
	float mFireInterval = 0.1f;
	float mFireTimer = 0.0f;
	bool mShotThisFrame = false;

	static constexpr float kFieldHalf = 99.0f; // 壁の内側が ±100、自機の半径ぶん内側

};

