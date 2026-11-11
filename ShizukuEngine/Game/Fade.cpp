#include "Fade.h"
#include <algorithm>

void Fade::Initialize(ShizukuEngine* engine) {
	mEngine = engine;

	texture=mEngine->LoadTexture("white1x1.png");
}

void Fade::Start(Status status, float duration) {

	mStatus = status;
	mDuration = duration;
	mCounter = 0.0f;

}

void Fade::Update() {

	switch (mStatus)
	{
	case Fade::Status::None:
		//何もしない
		break;
	case Fade::Status::FadeIn:
		mCounter += 1.0f / 60.0f;
		if (mCounter >= mDuration) {
			mCounter = mDuration;
		}
		color.w  = std::clamp((mDuration - mCounter) / mDuration, 0.0f, 1.0f);
		
		break;
	case Fade::Status::FadeOut:
		mCounter += 1.0f / 60.0f;
		if (mCounter >= mDuration) {
			mCounter = mDuration;
		}
		color.w= std::clamp(mCounter/ mDuration, 0.0f, 1.0f);
		
		break;
	default:
		break;
	}

}

void Fade::Draw() {

	if (mStatus == Status::None) {
		return;
	}

	mEngine->DrawSprite(
		{ 0.0f, 0.0f },
		0.0f,
		1280.0f, 720.0f,
		color,
		texture);
}

bool  Fade::IsFinished() const{

	switch (mStatus)
	{
	case Fade::Status::None:
		break;
	case Fade::Status::FadeIn:
		if (mCounter >= mDuration) {
			return true;
		}
		else {
			return false;
		}
		break;
	case Fade::Status::FadeOut:
		if (mCounter >= mDuration) {
			return true;
		}
		else {

			return false;
		}
		break;
	default:
		break;
	}
	return true;
}
