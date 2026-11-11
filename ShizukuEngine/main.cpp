#include "ShizukuEngine/ShizukuEngine.h"
#include "Game/GameScene.h"
#include "Game/TitleScene.h"
#include <Windows.h>
#include <memory>

const int32_t kClineWidth = 1280;
const int32_t kClineHeight = 720;

GameScene* gameScene = nullptr;
TitleScene* title = nullptr;
//GameClear* clear_ = nullptr;
//GameOver* gameOver_ = nullptr;


void ChangeScene();
void UpdateScene();
void DrawScene();

enum class Scene { kUnknown = 0, kTitle, kGame, kGameClear, kGameOver };

Scene scene = Scene::kTitle;
ShizukuEngine* engine = nullptr;


int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	engine = new ShizukuEngine(kClineWidth, kClineHeight, L"ShizukuEngine");
	title = new TitleScene();
	title->Initialize(engine);

	MSG msg{};
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else {

			engine->Update();

			ChangeScene();
			UpdateScene();


			engine->PreDraw();

			DrawScene();

			engine->PostDraw();

			if (engine->GetInput().GetKeyDown(VK_ESCAPE)) {
				break;
			}
		}
	}

	delete engine;

	return 0;
}



void ChangeScene() {

	switch (scene) {
	case Scene::kTitle:
		if (title->IsFinished()) {
			scene = Scene::kGame;
			delete title;
			title = nullptr;

			gameScene = new GameScene;
			gameScene->Initialize(engine);
		}
		break;
	case Scene::kGame:
		break;
	
	default:
		break;
	}
}


#pragma region ゲームの更新処理
void UpdateScene() {
	switch (scene) {
	case Scene::kTitle: // タイトル
		// LoadDebugSettings();

		title->Update(1.0f/60.0f);
		break;
	case Scene::kGame: // ゲーム中
		// LoadDebugSettings();

		gameScene->Update(1.0f / 60.0f);
		break;
	//case Scene::kGameClear: // ゲームクリア
	//	clear_->Updete();
	//	break;

	//case Scene::kGameOver: // ゲームオーバー
	//	gameOver_->Updete();
	//	break;

	default:
		break;
	}
}
#pragma endregion

#pragma region ゲームの描画
void DrawScene() {
	switch (scene) {
	case Scene::kTitle: // タイトル
		// LoadDebugSettings();

		title->Draw();
		break;
	case Scene::kGame: // ゲーム中
		// LoadDebugSettings();

		gameScene->Draw();
		break;
	//case Scene::kGameClear: // ゲームクリア
	//	clear_->Draw();

	//	break;

	//case Scene::kGameOver: // ゲームオーバー
	//	gameOver_->Draw();

	//	break;

	default:
		break;
	}
}
#pragma endregion

