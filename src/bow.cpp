#include "bow.hpp"
#include "body.hpp"
#include "gameobjectgod.hpp"
#include "arrow.hpp"

Bow::Bow(Body* user)
{
	mUser = user;
}

Bow::~Bow()
{
}

void Bow::ready()
{
}

void Bow::attack()
{
	Arrow* arrow = new Arrow();
	arrow->setRotation(mUser->getRotation());
	arrow->setPosition(mUser->getPosition());
	getGod()->manage(arrow);
}
