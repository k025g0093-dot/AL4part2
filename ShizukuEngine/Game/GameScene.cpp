#include "GameScene.h"

void GameScene::Initialize(ShizukuEngine* engine) {
	mEngine = engine;

	mFade = new Fade();
	mFade->Initialize(mEngine);
	mFade->Start(Fade::Status::FadeIn, 1.0f);


	//モデル読み込み
	field = mEngine->LoadModel("resources/field/field.obj");
	player = mEngine->LoadSkinnedModel("resources/Flair/Flair.dae");
	dancce= mEngine->LoadAnimation("resources/Flair/Flair.dae", player);

	mPlayer.Initialize(player, Vector3{ 0.0f,0.0f,0.0f });
	mFollowCamera.Initialize(mPlayer.GetPosition());


}

void GameScene::Update(float dt) {

	mFade->Update();

	switch (mPhese)
	{
	case Phese::kFadeIn:

		if (mFade->IsFinished()) {
			mPhese = Phese::kMain;
		}

		break;
	case Phese::kMain:

		Run(dt);

		if (mEngine->GetInput().GetKeyDown(VK_SPACE)) {
			mFade->Start(Fade::Status::FadeOut, 1.0f);
			mPhese = Phese::kFadeOut;
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

void GameScene::Draw() {

	mFade->Draw();

	mEngine->DrawSkinnedModel(
		player,
		dancce,
		mPlayer.GetPosition(),
		{ 0.0f, mPlayer.GetRotationY(), 0.0f},
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f }
	);

	mEngine->DrawModel(
		field,
		{ 0,0,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ 1,1,1,1 }
	);

}

void GameScene::Run(float dt) {

	PlayerCommand cmd = mInputMapper.Make(mEngine->GetInput());
	mPlayer.Update(cmd, dt);

	mEngine->GetCamera().SetCameraPos(
		mFollowCamera.Update(mPlayer.GetPosition())
	);
}
