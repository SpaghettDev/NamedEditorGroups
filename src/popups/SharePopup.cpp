#include "SharePopup.hpp"

SharePopup* SharePopup::create()
{
    auto ret = new SharePopup();

	if (ret && ret->initAnchored(150.f, 135.f))
		ret->autorelease();
	else
	{
		delete ret;
		ret = nullptr;
	}

	return ret;
}

bool SharePopup::setup()
{
    
    return true;
}
