#ifndef __FIREFLY_H__
#define __FIREFLY_H__

#include "ObjectSprite.h"

class Firefly : public ObjectSprite {
public:
	static const float MOVE_SPEED;
	CREATE_FUNC(Firefly);

	Firefly();
	~Firefly();

	bool init() override;
	void setTrailPosition(const cocos2d::Vec2& trailPosition) override;

	void addEffectsNode(cocos2d::Node* parent, const int zOrder);
	void removeEffectsNode();
	
	void updateMoving(float deltaTime);
private:
	cocos2d::Node* _effectsNode;
	cocos2d::Sprite* _glowSprite;
	cocos2d::ParticleSystem* _particles;
};

#endif //!__FIREFLY_H__