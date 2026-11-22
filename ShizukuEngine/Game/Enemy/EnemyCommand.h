#pragma once
#include "../../ShizukuEngine/Engine/Math/Vector3.h"

struct EnemyCommand
{
	Vector3 move{};
	Vector3 velcity;
	bool isAttack = false;
};