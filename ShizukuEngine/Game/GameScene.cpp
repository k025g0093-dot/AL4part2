#include "GameScene.h"

void GameScene::Initialize(ShizukuEngine* engine) {
	mEngine = engine;

	mFade = new Fade();
	mFade->Initialize(mEngine);
	mFade->Start(Fade::Status::FadeIn, 1.0f);


	//モデル読み込み
	field = mEngine->LoadModel("resources/field/field.obj");
	player = mEngine->LoadSkinnedModel("resources/Flair/Flair.dae");
	dancce = mEngine->LoadAnimation("resources/Flair/Flair.dae", player);

	bulletModel = mEngine->LoadModel("resources/bullet/bullet.fbx");
	enemyMode = mEngine->LoadModel("resources/enemy/enemy.fbx");

	mPlayer.Initialize(player,dancce, Vector3{ 0.0f,0.0f,0.0f });
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

		if (mEngine->GetInput().GetKeyDown(VK_CONTROL)) {
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

	mEngine->DrawModel(field, {}, {}, { 1,1,1 }, { 1,1,1,1 });

	mPlayer.Draw(mEngine);

	for (PlayerBullet* bullet : allyBullets) {
		mEngine->DrawModel(
			bulletModel, bullet->GetPosition(),
			{ 0.0f, bullet->GetRotationY(), 0.0f },
			{ 1,1,1 }, { 1,1,1,1 });
	}

	for (Enemy* enemy : allyEnemys) {
		enemy->Draw(mEngine);
	}


}

void GameScene::Run(float dt) {

	PlayerCommand cmd = mInputMapper.Make(mEngine->GetInput());
	mPlayer.Update(cmd, dt);

	if (mPlayer.IsShotThisFrame()) {
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(bulletModel, mPlayer.GetPosition(), cmd.aim);
		AddAllyBullet(newBullet);
	}

	if(mEnemy.IsSpwanEnemy()){
		Enemy* newEnemy = new Enemy();
		newEnemy->Initialize(enemyMode, dancce,mEnemy.GetPosition());
		allyEnemys.push_back(newEnemy);
	}


	mEngine->GetCamera().SetCameraPos(
		mFollowCamera.Update(mPlayer.GetPosition())
	);


	allyBullets.remove_if([](PlayerBullet* bullet) {
		if (!bullet->IsAcctiveFrame()) {
			delete bullet;
			return true;
		}
		return false;
		});

	allyEnemys.remove_if([](Enemy* enemy) {
		if (!enemy->IsAlive()) {
			delete enemy;
			return true;
		}return false;
		});


	for (PlayerBullet* bullet : allyBullets) {
		bullet->Update(dt);
	}

	for (Enemy* enemy : allyEnemys) {
		EnemyCommand cmd;
		enemy->Update(cmd,dt);
	}

}

void GameScene::AddAllyBullet(PlayerBullet* bullet) {
	allyBullets.push_back(bullet);
}

