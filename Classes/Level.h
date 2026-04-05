#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "CCValue.h"

struct Level {
	Level();

	float APosX;
	float BPosX;

	cocos2d::ValueVector objectsData;
};

#endif //!__LEVEL_H__