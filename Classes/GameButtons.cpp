#include "GameButtons.h"

USING_NS_CC;

#pragma region GAME_BUTTON

const float GameButton::ACTION_DURATION		= 0.4f;
const int	GameButton::ACTION_TAG			= 1;
const float GameButton::SELECT_SCALE_VALUE	= 1.2f;

GameButton::~GameButton(){
	_internalNode->release();
	// TODO: add _internalNode->removeFromParent() to 
	// prevent memory leaks if _internalNode was attached 
	// to another node from outside through getInternalNode();
}

bool GameButton::init() {
	if (!MenuItem::initWithCallback(NULL)) {
		return false;
	}

	setCascadeColorEnabled(true);
	setCascadeOpacityEnabled(true);

	_internalNode = Node::create();
	if (!_internalNode) {
		return false;
	}
	_internalNode->setAnchorPoint(_anchorPoint);
	_internalNode->retain();

	return true;
}

void GameButton::onEnter() {
	MenuItem::onEnter();
	_internalNode->onEnter();
}

void GameButton::onExit() {
	MenuItem::onExit();
	_internalNode->onExit();
}

cocos2d::Rect GameButton::rect() const {
	return Rect(_position.x - _contentSize.width * _anchorPoint.x,
		_position.y - _contentSize.height * _anchorPoint.y,
		_contentSize.width, _contentSize.height);
}


void GameButton::setContentSize(const cocos2d::Size& size) {
	MenuItem::setContentSize(size);
	_internalNode->setContentSize(size);
	_internalNode->setPosition(size / 2);
}

void GameButton::setAnchorPoint(const cocos2d::Vec2& anchorPoint) {
	MenuItem::setAnchorPoint(anchorPoint);
	/*if (_internalNode) {
		_internalNode->setAnchorPoint(anchorPoint);
	}*/
}

void GameButton::activate() {
	MenuItem::activate();
}

void GameButton::selected() {
	MenuItem::selected();

	_internalNode->stopActionByTag(ACTION_TAG);

	ActionInterval* action = EaseBounceOut::create(ScaleTo::create(ACTION_DURATION, SELECT_SCALE_VALUE));
	action->setTag(ACTION_TAG);
	_internalNode->runAction(action);
}

void GameButton::unselected() {
	MenuItem::unselected();

	_internalNode->stopActionByTag(ACTION_TAG);

	ActionInterval* action = EaseBounceOut::create(ScaleTo::create(ACTION_DURATION, 1.0f));
	action->setTag(ACTION_TAG);
	_internalNode->runAction(action);
}

void GameButton::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) {
	MenuItem::draw(renderer, transform, flags);
	_internalNode->visit(renderer, transform, flags);

	// debug draw (DELETE THIS)
	DrawNode* draw = DrawNode::create();
	draw->drawRect(Vec2(0, 0), _internalNode->getContentSize(), Color4F::WHITE);
	draw->setAnchorPoint(_internalNode->getAnchorPoint());
	draw->visit(renderer, transform, flags);
}

Node* GameButton::getInternalNode() {
	return _internalNode;
}

#pragma endregion GAME_BUTTON

#pragma region SPRITE_BUTTON

bool SpriteButton::init() {
	return init(Sprite::create());
}

bool SpriteButton::init(std::string frameName, ui::Widget::TextureResType resType) {
	switch (resType)
	{
	case ui::Widget::TextureResType::LOCAL: return init(Sprite::create(frameName));
	case ui::Widget::TextureResType::PLIST: return init(Sprite::createWithSpriteFrameName(frameName));
	default: return init(NULL);
	}
}

bool SpriteButton::init(Sprite* sprite) {
	if (!GameButton::init()) {
		return false;
	}

	if (!sprite || sprite->getParent()) {
		return false;
	}

	getInternalNode()->addChild(sprite);

	_sprite = sprite;
	setContentSize(sprite->getContentSize());

	return true;
}

void SpriteButton::setContentSize(const cocos2d::Size& size) {
	GameButton::setContentSize(size);

	_sprite->setContentSize(size);
	_sprite->setPosition(size / 2);
}

#pragma endregion SPRITE_BUTTON