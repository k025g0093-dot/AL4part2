#pragma once
#include "IScene.h"
#include "../ShizukuEngine/ShizukuEngine.h"
#include "Fade.h"
/*プレイヤー関係*/
#include "Player/Player.h"
#include "Player/PlayerBullet.h"
#include "Player/PlayerInputMapper.h"

/*敵関係*/
#include "Enemy/Enemy.h"

/*カメラ*/
#include "FollowCamera.h"

class GameScene : public IScene
{

public:

	void Initialize(ShizukuEngine* engine)override;
	void Update(float dt)override;
	void Draw()override;

	bool IsFinished()const { return isFinished; }

private:

	void AddAllyBullet(PlayerBullet* bullet);
	void Run(float dt);

private:

	ShizukuEngine* mEngine = nullptr;
	
	Phese mPhese = Phese::kFadeIn;

	Fade* mFade = nullptr;

	Player mPlayer;
	std::list<PlayerBullet*>allyBullets;
	PlayerInputMapper mInputMapper;

	Enemy mEnemy;
	std::list<Enemy*>allyEnemys;


	FollowCamera mFollowCamera;



	//素材（モデルど）int型で指定

	int field = 0;
	int player = 0;
	int dancce = 0;
	int bulletModel = 0;
	int enemyMode = 0;

	//ゲームの状態
	bool isFinished = false;

};

