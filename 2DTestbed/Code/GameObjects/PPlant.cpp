#include "PPlant.h"
#include "../Collisions/Collisions.h"
#include "../Game/Constants.h"

PPlant::PPlant(const sf::Vector2f& initPos)
	: Enemy(TexID::PPlant, TexID::PPlantBB, AnimationData{ 2, 2, true, 1.f })
{
	SetInitialDirection(false);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetBBox()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));
	GetAnimSpr()->SetFrames({ 1, 2 });
}

void PPlant::Reset()
{
	GetAnimSpr()->ChangeAnim(PPlantAnims::SINK);
	Enemy::Reset();
}

void PPlant::Die()
{
	// no way to destroy currently, requires fireplant mario
}

void PPlant::ResolveCollisions(Object* other)
{
}

void PPlant::ResolveCollisions(Tile* tile)
{
}

void PPlant::Animate(float deltaTime)
{
	auto animSpr = GetAnimSpr();

	SetPrevPosition(GetPosition());

	if (GetDirection())
	{
		SetYVelocity(c_jumpSpeed * 0.5f);
	}
	else
	{
		SetYVelocity(-c_jumpSpeed);
	}

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * FPS * deltaTime);
		Collisions::Get()->ProcessCollisions(this);
	}

	sf::Vector2f currentPos = GetPosition();

	if (currentPos.y > 390)
	{
		SetDirection(false);
		animSpr->ChangeAnim(PPlantAnims::SINK);
		//set wait timer
		//go up
	}

	if (currentPos.y < 160)
	{
		SetDirection(true);
		animSpr->ChangeAnim(PPlantAnims::BITING);
		//go down
	}
}
