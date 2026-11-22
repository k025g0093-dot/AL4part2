#pragma once
#include "EnemyCommand.h"


class ShizukuEngine;
class Enemy
{
public:
	void Initialize(int model, int animationModel, const Vector3& position);

	void Update(const EnemyCommand& cmd, float dt);

	void Draw(ShizukuEngine* engine);

	const Vector3& GetPosition() const { return mPosition; }
	float GetRotationY() const { return mRotationY; }
	int GetModel() const { return mModel; }
	bool IsAlive() const { return mIsAlive; }
	bool IsSpwanEnemy()const { return mIsSpwan; }

private:

	void SpwanEnemy();

private:
	
	int mModel = -1;
	int mBulletModel = -1;
	int mAnimation = -1;

	Vector3 mPosition{};
	float mRotationY = 0.0f;

	float mSpeed = 10.0f;
	float mFireInterval = 0.1f;
	float mFireTimer = 0.0f;
	bool mIsAlive = true;
	bool mIsSpwan = false;
	static constexpr float kFieldHalf = 99.0f; // 壁の内側が ±100、自機の半径ぶん内側
	float mDt{};

};

