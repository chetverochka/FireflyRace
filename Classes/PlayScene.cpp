#include "PlayScene.h"
#include "PlayLayer.h"
#include "Level.h"
#include "Definitions.h"
#include "BreakPoint.h"
#include "GameButtons.h"

USING_NS_CC;

bool PlayScene::init() {
	if (!Scene::init()) {
		return false;
	}

	Level level;
	level.APosX = 0.f;
	level.BPosX = 10000.f;

	_playLayer = PlayLayer::createWithLevel(level);
	if (!_playLayer) {
		return false;
	}

	for (int i = 0; i < 98; i++) {
		BreakPoint* bp = BreakPoint::create();
		bp->setTrailPosition(Vec2(150 + i * 100, 0));
		_playLayer->addObject(bp);
	}

	addChild(_playLayer);
	
	initProgressBar();

	Menu* menu = Menu::create({});
	
	SpriteButton* button = new SpriteButton();
	button->autorelease();
	button->init(Sprite::create("fonts/WhiteFont.png"));
	button->setAnchorPoint(Vec2(0, 0));
	button->runAction(RotateBy::create(1.f, 360.f));
	menu->addChild(button);

	addChild(menu, 0);
	
	return true;
}

void PlayScene::onEnterTransitionDidFinish() {
	Scene::onEnterTransitionDidFinish();

	_playLayer->runGame();
}

void PlayScene::initProgressBar() {
    
}