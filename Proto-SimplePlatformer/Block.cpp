#include "Block.h"

using namespace agp;

Block::Block(Scene* scene, const RectF& rect, Sprite* sprite, CollidableObject* watched, int layer) :
	KinematicObject(scene, rect, sprite, layer)
{
	_compenetrable = false;
	_watched = watched;
}

void Block::update(float dt)
{
	KinematicObject::update(dt);
}