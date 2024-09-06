#include "Koopa.h"
#include "../Game/GameManager.h"
#include "../Game/Constants.h"

Koopa::Koopa(bool dir, const sf::Vector2f& initPos)
	: Enemy(TexID::Koopa, sf::Vector2f(17, 12), AnimationData{3, 3, false, 0.5f})
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetAABB()->Update(sf::Vector2f(GetPosition().x, GetPosition().y));
	AnimatedSprite* animSpr = GetAnimSpr();
	animSpr->SetFrames({ 2, 3, 1 });
	animSpr->ChangeAnim(KoopaAnims::WALK);
}

void Koopa::Reset()
{
	GetAnimSpr()->ChangeAnim(KoopaAnims::WALK);
	Enemy::Reset();
}

void Koopa::Die()
{
	GetAnimSpr()->ChangeAnim(KoopaAnims::COMPRESS);
	SetTimeLeftActive(0.5f);
}

void Koopa::Animate(float deltaTime)
{
	PhysicsController* physCtrl = GetPhysicsController();
	CollisionManager* colMgr = GameManager::GetGameMgr()->GetCollisionMgr();

	GetAnimSpr()->Update(deltaTime);

	SetPrevPosition(GetPosition());

	if (GetDirection() != GetPrevDirection())
		SetXVelocity((GetDirection() ? 1 : -1) * c_moveSpeed * 0.67);

	if (GetOnGround())
	{
		SetYVelocity(0);
	}
	else
	{
		if (physCtrl->GetPhysicsType() != PhysicsType::drop)
			physCtrl->SetFalling();

		IncrementYVelocity(c_gravity);
	}

	if (HasLifes())
	{
		if (GetXVelocity() != 0)
		{
			Move(GetXVelocity() * FPS * deltaTime, 0);
			colMgr->ProcessCollisions(this);
		}

		CheckForHorizontalBounds(deltaTime);

		if (GetYVelocity() != 0)
		{
			Move(0, GetYVelocity() * FPS * deltaTime);
			colMgr->ProcessCollisions(this);
		}
	}
}
