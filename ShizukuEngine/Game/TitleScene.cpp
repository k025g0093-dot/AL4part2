#include "TitleScene.h"

void TitleScene::Initialize(ShizukuEngine* engine) {
	mEngine = engine;

	mFade = new Fade();
	mFade->Initialize(mEngine);
	mFade->Start(Fade::Status::FadeIn, 1.0f);

}

void TitleScene::Update(float dt) {

	mFade->Update();

	switch (mPhese)
	{
	case Phese::kFadeIn:

		if (mFade->IsFinished()) {
			mPhese = Phese::kMain;
		}

		break;
	case Phese::kMain:

		if (mEngine->GetInput().GetKeyDown(VK_SPACE)) {
			mFade->Start(Fade::Status::FadeOut, 1.0f);
			mPhese=Phese::kFadeOut;
		}

		break;
	case Phese::kFadeOut:

		if (mFade->IsFinished()) {
			isFinished = true;
		}

		break;
	default:
		break;
	}



}

void TitleScene::Draw() {

	mFade->Draw();

}