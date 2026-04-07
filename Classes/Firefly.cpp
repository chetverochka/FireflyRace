#include "Firefly.h"
#include "cocos2d.h"

USING_NS_CC;

const float Firefly::MOVE_SPEED = 200.f;

Firefly::Firefly() 
	: _glowSprite(NULL)
	, _particles(NULL)
	, _effectsNode(NULL)
{

}

Firefly::~Firefly() {
	_effectsNode->release();
}

bool Firefly::init() {
	if (!ObjectSprite::init()) {
		return false;
	}

	setTexture("Firefly.png");
	setIgnoreAnchorPointForPosition(false);

	_glowSprite = Sprite::create("FireflyGlow.png");
	if (!_glowSprite) {
		return false;
	}

	_particles = ParticleSystem::create("PL_IconParticles.plist");
	if (!_particles) {
		return false;
	}

	_effectsNode = Node::create();
	if (!_effectsNode) {
		return false;
	}

	_effectsNode->retain();
	_effectsNode->addChild(_particles, 0);
	_effectsNode->addChild(_glowSprite, 1);

	return true;
}

void Firefly::setTrailPosition(const Vec2& trailPosition) {
	ObjectSprite::setTrailPosition(trailPosition);

	_particles->setPosition(trailPosition);
	_glowSprite->setPosition(trailPosition);
}

void Firefly::addEffectsNode(Node* parent, const int zOrder) {
	if (parent && !_effectsNode->getParent()) {
		parent->addChild(_effectsNode, zOrder);
	}
}
void Firefly::removeEffectsNode() {
	if (_effectsNode->getParent()) {
		_effectsNode->removeFromParent();
	}
}

void Firefly::updateMoving(float deltaTime) {
	Vec2 position = getTrailPosition();

	position.x += MOVE_SPEED * deltaTime;

	setTrailPosition(position);
}