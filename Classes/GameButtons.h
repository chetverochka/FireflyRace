#ifndef __GAME_BUTTONS_H__
#define __GAME_BUTTONS_H__

#include "cocos2d.h"
#include "ui/UIWidget.h"

class GameButton : public cocos2d::MenuItem {
public:
	static const float ACTION_DURATION;
	static const int ACTION_TAG;
	static const float SELECT_SCALE_VALUE;

	CREATE_FUNC(GameButton);
	~GameButton();

	bool init() override;
	void onEnter() override;
	void onExit() override;
	cocos2d::Rect rect() const override;
	void setContentSize(const cocos2d::Size& size) override;
	void setAnchorPoint(const cocos2d::Vec2& anchorPoint) override;
	void activate() override;
	void selected() override;
	void unselected() override;
	void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;

	cocos2d::Node* getInternalNode();
private:
	cocos2d::Node* _internalNode = NULL;
	cocos2d::CustomCommand _customDrawCommand;
};

class SpriteButton : public GameButton {
public:
	CREATE_FUNC(SpriteButton);

	bool init() override;
	bool init(std::string frameName, cocos2d::ui::Widget::TextureResType resType);
	bool init(cocos2d::Sprite* sprite); // main constructor
	void setContentSize(const cocos2d::Size& size) override;
private:
	cocos2d::Sprite* _sprite;
};

#endif //!__GAME_BUTTONS_H__