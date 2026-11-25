#include "PlayerInputMapper.h"
#include <numbers>
PlayerCommand PlayerInputMapper::Make(const Input& input, float dt ){

	Vector3 move(
		float (input.GetKey('D') )- float(input.GetKey('A')),
			0.0f,
			float(input.GetKey('W') )- float(input.GetKey('S'))
			);

	const float turn = float(input.GetKey(VK_RIGHT)) - float(input.GetKey(VK_LEFT));

	mAimAngle += turn * mAimTurnSpeed * dt;
	mAimAngle = std::remainder(mAimAngle, 2.0f * std::numbers::pi_v<float>);


	Vector3 aim{ mAimAngle ,0.0f,mAimAngle };

	bool fire = false;
	if (input.GetKey(VK_SPACE)) { fire = true; }

	if (input.IsPadConnected()) {

		Vector3 padMove(input.GetLeftStickX(), 0.0f, input.GetLeftStickY());
		Vector3 padAim(input.GetRightStickX(), 0.0f, input.GetRightStickY());

		if (padMove.Length() > 0.0f) { move = padMove; }
		if (padAim.Length() > 0.0f) { mAimAngle = std::atan2(padAim.x, padAim.z); }
		if (input.GetPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) { fire = true; }
	}


	PlayerCommand cmd;
	// 斜め入力で長さ√2 にならないよう、1を超えたら1にそろえる
	cmd.move = (move.Length() > 1.0f) ? move.Normalized() : move;
	cmd.aim = Vector3(std::sin(mAimAngle), 0.0f, std::cos(mAimAngle));
	cmd.isFire = fire;
	return cmd;

}