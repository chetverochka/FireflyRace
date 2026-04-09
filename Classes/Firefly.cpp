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
	if (!GameObject::init()) {
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
	
	schedule([this](float dt) { spawnGhost(dt); }, 0.1f, "spawn_ghost");
	return true;
}

void Firefly::setTrailPosition(const Vec2& trailPosition) {
	GameObject::setTrailPosition(trailPosition);

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

void Firefly::spawnGhost(float deltaTime) {
	const float size = 150.f;

	DrawNode* rect = DrawNode::create();
	rect->drawRect(Vec2(0, 0), Vec2(size, size), Color4F(1.f,1.f,1.f, 0.1f));
	rect->setContentSize(Size(size, size));
	rect->setAnchorPoint(Vec2(0.5, 0.5));
	rect->setPosition(getPosition());
	rect->runAction(Spawn::createWithTwoActions(Sequence::create({ FadeOut::create(0.3f), RemoveSelf::create() }), ScaleTo::create(1.3f, 0.f)));
	_effectsNode->addChild(rect, 2);

	rect->setCameraMask(getCameraMask());
}