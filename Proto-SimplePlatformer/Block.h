#pragma once

#include "KinematicObject.h"

namespace agp
{
	class Block;
}

class agp::Block : public KinematicObject
{
protected:
	CollidableObject* _watched;


public:

	Block(Scene* scene, const RectF& rect, Sprite* sprite, CollidableObject* watched, int layer = 0);

	virtual void update(float dt) override;

	virtual std::string name() override {
		return strprintf("Block[%d]", _id);
	}
};