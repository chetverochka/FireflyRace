#ifndef __PLAY_LAYER_H__
#define __PLAY_LAYER_H__

#include "cocos2d.h"
#include "ObjectEventProtocol.h"

struct Level;
class GameObject;
class Firefly;
class BreakPoint;

class PlayLayer : public cocos2d::Layer, public ObjectEventProtocol {
public:
	static const cocos2d::CameraFlag CAMERA_FLAG;
	static const unsigned int CAMERA_FLAG_UINT;
	static cocos2d::Scene* scene(Level&);
	static PlayLayer* createWithLevel(Level&);

	CREATE_FUNC(PlayLayer);

	PlayLayer();
	~PlayLayer();

	bool init() override;
	bool initWithData(Level&);

	void update(float deltaTime) override;

	GameObject* createObject(cocos2d::ValueMap&) const;

	bool loadLevel(Level&);
	bool addObject(GameObject*);
	bool removeObject(GameObject*);
	const cocos2d::Rect& getVisibleArea(cocos2d::Camera*) const;
	const cocos2d::Rect& getVisibleArea() const override;

	void setupCameraBetween(const cocos2d::Vec2& left, const cocos2d::Vec2& right, const bool smooth);

	void runGame();
	void stopGame();
	void pauseGame();
	void resumeGame();
private:
	void updateCamera(float deltaTime);

	bool ccTouchDown(cocos2d::Touch* touch, cocos2d::Event* event);
	void ccTouchMove(cocos2d::Touch* touch, cocos2d::Event* event);
	bool ccTouchUp(cocos2d::Touch* touch, cocos2d::Event* event);

	void ccMouseScroll(cocos2d::EventMouse* event);

	GameObject* _APointObject;
	GameObject* _BPointObject;
	GameObject* _currentObject;

	cocos2d::Node* _batchNodeBottom; // should be a batch node but -_- i am too lazy to add my sprites into atlas today
	cocos2d::Node* _batchNodeTop; // should be a batch node but -_- i am too lazy to add my sprites into atlas today
	cocos2d::Node* _batchNodeMiddle; // should be a batch node but -_- i am too lazy to add my sprites into atlas today
	cocos2d::Vector<GameObject*> _allObjects;

	cocos2d::Camera* _camera;

	cocos2d::DrawNode* _AtoBLine;
	Firefly* _firefly;

	cocos2d::Vec2 _previousCameraPosition;
	bool _cameraMoved;
	bool _levelCompleted;
	bool _gameRunning;
	bool _gamePaused;

	cocos2d::Vector<BreakPoint*> _breakPoints;
};

#endif //!__PLAY_LAYER_H__