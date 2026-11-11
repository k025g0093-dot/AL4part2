#pragma once
#include "../../ShizukuEngine/Engine/Math/Vector3.h"

struct PlayerCommand
{
	Vector3 move{};
	Vector3 aim{};
	bool isFire = false;
};


