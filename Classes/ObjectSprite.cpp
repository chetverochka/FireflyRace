#include "ObjectSprite.h"

ObjectSprite::ObjectSprite() 

{

}

ObjectSprite::~ObjectSprite() {

}

bool ObjectSprite::init() {
	if (!Sprite::init()) {
		return false;
	}



	return true;
}