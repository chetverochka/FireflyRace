#ifndef __BREAKPOINT_H__
#define __BREAKPOINT_H__

#include "GameObject.h"

class BreakPoint : public GameObject {
public:
	CREATE_FUNC(BreakPoint);

	BreakPoint();
	~BreakPoint();

	bool init() override;
};

#endif //!__BREAKPOINT_H__