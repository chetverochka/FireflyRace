#include "PlayLayer.h"
#include "Level.h"
#include "GameObject.h"
#include "Firefly.h"
#include "BreakPoint.h"

USING_NS_CC;

const cocos2d::CameraFlag	PlayLayer::CAMERA_FLAG		 = cocos2d::CameraFlag::USER1;
const unsigned int			PlayLayer::CAMERA_FLAG_UINT	 = (unsigned int)(CAMERA_FLAG);

Scene* PlayLayer::scene(Level& level) {
	PlayLayer* layer = new(std::nothrow) PlayLayer();

	if (layer && layer->initWithData(level)) {
		layer->autorelease();

		
		for (int i = 0; i < 4; i++) {
			BreakPoint* bp = BreakPoint::create();
			bp->setTrailPosition(Vec2(150 + i * 100, 0));
			layer->addObject(bp);
		}

		layer->runGame();

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

PlayLayer* PlayLayer::createWithLevel(Level& level) {
	PlayLayer* ret = new(std::nothrow) PlayLayer();

	if (ret && ret->initWithData(level)) {
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
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
	, _gameRunning(false)
	, _gamePaused(false)
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

	EventListenerTouchOneByOne* touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(PlayLayer::ccTouchDown, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(PlayLayer::ccTouchMove, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(PlayLayer::ccTouchUp, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	EventListenerMouse* mouseListener = EventListenerMouse::create();
	mouseListener->onMouseScroll = CC_CALLBACK_1(PlayLayer::ccMouseScroll, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);

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

	_APointObject = GameObject::create();
	_BPointObject = GameObject::create();
	_APointObject->setTexture("APointObject.png");
	_BPointObject->setTexture("BPointObject.png");
	addObject(_APointObject);
	addObject(_BPointObject);

	_currentObject = _APointObject;

	loadLevel(level);

	_firefly = Firefly::create();
	_firefly->addEffectsNode(this, -10);
	addObject(_firefly);

	setCameraMask(CAMERA_FLAG_UINT, true);

	setupCameraBetween(_APointObject->getPosition(), _BPointObject->getPosition(), true);

	return true;
}

void PlayLayer::update(float deltaTime) {
	Layer::update(deltaTime);

	for (int i = 0; i < _allObjects.size(); i++) {
		GameObject* object = _allObjects.at(i);

		if (_cameraMoved) {
			object->updateEEEffects(getVisibleArea(), 50.f, 50.f);
		}
	}

	if (!_levelCompleted && _gameRunning && !_gamePaused) {
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

GameObject* PlayLayer::createObject(cocos2d::ValueMap& values) const {
	const int id = values.count("id") > 0 ? values["id"].asInt() : 0;

	GameObject* object = NULL;

	switch (id) {
	default:
	case 0:
		object = GameObject::create();
		break;
	case 1: // [ A ] Object
	case 2: // [ B ] Object
		object = GameObject::create();
		break;
	case 3:
		object = BreakPoint::create();
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

bool PlayLayer::addObject(GameObject* object) {
	if (_allObjects.contains(object)) {
		return false;
	}
	_allObjects.pushBack(object);
	
	if (!object->getParent()) {
		_batchNodeBottom->addChild(object, object->getDefaultZOrder());
	}
	else {
		object->setLocalZOrder(object->getDefaultZOrder());
	}

	object->setEventNotifier(this);
	object->setCameraMask(CAMERA_FLAG_UINT, true);
	
	BreakPoint* breakPoint = dynamic_cast<BreakPoint*>(object);
	if (breakPoint) {
		_breakPoints.pushBack(breakPoint);
	}

	return true;
}

bool PlayLayer::removeObject(GameObject* object) {
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

	BreakPoint* breakPoint = dynamic_cast<BreakPoint*>(object);
	if (breakPoint) {
		_breakPoints.eraseObject(breakPoint);
	}

	return true;
}

const cocos2d::Rect& PlayLayer::getVisibleArea(cocos2d::Camera* camera) const {
	// static is too dangerous here
	static const Size visibleSize = Director::sharedDirector()->getVisibleSize(); 
	
	Rect ret;
	ret.size = Size(visibleSize.width * camera->getScaleX(), visibleSize.height * camera->getScaleY());
	ret.origin = camera->getPosition();

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

bool PlayLayer::ccTouchDown(cocos2d::Touch* touch, cocos2d::Event* event) {
	return true;
}
void PlayLayer::ccTouchMove(cocos2d::Touch* touch, cocos2d::Event* event) {
	const Vec2 touchDelta = touch->getDelta();

	_camera->setPosition(_camera->getPosition() - touchDelta * _camera->getScale());
}
bool PlayLayer::ccTouchUp(cocos2d::Touch* touch, cocos2d::Event* event) {
	return true;
}

void PlayLayer::ccMouseScroll(cocos2d::EventMouse* event) {
	const float scroll = event->getScrollY();

	const Vec2 previousPosition = _camera->getPosition();
	const float previousScale = _camera->getScale();
	const float newScale = previousScale + scroll * 0.06f;

	_camera->setScale(newScale);

	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Size sizeDiff = (visibleSize * newScale) - (visibleSize * previousScale);

	Vec2 newPosition = previousPosition - (sizeDiff / 2);
	_camera->setPosition(newPosition);
}


static void sortByTrailX(cocos2d::Vector<GameObject*>& objects) {
	std::sort(objects.begin(), objects.end(), [](GameObject* a, GameObject* b) {
		return a->getTrailPosition().x > b->getTrailPosition().x;
		});
}

void PlayLayer::runGame() {
	if (_gameRunning)
		return;
	_gameRunning = true;

#pragma region 0
	/*
		при старте игры определяется порядок объектов которые игрок должен преодлеть, 
		для этого все нужные объекты идут в массив, сортируются по возростанию X и
		указывается нулевой индекс текущего объекта
	*/
	cocos2d::Vector<GameObject*> sequence;
	for (int i = 0; i < _breakPoints.size(); i++)
		sequence.pushBack(_breakPoints.at(i));
	sortByTrailX(sequence);
	sequence.insert(0, _APointObject);
	sequence.pushBack(_BPointObject);
#pragma endregion 0


}

void PlayLayer::stopGame() {
	if (!_gameRunning)
		return;

	_gameRunning = false;
}

void PlayLayer::pauseGame() {
	if (_gamePaused)
		return;

	_gamePaused = true;
}

void PlayLayer::resumeGame() {
	if (!_gamePaused)
		return;

	_gamePaused = false;
}