#include "ObjectSprite.h"
#include <cmath>

USING_NS_CC;

ObjectSprite::ObjectSprite() 
	: _eeMultiplier(1.f)
{

}

ObjectSprite::~ObjectSprite() {

}

bool ObjectSprite::init() {
	if (!Sprite::init()) {
		return false;
	}



	return true;
}

void ObjectSprite::onEnter() {
	Sprite::onEnter();
	updateRealTransform();
}

void ObjectSprite::updateRealTransform() {
	Vec2 absolutePosition;
	absolutePosition.x = _trailPosition.x;
	absolutePosition.y = _trailPosition.y;

	Vec2 absoluteScale(1.f, 1.f);
	uint8_t opacity = 255;

	applyEEEffect(EEEffect::SCALE, absolutePosition, absoluteScale, opacity);

	setPosition(absolutePosition);
	setScaleX(absoluteScale.x);
	setScaleY(absoluteScale.y);
	setOpacity(opacity);
}

// update enter+exit effects
void ObjectSprite::updateEEEffects(const Rect& visibleArea, const float enterDistance, const float exitDistance) {
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


void ObjectSprite::setTrailPosition(const cocos2d::Vec2& trailPosition) {
	if (trailPosition != _trailPosition) {
		_trailPosition = trailPosition;
		updateRealTransform();
	}
}

void ObjectSprite::applyEEEffect(const EEEffect& effect, Vec2& position, Vec2& scale, uint8_t& opacity) {
	const float multiplier = _eeMultiplier;
	
	switch (effect) {
	default:
	case EEEffect::NONE:
		break;
	case EEEffect::FADE:
		opacity *= multiplier;
		break;
	case EEEffect::SCALE:
		scale.x *= multiplier;
		scale.y *= multiplier;
		break;
	}
}