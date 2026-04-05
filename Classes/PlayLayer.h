#ifndef __PLAY_LAYER_H__
#define __PLAY_LAYER_H__

#include "cocos2d.h"
#include "ObjectEventProtocol.h"

struct Level;
class ObjectSprite;

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
private:
	ObjectSprite* _APointObject;
	ObjectSprite* _BPointObject;

	cocos2d::Node* _batchNodeBottom; // should be a batch node but -_- i am too lazy to add my sprites into atlas today
	cocos2d::Node* _batchNodeTop; // should be a batch node but -_- i am too lazy to add my sprites into atlas today
	cocos2d::Node* _batchNodeMiddle; // should be a batch node but -_- i am too lazy to add my sprites into atlas today
	cocos2d::Vector<ObjectSprite*> _allObjects;

	cocos2d::Camera* _camera;
};

#endif //!__PLAY_LAYER_H__