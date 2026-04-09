#include "GameObject.h"
#include <cmath>

USING_NS_CC;

GameObject::GameObject() 
	: _eeMultiplier(1.f)
{

}

GameObject::~GameObject() {

}

bool GameObject::init() {
	if (!Sprite::init()) {
		return false;
	}



	return true;
}

void GameObject::onEnter() {
	Sprite::onEnter();
	updateRealTransform();
}

void GameObject::updateRealTransform() {
	Vec2 absolutePosition;
	absolutePosition.x = _trailPosition.x;
	absolutePosition.y = _trailPosition.y;

	Vec2 absoluteScale(1.f, 1.f);
	uint8_t opacity = 255;

	applyEEEffect(EEEffect::MOVE_VERTICAL, absolutePosition, absoluteScale, opacity);

	setPosition(absolutePosition);
	setScaleX(absoluteScale.x);
	setScaleY(absoluteScale.y);
	setOpacity(opacity);
}

// update enter+exit effects
void GameObject::updateEEEffects(const Rect& visibleArea, const float enterDistance, const float exitDistance) {
	const float& x = getTrailPosition().x;

	float enterMultiplier, exitMultiplier;

	enterMultiplier = (x - visibleArea.getMinX()) / enterDistance; // (x - left) / (left + dist - left) = (x - left) / dist
	exitMultiplier = (visibleArea.getMaxX() - x) / exitDistance;

	if (enterMultiplier > 1.f) enterMultiplier = 1.f;
	else if (enterMultiplier < 0.f) enterMultiplier = 0.f;

	if (exitMultiplier > 1.f) exitMultiplier = 1.f;
	else if (exitMultiplier < 0.f) exitMultiplier = 0.f;

	_eeMultiplier = enterMultiplier * exitMultiplier;

	updateRealTransform();
}


void GameObject::setTrailPosition(const cocos2d::Vec2& trailPosition) {
	if (trailPosition != _trailPosition) {
		_trailPosition = trailPosition;
		updateRealTransform();
	}
}

void GameObject::applyEEEffect(const EEEffect& effect, Vec2& position, Vec2& scale, uint8_t& opacity) {
	const float multiplier = _eeMultiplier;
	
	switch (effect) {
	default:
	case EEEffect::NONE:
		break;
	/*case EEEffect::FADE:
		opacity *= multiplier;
		break;*/
	case EEEffect::SCALE:
		scale.x *= multiplier;
		scale.y *= multiplier;
		break;
	case EEEffect::MOVE_VERTICAL:
		position.y += 170.f * (1 - multiplier);
		opacity *= multiplier;
		break;
	}

	opacity *= (effect == EEEffect::NONE) ? 1.f : multiplier;
}