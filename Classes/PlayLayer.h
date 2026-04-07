#ifndef __PLAY_LAYER_H__
#define __PLAY_LAYER_H__

#include "cocos2d.h"
#include "ObjectEventProtocol.h"

struct Level;
class ObjectSprite;
class Firefly;

class PlayLayer : public cocos2d::Layer, public ObjectEventProtocol {
public:
	static const cocos2d::CameraFlag CAMERA_FLAG;
	static const unsigned int CAMERA_FLAG_UINT;
	static cocos2d::Scene* scene(Level&);

	CREATE_FUNC(PlayLayer);

	PlayLayer();
	~PlayLayer();

	bool init() override;
	bool initWithData(Level&);

	void update(float deltaTime) override;

	ObjectSprite* createObject(cocos2d::ValueMap&) const;

	bool loadLevel(Level&);
	bool addObject(ObjectSprite*);
	bool removeObject(ObjectSprite*);
	const cocos2d::Rect& getVisibleArea(cocos2d::Camera*) const;
	const cocos2d::Rect& getVisibleArea() const override;

	void setupCameraBetween(const cocos2d::Vec2& left, const cocos2d::Vec2& right, const bool smooth);
private:
	void updateCamera(float deltaTime);

	ObjectSprite* _APointObject;
	ObjectSprite* _BPointObject;
	ObjectSprite* _currentObject;

	cocos2d::Node* _batchNodeBottom; // should be a batch node but -_- i am too lazy to add my sprites into atlas today
	cocos2d::Node* _batchNodeTop; // should be a batch node but -_- i am too lazy to add my sprites into atlas today
	cocos2d::Node* _batchNodeMiddle; // should be a batch node but -_- i am too lazy to add my sprites into atlas today
	cocos2d::Vector<ObjectSprite*> _allObjects;

	cocos2d::Camera* _camera;

	cocos2d::DrawNode* _AtoBLine;
	Firefly* _firefly;

	cocos2d::Vec2 _previousCameraPosition;
	bool _cameraMoved;
	bool _levelCompleted;
};

#endif //!__PLAY_LAYER_H__