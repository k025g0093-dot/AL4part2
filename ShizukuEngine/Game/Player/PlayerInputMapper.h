#pragma once
#include "PlayerCommand.h"
#include "../../ShizukuEngine/Engine/Core/Input.h"

class PlayerInputMapper
{
public:
	// dt は照準の回転速度をフレームレートに左右されないようにするために使う
	PlayerCommand Make(const Input& input, float dt = 1.0f / 60.0f);

private:
	// 照準の角度（ラジアン）。0 で +Z（奥）、プラスで +X（右）回り
	float mAimAngle = 0.0f;

	// Q / E で照準を回す速さ（ラジアン / 秒）
	float mAimTurnSpeed = 3.0f;
};
