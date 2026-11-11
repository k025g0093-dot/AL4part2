#pragma once
#include "IScene.h"
#include "../ShizukuEngine/ShizukuEngine.h"
#include "Fade.h"

class TitleScene : public IScene
{

public:

	void Initialize(ShizukuEngine* engine)override;
	void Update(float dt)override;
	void Draw()override;

	bool IsFinished()const { return isFinished; }

private:

	ShizukuEngine* mEngine = nullptr;

	Phese mPhese = Phese::kFadeIn;

	Fade* mFade = nullptr;

	//素材（モデルど）int型で指定



	//ゲームの状態
	bool isFinished = false;


};

