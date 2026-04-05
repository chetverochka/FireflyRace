#ifndef __OBJECT_SPRITE_H__
#define __OBJECT_SPRITE_H__

#include "cocos2d.h"
#include "ObjectEventProtocol.h"

class ObjectSprite : public cocos2d::Sprite {
public:
	CREATE_FUNC(ObjectSprite);

	ObjectSprite();
	~ObjectSprite();

	bool init() override;

#pragma region SIMPLE_GETTERS_AND_SETTERS
private:
	CC_SYNTHESIZE(int, _key, Key);
	CC_SYNTHESIZE(ObjectEventProtocol*, _eventNotifier, EventNotifier);
private:
#pragma endregion SIMPLE_GETTERS_AND_SETTERS
};

#endif //!__OBJECT_SPRITE_H__