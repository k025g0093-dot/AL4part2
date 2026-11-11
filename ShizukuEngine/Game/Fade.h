#pragma once
#include "../ShizukuEngine/ShizukuEngine.h"

class Fade
{
public:
	// フェードの状態
	enum class Status {
		None,    // 何もしない
		FadeIn,  // 明るくなる
		FadeOut, // 暗くなる
	};

	void Initialize(ShizukuEngine* engine);
	void Update();
	void Draw();

	// フェード開始の指示
	void Start(Status status, float duration);

	void Stop();

	// フェードが終了したか
	bool IsFinished() const;

private:

	ShizukuEngine* mEngine;

	Status mStatus = Status::None;
	int texture = 0;
	float mDuration = 1.0f; // かける時間
	float mCounter = 0.0f;  // 現在の時間

	Vector4 color = { 0.0f,0.0f,0.0f,0.0f };

};

