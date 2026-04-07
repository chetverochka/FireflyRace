#include "PlayLayer.h"
#include "Level.h"
#include "ObjectSprite.h"
#include "Firefly.h"

USING_NS_CC;

const cocos2d::CameraFlag	PlayLayer::CAMERA_FLAG		 = cocos2d::CameraFlag::USER1;
const unsigned int			PlayLayer::CAMERA_FLAG_UINT	 = (unsigned int)(CAMERA_FLAG);

Scene* PlayLayer::scene(Level& level) {
	PlayLayer* layer = new(std::nothrow) PlayLayer();

	if (layer && layer->initWithData(level)) {
		layer->autorelease();

		Scene* scene = Scene::create();
		if (scene) {
			scene->addChild(layer);
			scene->getDefaultCamera()->setVisible(false);
			return scene;
		}
	}

	CC_SAFE_DELETE(layer);
	return NULL;
}

PlayLayer::PlayLayer() 
	: _APointObject(NULL)
	, _BPointObject(NULL)
	, _batchNodeBottom(NULL)
	, _batchNodeTop(NULL)
	, _batchNodeMiddle(NULL)
	, _allObjects({})
	, _AtoBLine(NULL)
	, _levelCompleted(false)
{

}

PlayLayer::~PlayLayer() {

}

bool PlayLayer::init() {
	Level level;
	return initWithData(level);
}

bool PlayLayer::initWithData(Level& level) {
	if (!Layer::init()) {
		return false;
	}

	scheduleUpdate();

	_batchNodeBottom = Node::create();
	addChild(_batchNodeBottom, -1);

	_batchNodeTop = Node::create();
	addChild(_batchNodeTop, 1);

	_batchNodeMiddle = Node::create();
	addChild(_batchNodeMiddle, 0);

	_AtoBLine = DrawNode::create();
	_AtoBLine->drawLine(Vec2(level.APosX, 0), Vec2(level.BPosX, 0), Color4F(1,1,1,0.1f));
	addChild(_AtoBLine, -2);

	const Size visibleSize = Director::sharedDirector()->getVisibleSize();

	_camera = Camera::createOrthographic(visibleSize.width, visibleSize.height, -1024, 1024);
	_camera->setCameraFlag(CAMERA_FLAG);
	addChild(_camera, 10);

	_previousCameraPosition = _camera->getPosition();
	_cameraMoved = true;

	_APointObject = ObjectSprite::create();
	_BPointObject = ObjectSprite::create();
	_APointObject->setTexture("APointObject.png");
	_BPointObject->setTexture("BPointObject.png");
	addObject(_APointObject);
	addObject(_BPointObject);

	_currentObject = _APointObject;

	loadLevel(level);

	_firefly = Firefly::create();
	_firefly->addEffectsNode(this, -10);
	addChild(_firefly, 10);

	setCameraMask(CAMERA_FLAG_UINT, true);

	setupCameraBetween(_APointObject->getPosition(), _BPointObject->getPosition(), true);

	return true;
}

void PlayLayer::update(float deltaTime) {
	Layer::update(deltaTime);

	for (int i = 0; i < _allObjects.size(); i++) {
		ObjectSprite* object = _allObjects.at(i);

		if (_cameraMoved) {
			object->updateEEEffects(getVisibleArea(), 100.f, 100.f);
		}
	}

	if (!_levelCompleted) {
		_firefly->updateMoving(deltaTime);
	}

	const bool fireflyFinished = _firefly->getTrailPosition().x >= _BPointObject->getTrailPosition().x;
	if (fireflyFinished && !_levelCompleted) {
		_levelCompleted = true;
		// this->onLevelCompleted();
		CCLOG("Level Completed!");
	}

	// update camera
	updateCamera(deltaTime);
}

ObjectSprite* PlayLayer::createObject(cocos2d::ValueMap& values) const {
	const int id = values.count("id") > 0 ? values["id"].asInt() : 0;

	ObjectSprite* object = NULL;

	switch (id) {
	default:
	case 0:
		object = ObjectSprite::create();
		break;
	case 1: // [ A ] Object
	case 2: // [ B ] Object
		object = ObjectSprite::create();
		break;
	}

	if (object) {
		object->setKey(id);
	}

	return object;
}

bool PlayLayer::loadLevel(Level& level) {
	_APointObject->setTrailPosition(Vec2(level.APosX, 0.f));
	_BPointObject->setTrailPosition(Vec2(level.BPosX, 0.f));

	return true;
}

bool PlayLayer::addObject(ObjectSprite* object) {
	if (_allObjects.contains(object)) {
		return false;
	}
	_allObjects.pushBack(object);
	
	if (!object->getParent()) {
		_batchNodeBottom->addChild(object);
	}

	object->setEventNotifier(this);
	object->setCameraMask(CAMERA_FLAG_UINT, true);
	
	return true;
}

bool PlayLayer::removeObject(ObjectSprite* object) {
	if (!_allObjects.contains(object)) {
		return false;
	}
	_allObjects.eraseObject(object);

	Vector<Node*> approvedParents = {
		_batchNodeBottom,
		_batchNodeMiddle,
		_batchNodeTop
	};

	for (Node* parent : approvedParents) {
		if (object->getParent() == parent) {
			parent->removeChild(object);
			break;
		}
	}

	object->setEventNotifier(NULL);

	return true;
}

const cocos2d::Rect& PlayLayer::getVisibleArea(cocos2d::Camera* camera) const {
	// static is too dangerous here
	static const Size visibleSize = Director::sharedDirector()->getVisibleSize(); 
	static float zoomX, zoomY;

	zoomX = 1.f;
	zoomY = 1.f;

	Rect ret;
	ret.size = Size(visibleSize.width * zoomX, visibleSize.height * zoomY);
	ret.origin = camera->getPosition() - (ret.size / 2);

	return ret;
}

const cocos2d::Rect& PlayLayer::getVisibleArea() const {
	return getVisibleArea(_camera);
}

void PlayLayer::setupCameraBetween(const cocos2d::Vec2& left, const cocos2d::Vec2& right, const bool smooth) {

	const Rect& visibleArea = getVisibleArea();

	Vec2 center;
	center.x = (left.x + right.x) / 2;
	center.y = (left.y + right.y) / 2;

	const float padding = 250.f;
	const float lengthBetween = right.x - left.x + padding * 2;
	const float zoom = lengthBetween / visibleArea.size.width;

	Vec2 cameraPos(center - visibleArea.size / 2 * zoom);

	const int smoothTag = 1;
	_camera->stopAllActionsByTag(smoothTag);

	if (smooth) {
		const float duration = 1.f;

		ActionInterval* moveAction, *scaleAction, *spawnAction;
		moveAction = EaseCubicActionInOut::create(MoveTo::create(duration, Vec3(cameraPos.x, cameraPos.y, _camera->getPositionZ())));
		scaleAction = EaseCubicActionInOut::create(ScaleTo::create(duration, zoom));
		spawnAction = Spawn::createWithTwoActions(moveAction, scaleAction);
		spawnAction->setTag(smoothTag);

		_camera->runAction(spawnAction);
	}
	else {
		_camera->setPosition(cameraPos);
		_camera->setScale(zoom);
	}
}

void PlayLayer::updateCamera(float deltaTime) {
	Vec2 currentPosition = _camera->getPosition();

	_cameraMoved = false;
	if (_previousCameraPosition != currentPosition) {
		_cameraMoved = true;
	}

	_previousCameraPosition = currentPosition;
}