#ifndef __PAUSE_LAYER_H__
#define __PAUSE_LAYER_H__

#include "cocos2d.h"

class PauseLayer : public cocos2d::LayerColor {
public:
	CREATE_FUNC(PauseLayer);

	bool init() override;
};

#endif //!__PAUSE_LAYER_H__