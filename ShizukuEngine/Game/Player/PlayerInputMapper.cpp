#include "PlayerInputMapper.h"
PlayerCommand PlayerInputMapper::Make(const Input& input)const {

	Vector3 move(
		float (input.GetKey('D') )- float(input.GetKey('A')),
			0.0f,
			float(input.GetKey('W') )- float(input.GetKey('S'))
			);

	Vector3 aim(
		float(input.GetMouseDeltaX()),
		0.0f,
		float(input.GetMouseDeltaY()));

	bool fire = false;
	if (input.GetMouseButton(0)) { fire = true; }

	if (input.IsPadConnected()) {

		Vector3 padMove(input.GetLeftStickX(), 0.0f, input.GetLeftStickY());
		Vector3 padAim(input.GetRightStickX(), 0.0f, input.GetRightStickY());

		if (padMove.Length() > 0.0f) { move = padMove; }
		if (padAim.Length() > 0.0f) { aim = padAim; }
		if (input.GetPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) { fire = true; }
	}

	PlayerCommand cmd;
	// 斜め入力で長さ√2 にならないよう、1を超えたら1にそろえる
	cmd.move = (move.Length() > 1.0f) ? move.Normalized() : move;
	cmd.aim = aim.Normalized();
	cmd.isFire = fire;
	return cmd;

}