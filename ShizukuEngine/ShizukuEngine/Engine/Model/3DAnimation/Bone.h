#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>
#include "AssimpGLMHelpersMath/GlmConvert.h"

struct aiNodeAnim;

/*
各構造体に入っている"timeStamp"について
どのタイミングで補間がいるのかを教えてくれる者になっています
*/

struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};
struct KeyRotation
{
	//四元数の回転
	glm::quat orientation;
	float timeStamp;
};
struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

class Bone
{

public:

	Bone();
	~Bone();

	void Initialize(const std::string& name, int ID, const aiNodeAnim* channel);

	void Update(float animationTime);

	int GetPositionIndex(float animationTime);
	int GetRotationIndex(float animationTime);
	int GetScaleIndex(float animationTime);

	glm::mat4 GetLocalTransform() { return mLocalTransform; }
	std::string GetBoneName() const{ return mName; }
	int GetBoneID() { return mID; }

private://関数

	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

	glm::mat4 InterpolatePosition(float animationTime);

	glm::mat4 InterpolateRotation(float animationTime);

	glm::mat4 InterpolateScaling(float animationTime);


private://メンバ変数
	std::vector<KeyPosition>mPosition;
	std::vector<KeyRotation>mRotation;
	std::vector<KeyScale>mScale;

	int mNumPositions;
	int mNumRotations;
	int mNumScalings;

	glm::mat4 mLocalTransform;//トランスフォーム
	std::string mName;//名前
	int mID;//ID

};

