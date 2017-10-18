#include "Component.h"

Component::Component(componentType argtype, GameObject * argparent) : type(argtype), parent(argparent) {}

bool Component::SetParent(GameObject * parent)
{

	bool ret = false;
	if (parent != nullptr) {
		this->parent = parent;
		ret = true;
	}
	return ret;
}
