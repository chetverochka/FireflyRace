#ifndef __FIREFLY_H__
#define __FIREFLY_H__

#include "2d/CCSprite.h"
#include "2d/CCParticleSystem.h"

class Firefly : public cocos2d::Sprite {
public:
	CREATE_FUNC(Firefly);

	Firefly();
	~Firefly();

	bool init() override;

	void addEffectsNode(cocos2d::Node* parent, const int zOrder);
	void removeEffectsNode();
private:
	cocos2d::Node* _effectsNode;
	cocos2d::Sprite* _glowSprite;
	cocos2d::ParticleSystem* _particles;
};

#endif //!__FIREFLY_H__