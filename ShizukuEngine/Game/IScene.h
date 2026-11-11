#pragma once
class ShizukuEngine;

enum class Phese {
	kFadeIn,
	kMain,
	kDeht,
	kClear,
	kFadeOut,
};

//基底クラス
class IScene {

public:

	virtual ~IScene()=default;

	virtual void Initialize(ShizukuEngine* enigne) = 0;

	virtual void Update(float deltaTime)=0;

	virtual void Draw() = 0;

};

