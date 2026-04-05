#ifndef __OBJECT_EVENT_PROTOCOL_H__
#define __OBJECT_EVENT_PROTOCOL_H__

#include "math/CCGeometry.h"

class ObjectEventProtocol {
public:
	virtual const cocos2d::Rect& getVisibleArea() const = 0;
};

#endif //!__OBJECT_EVENT_PROTOCOL_H__