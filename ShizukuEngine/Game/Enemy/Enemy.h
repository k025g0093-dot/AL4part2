#pragma once
#include "EnemyCommand.h"


class ShizukuEngine;
class Enemy
{
public:

	// 敵の種類
	enum class Type {
		kChase,   // 近距離型
		kRanged   // 遠距離型
	};

	// 弾
	struct Bullet {
		Vector3 position{};
		Vector3 direction{};
		float speed = 15.0f;
		float lifeTime = 5.0f;
	};

	void Initialize(int model, int animationModel, const Vector3& position,Type enemyType);

	void Update(const Vector3 &playerPosition, float dt);

	void Draw(ShizukuEngine* engine);

	const Vector3& GetPosition() const { return mPosition; }
	float GetRotationY() const { return mRotationY; }
	int GetModel() const { return mModel; }
	bool IsAlive() const { return mIsAlive; }
	bool IsSpwanEnemy()const { return mIsSpwan; }

private:

	void UpdateChase(
		const Vector3& playerPosition,
		float dt
	);

	void UpdateRanged(
		const Vector3& playerPosition,
		float dt
	);

private:
	
	int mModel = -1;
	int mBulletModel = -1;
	int mAnimation = -1;

	Vector3 mPosition{};
	float mRotationY = 0.0f;
	Type mType{};


	float mSpeed={};
	float mFireInterval = 0.1f;
	float mFireTimer = 0.0f;
	bool mIsAlive = true;
	bool mIsSpwan = true;
	static constexpr float kFieldHalf = 99.0f; // 壁の内側が ±100、自機の半径ぶん内側
	float mDt{};

};

