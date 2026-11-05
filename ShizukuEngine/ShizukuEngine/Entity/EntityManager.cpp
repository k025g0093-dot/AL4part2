#include "EntityManager.h"

Entity* EntityManager::CloneToRuntime(const Entity& entity) {

	auto runtimeEntity = std::make_unique<Entity>();
	runtimeEntity->SetName(entity.GetName());
	runtimeEntity->SetDisplayName(entity.GetDisplayName());


	Entity* result = runtimeEntity.get();
	mRuntimeEntities.push_back(std::move(runtimeEntity));
	return result;
}



void EntityManager::StartPlay() {

	if (mPlayState != PlayState::editing)return;

	mRuntimeEntities.clear();
	for (const auto& editorEntity : mEntities) {
		CloneToRuntime(*editorEntity);
	}

	mPlayState = PlayState::playing;

}

void EntityManager::StopPlay() {
	if (mPlayState == PlayState::editing)return;

	mRuntimeEntities.clear();
	mPlayState = PlayState::editing;
}

void EntityManager::PausePlay() {
	if (mPlayState == PlayState::playing)
		mPlayState = PlayState::paused;
}

void EntityManager::ResumePlay() {
	if (mPlayState == PlayState::paused)
		mPlayState = PlayState::playing;
}
