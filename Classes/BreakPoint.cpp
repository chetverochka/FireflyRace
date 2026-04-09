#include "BreakPoint.h"

BreakPoint::BreakPoint()

{}

BreakPoint::~BreakPoint() {

}

bool BreakPoint::init() {
	if (!GameObject::init()) {
		return false;
	}

	setTexture("BreakPointObject.png");

	return true;
}