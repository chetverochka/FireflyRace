#ifndef __PLAY_SCENE_H__
#define __PLAY_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class PlayLayer;


class PlayScene : public cocos2d::Scene {
public:
	CREATE_FUNC(PlayScene);

	bool init() override;
	void onEnterTransitionDidFinish() override;

private:
	void initProgressBar();

	PlayLayer* _playLayer;
	cocos2d::ui::LoadingBar* _progressBar;
	cocos2d::Label* _progressLabel;
};

#endif //!__PLAY_SCENE_H__