#include "Bone.h"
#include <assimp/scene.h>
#pragma comment(lib,"assimp-vc145-mt")
Bone::Bone() {}
Bone::~Bone() {}

void Bone::Initialize(const std::string& name, int ID, const aiNodeAnim* channel) {

}

void Bone::Update(float animationTime) {

}

int Bone::GetPositionIndex(float animationTime) {

}

int Bone::GetRotationIndex(float animationTime){

}

int Bone::GetScaleIndex(float animationTime) {

}

float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) {

}

glm::mat4 Bone::InterpolatePosition(float animationTime) {

}

glm::mat4 Bone::InterpolateRotation(float animationTime) {

}

glm::mat4 Bone::InterpolateScaling(float animationTime) {

}