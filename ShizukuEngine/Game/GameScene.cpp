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
	dancce = mEngine->CreateAnimator(dancce);

	bulletModel = mEngine->LoadModel("resources/bullet/bullet.fbx");
	enemyMode = mEngine->LoadModel("resources/enemy/enemy.fbx");

	allyEnemys.clear();

	mSpawnCount = 0;

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

		if (mPlayer.GetHp() < 0) {
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
		if (enemy->GetHp()<0) {
			delete enemy;
			return true;
		}
		return false;
		});

	for (PlayerBullet* bullet : allyBullets) {
		bullet->Update(dt);
	}

	for (Enemy* enemy : allyEnemys) {
		enemy->Update(mPlayer.GetPosition(), dt);
	}

	mSpawnTimer += dt;
	if (mSpawnTimer >= mSpawnInterval) {
		mSpawnTimer = 0.0f;
		if (allyEnemys.size() < mMaxEnemyCount) {
			SpwanEnemy();
		}
	}

	AllCollision();

}


void GameScene::AllCollision() {

	/*敵とプレイヤーの当たり判定*/
	for (Enemy* enemy : allyEnemys) {
		OBB enemyObb{};
		enemyObb.center = enemy->GetPosition();
		enemyObb.orientations[0] = {1,0,0};
		enemyObb.orientations[1] = {0,1,1};
		enemyObb.orientations[2] = {0,0,1};
		enemyObb.size = { 1.0f,1.0f,1.0f };

		OBB playerObb{};
		playerObb.center = mPlayer.GetPosition();
		playerObb.orientations[0] = {1,0,0};
		playerObb.orientations[1] = {0,1,1};
		playerObb.orientations[2] = {0,0,1};
		playerObb.size = { 1.0f,1.0f,1.0f };

		if (mEngine->IsOBBToOBB(playerObb, enemyObb)) {

			Vector3 pos=mPlayer.GetPosition();
			pos+=0.5f;
			mPlayer.UpDateDamage();
			mPlayer.SetPosition(pos);
		}
	}

	/*敵とプレイヤーの弾の当たり判定*/
	for (Enemy* enemy : allyEnemys) {
		OBB enemyObb{};
		enemyObb.center = enemy->GetPosition();
		enemyObb.orientations[0] = { 1,0,0 };
		enemyObb.orientations[1] = { 0,1,1 };
		enemyObb.orientations[2] = { 0,0,1 };
		enemyObb.size = { 1.0f,1.0f,1.0f };
		for (PlayerBullet* bullet : allyBullets) {
		
			OBB playerBulletObb{};
			playerBulletObb.center = bullet->GetPosition();
			playerBulletObb.orientations[0] = { 1,0,0 };
			playerBulletObb.orientations[1] = { 0,1,1 };
			playerBulletObb.orientations[2] = { 0,0,1 };
			playerBulletObb.size = { 1.0f,1.0f,1.0f };
		
			if (mEngine->IsOBBToOBB(playerBulletObb, enemyObb)) {
				enemy->UpDateDamage();
			}
		}
	}
}


void GameScene::AddAllyBullet(PlayerBullet* bullet) {
	allyBullets.push_back(bullet);
}

void GameScene::SpwanEnemy() {

	Vector3 spawnPosition{};

	Enemy::Type type = Enemy::Type::kChase;

	switch (mSpawnCount%4)
	{
	case 0:
		spawnPosition = { 50.0f,0.0f,50.0f };
		break;
	case 1:
		spawnPosition = { -50.0f,0.0f,50.0f };
		break;
	case 2:
		spawnPosition = { 50.0f,0.0f,-50.0f };
		break;
	case 3:
		spawnPosition = { -50.0f,0.0f,-50.0f };
		break;
	default:
		break;
	}

	if (mSpawnCount % 5 == 1) {
		type = Enemy::Type::kRanged;
	}

	// ザコ敵生成
	Enemy* newEnemy=new Enemy();

	newEnemy->Initialize(
		enemyMode, -1,
		spawnPosition,
		type
	);

	allyEnemys.push_back(newEnemy);

	mSpawnCount++;

}