#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "cocos2d.h"
#include "ObjectEventProtocol.h"

class GameObject : public cocos2d::Sprite {
public:
	enum EEEffect { //Enter+Exit effect
		NONE,
		SCALE,
		FADE,
		MOVE_VERTICAL
	};

	CREATE_FUNC(GameObject);

	GameObject();
	~GameObject();

	bool init() override;
	void onEnter() override;
 
	bool isPlayable() { return (bool)(_eventNotifier); }

	void updateRealTransform();
	void updateEEEffects(const cocos2d::Rect& visibleArea, const float enterDistance, const float exitDistance); // update enter+exit effects
	// public setters
	virtual void setTrailPosition(const cocos2d::Vec2& trailPosition);

	// public getters
	inline const cocos2d::Vec2& getTrailPosition() const { return _trailPosition; }

	virtual const int getDefaultZOrder() { return 0; }
protected:
	void applyEEEffect(const EEEffect& effect, cocos2d::Vec2& position, cocos2d::Vec2& scale, uint8_t& opacity);
private:
	cocos2d::Vec2 _trailPosition; // указывает реальное положение объекта на пути
	float _eeMultiplier; // enter+exit effect transform multiplier
#pragma region SIMPLE_GETTERS_AND_SETTERS
private:
	CC_SYNTHESIZE(int, _key, Key);
	CC_SYNTHESIZE(ObjectEventProtocol*, _eventNotifier, EventNotifier);
	//CC_SYNTHESIZE(float, _realPositionX, RPositionX);
private:
#pragma endregion SIMPLE_GETTERS_AND_SETTERS
};

#endif //!__GAME_OBJECT_H__