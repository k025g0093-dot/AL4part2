#include "Animation.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h> 
#include <assimp/scene.h>
#include "Bone.h"

Animation::~Animation()
{}

void Animation::Initialize(
	const std::string& animationPath,
	SkinnedModel* skinnedModel)
{
	mSkinnedModel = skinnedModel;
	Assimp::Importer importer{};
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);
	auto animation = scene->mAnimations[0];
	mDuration = animation->mDuration;
	mTickPerSecond = animation->mTicksPerSecond;
	ReadHeirarchyData(mRootNode, scene->mRootNode);
	ReadMissingBones(animation);
}

Bone* Animation::FindBone(const std::string& name) {
	auto iter = std::find_if(mBones.begin(), mBones.end(),
		[&](const Bone& Bone) {
			return Bone.GetBoneName() == name;
		}
	);

	if (iter == mBones.end())return nullptr;
	else return &(*iter);
}

void Animation::ReadMissingBones(const aiAnimation* animation) {

	int size = animation->mNumChannels;

	auto& boneInFoMap = mSkinnedModel->GetBoneInFoMap();
	int& boneCount = mSkinnedModel->GetBoneCount();

	for (int i = 0; i < size; i++)
	{

		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if (boneInFoMap.find(boneName) == boneInFoMap.end())
		{
			boneInFoMap[boneName].id = boneCount;
			boneCount++;
		}
		mBones.push_back(Bone(channel->mNodeName.data,
			boneInFoMap[channel->mNodeName.data].id, channel));
	}
	mBoneInFoMap = boneInFoMap;
}
