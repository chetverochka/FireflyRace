#ifndef __FIREFLY_H__
#define __FIREFLY_H__

#include "GameObject.h"

class Firefly : public GameObject {
public:
	static const float MOVE_SPEED;
	CREATE_FUNC(Firefly);

	Firefly();
	~Firefly();

	bool init() override;
	void setTrailPosition(const cocos2d::Vec2& trailPosition) override;
	const int getDefaultZOrder() { return 1; }

	void addEffectsNode(cocos2d::Node* parent, const int zOrder);
	void removeEffectsNode();
	
	void updateMoving(float deltaTime);

	virtual void spawnGhost(float deltaTime);
private:
	cocos2d::Node* _effectsNode;
	cocos2d::Sprite* _glowSprite;
	cocos2d::ParticleSystem* _particles;
};

#endif //!__FIREFLY_H__