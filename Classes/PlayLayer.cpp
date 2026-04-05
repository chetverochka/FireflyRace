#include "PlayLayer.h"
#include "Level.h"
#include "ObjectSprite.h"

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

	const Size visibleSize = Director::sharedDirector()->getVisibleSize();

	_camera = Camera::createOrthographic(visibleSize.width, visibleSize.height, -1024, 1024);
	_camera->setCameraFlag(CAMERA_FLAG);
	addChild(_camera, 10);

	_APointObject = ObjectSprite::create();
	_BPointObject = ObjectSprite::create();
	_APointObject->setTexture("APointObject.png");
	_BPointObject->setTexture("BPointObject.png");
	addObject(_APointObject);
	addObject(_BPointObject);

	loadLevel(level);

	return true;
}

void PlayLayer::update(float deltaTime) {
	Layer::update(deltaTime);

	const Size visibleSize = Director::sharedDirector()->getVisibleSize();
	const Vec2 targetCameraPos = _APointObject->getPosition();
	Vec2 cameraPos;
	cameraPos.x = targetCameraPos.x - visibleSize.width / 2;
	cameraPos.y = targetCameraPos.y - visibleSize.height / 2;
	_camera->setPosition(cameraPos);
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
	_APointObject->setPositionX(level.APosX);
	_BPointObject->setPositionX(level.BPosX);



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