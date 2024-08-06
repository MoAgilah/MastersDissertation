#include "CollisionManager.h"
#include <format>
#include <algorithm>

#include "../Controller/CtrlMgr.h"

#include "../Game/Camera.h"
#include "../Game/Constants.h"
#include "../Game/GameManager.h"

#include "../GameObjects/Bill.h"
#include "../GameObjects/Box.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Object.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Collectables.h"

namespace
{
	bool IsPlayerObject(TexID id)
	{
		return id == TexID::Mario || id == TexID::Super;
	}

	std::array<TexID, 7> canCollideWithTile =
	{
		TexID::Mario, TexID::Super, TexID::Koopa, TexID::Rex, TexID::Chuck, TexID::Shroom, TexID::Goal
	};

	bool CanCollideWithTile(TexID id)
	{
		return std::find(canCollideWithTile.begin(), canCollideWithTile.end(), id) != canCollideWithTile.end();
	}

	void SortCollidedTiles(std::vector<std::shared_ptr<Tile>> collidedWith)
	{
		std::ranges::sort(collidedWith, [](const std::shared_ptr<Tile>& a, const std::shared_ptr<Tile>& b)
			{
				if (a->GetColNum() == b->GetColNum())
					return a->GetRowNum() < b->GetRowNum();
				return a->GetColNum() > b->GetColNum();
			});
	}

	std::array<TexID, 5> collectableObject =
	{
		TexID::Coin, TexID::YCoin, TexID::ChkPnt, TexID::Shroom, TexID::Goal
	};

	bool IsCollectableObject(TexID id)
	{
		return std::find(collectableObject.begin(), collectableObject.end(), id) != collectableObject.end();
	}

	bool IsDynamicCollectable(TexID id)
	{
		return id == TexID::Shroom || id == TexID::Goal;
	}

	std::array<TexID, 9> dynamicObject =
	{
		TexID::Mario, TexID::Super, TexID::Koopa, TexID::Bill, TexID::Rex, TexID::PPlant, TexID::Chuck, TexID::Shroom
	};

	bool IsDynamicObject(TexID id)
	{
		return std::find(dynamicObject.begin(), dynamicObject.end(), id) != dynamicObject.end();
	}

	std::array<TexID, 5> enemyObject =
	{
		TexID::Koopa, TexID::Bill, TexID::Rex, TexID::PPlant, TexID::Chuck
	};

	bool IsEnemyObject(TexID id)
	{
		return std::find(enemyObject.begin(), enemyObject.end(), id) != enemyObject.end();
	}

	bool IsBoxObject(TexID id)
	{
		return id == TexID::QBox || id == TexID::SBox;
	}

	struct Circle
	{
		Circle(AABB* box)
		{
			center = box->GetPosition() + sf::Vector2f(-2, box->GetExtents().y - 4);
		}

		const float radius = 2;
		Point center;
	};

	float GetXDist(const Point& p1, const Point& p2) { return p2.x - p1.x; }
}

CollisionManager::CollisionManager()
{
	for (auto& tile : m_grid.GetGrid())
	{
		if (tile->GetType() == EMPTY)
			continue;

		m_tiles.push_back(tile);
	}
}

void CollisionManager::AddCollidable(Object* go)
{
	std::shared_ptr<Object> obj;
	obj.reset(go);
	m_collidables.push_back(obj);
}

void CollisionManager::RemoveLastAdded()
{
	m_collidables.pop_back();
}

void CollisionManager::ReplacePlayer(Player * currPlayer)
{
	m_collidables[0].reset(currPlayer);
}

Object* CollisionManager::GetLastAdded()
{
	return m_collidables.back().get();
}

void CollisionManager::Render(sf::RenderWindow& window)
{
	m_grid.Render(window);

	for (auto& tile : m_tiles)
	{
		if (!tile->GetActive())
			continue;

		if (!tile->GetAABB()->GetHit())
			continue;

		tile->GetAABB()->SetFillColour(sf::Color::Transparent);
		tile->GetAABB()->SetOutlineColour(sf::Color::Red);
		tile->Render(window);
		tile->GetAABB()->SetFillColour(sf::Color::White);
		tile->GetAABB()->SetOutlineColour(sf::Color::Black);
	}
}

void CollisionManager::ProcessCollisions(Object* gobj)
{
	if (CanCollideWithTile(gobj->GetID()))
		DynamicObjectToTileCollisions((DynamicObject*)gobj);

	for (auto& collidable : m_collidables)
	{
		if (gobj->GetObjectNum() == collidable->GetObjectNum())
			continue;

		if (!collidable->GetActive())
			continue;

		if (IsPlayerObject(gobj->GetID()))
		{
			PlayerToObjectCollisions((Player*)gobj, collidable.get());
		}
		else
		{
			ObjectToObjectCollisions(gobj, collidable.get());
		}
	}
}

Tile CollisionManager::GetTile(int x, int y)
{
	return *m_grid.GetTile(x, y);
}

std::vector<std::shared_ptr<Tile>> CollisionManager::GetGrid()
{
	return m_grid.GetGrid();
}

std::vector<std::shared_ptr<Object>> CollisionManager::GetCollidables()
{
	return m_collidables;
}

void CollisionManager::DynamicObjectToTileCollisions(DynamicObject* obj)
{
	for (auto& tile : m_tiles)
		tile->GetAABB()->SetHit(false);

	bool collided = false;
	std::vector<std::shared_ptr<Tile>> collidedWith;

	for (auto& tile : m_tiles)
	{
		if (!tile->GetActive())
			continue;

		if (tile->GetType() == DIAGU || tile->GetType() == DIAGD)
		{
			tile->GetAABB()->SetHit(true);
		}
		if (tile->GetAABB()->Intersects(obj->GetAABB()))
		{
			tile->GetAABB()->SetHit(true);
			collidedWith.push_back(tile);
		}
	}

	if (collided = !collidedWith.empty())
	{
		if (obj->GetDirection())
			SortCollidedTiles(collidedWith);

		for (auto& tile : collidedWith)
			if (tile->GetAABB()->Intersects(obj->GetAABB()))
				DynamicObjectToTileResolution(obj, tile.get());
	}

	if (!collided)
		if (obj->GetID() != TexID::Goal)
			obj->SetOnGround(false);
}



void CollisionManager::DynamicObjectToTileResolution(DynamicObject* obj, Tile* tile)
{
	Direction dir = GetFacingDirection(obj);

	float objBottom = obj->GetAABB()->GetPosition().y + obj->GetAABB()->GetExtents().y;
	float tileTop = tile->GetAABB()->GetPosition().y - tile->GetAABB()->GetExtents().y;

	switch (tile->GetType())
	{
	case Types::OWAY:
	case Types::GRND:
		if (dir == DDIR)
			if (objBottom >= tileTop)
				ResolveObjectToBoxTop(obj, tile->GetAABB());
		if (dir == LDIR || dir == RDIR)
			ResolveObjectToBoxTop(obj, tile->GetAABB());
		return;
	case Types::CRN:
		if (dir == DDIR || dir == UDIR)
			if (objBottom > tileTop)
				ResolveObjectToBoxHorizontally(obj, tile->GetAABB());
		return;
	case Types::WALL:
		ResolveObjectToBoxHorizontally(obj, tile->GetAABB());
		return;
	case Types::DIAGU:
	{
		if (dir == DDIR)
		{
			Circle circle(obj->GetAABB());
			Slope slp = obj->GetDirection() ? tile->GetSlope(0, 1) : tile->GetSlope(1, 0);

			if (slp.IsPointAboveLine(circle.center))
			{
				std::cout << "is above the line";
			}
		}

		if (dir == RDIR)
		{
			Circle circle(obj->GetAABB());
			Slope slp = tile->GetSlope(0, 1);
			if (LineToCircle(slp.bgn, slp.end, circle.center, circle.radius))
			{
				auto yOffset = GetYOffSet(GetXDist(slp.bgn, circle.center),
					slp.DistY(),
					slp.bgn.y,
					obj->GetAABB()->GetPosition().y,
					tile->GetTileHeight());

				obj->Move(sf::Vector2f(0, yOffset));
			}
		}

		if (dir == LDIR)
		{
			Circle circle(obj->GetAABB());
			Slope slp = tile->GetSlope(1, 0);

			if (!slp.IsPointAboveLine(circle.center))
			{
				auto yOffset = GetYOffSet(GetXDist(circle.center, slp.bgn),
					slp.DistY(),
					slp.bgn.y,
					obj->GetAABB()->GetPosition().y,
					tile->GetTileHeight());

				obj->Move(sf::Vector2f(0, -yOffset));
			}
		}
		return;
	case Types::DIAGD:
	{
		if (dir == DDIR)
		{
			Circle circle(obj->GetAABB());
			Slope slp = obj->GetDirection() ? tile->GetSlope(1, 0) : tile->GetSlope(0, 1);

			if (slp.IsPointAboveLine(circle.center))
			{
				std::cout << "is above the line";
			}
		}

		if (dir == LDIR)
		{
			Circle circle(obj->GetAABB());
			Slope slp = tile->GetSlope(1, 0);
			if (LineToCircle(slp.bgn, slp.end, circle.center, circle.radius))
			{
				auto yOffset = GetYOffSet(GetXDist(circle.center, slp.bgn),
					slp.DistY(),
					slp.bgn.y,
					obj->GetAABB()->GetPosition().y,
					tile->GetTileHeight());

				obj->Move(sf::Vector2f(0, yOffset));
				obj->SetOnGround(true);
			}
		}

		if (dir == RDIR)
		{
			Circle circle(obj->GetAABB());
			Slope slp = tile->GetSlope(0, 1);

			if (slp.IsPointAboveLine(circle.center))
			{
				auto yOffset = GetYOffSet(GetXDist(slp.bgn, circle.center),
					slp.DistY(),
					slp.bgn.y,
					obj->GetAABB()->GetPosition().y,
					tile->GetTileHeight());

				obj->Move(sf::Vector2f(0, -yOffset));
			}
		}
		return;
	}
	}

	}

	obj->SetOnGround(false);
}

void CollisionManager::PlayerToObjectCollisions(Player* ply, Object* obj)
{
	if (IsBoxObject(obj->GetID()))
	{
		if (obj->Intersects(ply))
		{
			switch (obj->GetID())
			{
			case TexID::QBox:
				PlayerToQBoxResolutions(ply, (QBox*)obj);
				break;
			case TexID::SBox:
				PlayerToSBoxResolutions(ply, (SBox*)obj);
				break;
			}
		}
	}
	else if (IsCollectableObject(obj->GetID()))
	{
		if (obj->Intersects(ply))
		{
			if (IsDynamicCollectable(obj->GetID()))
			{
				((DynamicCollectable*)obj)->Collect(ply);
			}
			else
			{
				((StaticCollectable*)obj)->Collect(ply);
			}
		}
	}
	else if (IsEnemyObject(obj->GetID()))
	{
		Enemy* enemy = (Enemy*)obj;
		if (!enemy->GetIsAlive())
			return;

		if (obj->Intersects(ply))
			PlayerToEnemyResolutions(ply, (Enemy*)obj);
	}
}

void CollisionManager::ObjectToObjectCollisions(Object* obj1, Object* obj2)
{
	if (IsPlayerObject(obj2->GetID()))
	{
		PlayerToObjectCollisions((Player*)obj2, obj1);
	}
	else if (IsBoxObject(obj2->GetID()))
	{
		if (obj2->Intersects(obj1))
		{
			DynamicObject* dynObj = (DynamicObject*)obj1;
			DynamicObjectToBoxResolutions((Direction)GetDirTravelling(dynObj), dynObj, obj2->GetAABB());
		}
	}
	else
	{
		DynamicObjectToDynamicObject((DynamicObject*)obj1, (DynamicObject*)obj2);
	}
}

void CollisionManager::PlayerToQBoxResolutions(Player* ply, QBox* box)
{
	auto dir = (Direction)GetDirTravelling(ply);
	if (dir == Direction::UDIR)
	{
		if (box->GetCanHit())//if not yet been hit
		{
			box->SetJustHit(true);
			GameManager::GetGameMgr()->GetLevel()->AddObject(sf::Vector2f(box->GetPosition().x, (box->GetPosition().y - box->GetOrigin().y * sY) - (box->GetOrigin().y * sY) + 4.f));
			//ply->UpdateFitness(100);
		}
	}

	DynamicObjectToBoxResolutions(dir, ply, box->GetAABB());
}

void CollisionManager::PlayerToSBoxResolutions(Player* ply, SBox* box)
{
	if (box->GetCanHit())//if not yet been hit
	{
		Direction dir = GetDirTravelling(ply);
		if (dir == Direction::UDIR)
		{
			if (!box->GetJustHit())
				box->SetJustHit(true);
		}
		else if (dir == Direction::DDIR)
		{
			if (ply->GetIsSuper() && ply->GetCantSpinJump())
			{
				box->SetJustSmashed(true);
				return;
			}
		}

		DynamicObjectToBoxResolutions(dir, ply, box->GetAABB(), false);
	}
}

void CollisionManager::PlayerToEnemyResolutions(Player* ply, Enemy* enmy)
{
	float pBot = ply->GetAABB()->GetPosition().y + ply->GetAABB()->GetOrigin().y;
	float eTop = enmy->GetAABB()->GetPosition().y - enmy->GetAABB()->GetOrigin().y;

	if (ply->GetIsAlive())
		return;

	if (pBot > eTop || enmy->GetID() == TexID::PPlant)
	{
		if (!ply->GetIfInvulnerable())
		{
			if (ply->GetIsSuper())
			{
				ply->JustBeenHit(true);
				ply->SetIsSuper(false);
				//ptmp->UpdateFitness(-100);
			}
			else
			{
				ply->SetIsAlive(false);
			}
		}
	}
	else
	{
		//set hover time
		ply->JusyHitEnemy(0.1f);
		enmy->DecrementLife();
		//ptmp->UpdateFitness(-100);
	}
}

void CollisionManager::DynamicObjectToBoxResolutions(Direction dirOfTravel, DynamicObject* obj, AABB* box, bool resolveUpDir)
{
	switch (dirOfTravel)
	{
	case UDIR:
		if (resolveUpDir)
			ResolveObjectToBoxBottom(obj, box);
		break;
	case DDIR:
		ResolveObjectToBoxTop(obj, box);
		break;
	case LDIR:
	case RDIR:
		ResolveObjectToBoxHorizontally(obj, box);
		break;
	}
}

void CollisionManager::DynamicObjectToDynamicObject(DynamicObject* obj1, DynamicObject* obj2)
{
	float tFirst, tLast = 0;
	if (obj1->GetAABB()->IntersectsMoving(obj2->GetAABB(), obj1->GetVelocity(), obj2->GetVelocity(), tFirst, tLast))
	{
		obj1->SetPosition(
		std::lerp(obj1->GetPrevPostion().x, obj1->GetPosition().y, tFirst),
		std::lerp(obj1->GetPrevPostion().x, obj1->GetPosition().y, tFirst));
		obj1->GetAABB()->Update(obj1->GetPosition());
		obj1->SetDirection(!obj1->GetDirection());

		obj2->SetPosition(
			std::lerp(obj2->GetPrevPostion().x, obj2->GetPosition().y, tFirst),
			std::lerp(obj2->GetPrevPostion().x, obj2->GetPosition().y, tFirst));
		obj2->GetAABB()->Update(obj2->GetPosition());
		obj2->SetDirection(!obj2->GetDirection());
	}
}

void CollisionManager::ResolveObjectToBoxTop(DynamicObject* obj, AABB* box)
{
	obj->Move(0, -box->GetOverlap().y);
	obj->SetOnGround(true);
}

void CollisionManager::ResolveObjectToBoxBottom(DynamicObject* obj, AABB* box)
{
	obj->Move(0, box->GetOverlap().y);
	if (IsPlayerObject(obj->GetID()))
		((Player*)obj)->SetAirbourne(false);
}

void CollisionManager::ResolveObjectToBoxHorizontally(DynamicObject* obj, AABB* box)
{
	obj->Move((obj->GetDirection() ? -1 : 1) * box->GetOverlap().x, 0);
	if (!IsPlayerObject(obj->GetID()))
		obj->SetDirection(!obj->GetDirection());
}

Direction CollisionManager::GetDirTravelling(DynamicObject* obj)
{
	//direction travelling
	sf::Vector2f dirV = obj->GetPosition() - obj->GetPrevPostion();
	Direction dir = DDIR;

	//if movement in x
	if (dirV.x != 0)
	{
		if (dirV.x > 0)	dir = RDIR;
		else dir = LDIR;
	}

	//if movement in y
	if (dirV.y != 0)
	{
		if (dirV.y > 0)	dir = DDIR;
		else dir = UDIR;
	}

	return dir;
}

Direction CollisionManager::GetFacingDirection(DynamicObject* obj)
{
	const sf::Vector2f& currentVel = obj->GetVelocity();

	Direction currentDir = DDIR;

	if (currentVel.x != 0.f || currentVel.y != 0.f)
	{
		float vxa = std::abs(currentVel.x);
		float vya = std::abs(currentVel.y);

		if (vxa > vya)
		{
			currentDir = (currentVel.x < 0) ?
				Direction::LDIR : Direction::RDIR;
		}
		else
		{
			currentDir = (currentVel.y < 0) ?
				Direction::UDIR : Direction::DDIR;
		}
	}

	return currentDir;
}

bool CollisionManager::LineToCircle(const Point& start, const Point& end, const Point& center, float radius)
{
	if (start.PointToCircle(center, radius))
		return true;

	if (end.PointToCircle(center, radius))
		return true;

	// get length of the line
	auto len = pnt::distance(start, end);

	// get dot product of the line and circle
	float dot = (((center.x - start.x) * (end.x - start.x)) + ((center.y - start.y) * (end.y - start.y))) / std::pow(len, 2);

	// find the closest point on the line
	Point closestPnt = start + (dot * (end - start));

	// is this point actually on the line segment?
	// if so keep going, but if not, return false
	if (!closestPnt.PointToLineSegmentIntersects(start, end))
		return false;

	// get distance to closest point
	float distance = pnt::distance(center, closestPnt);

	return distance <= radius;
}

float CollisionManager::GetYOffSet(float pDistX, float lDistY, float slopeY, float currY, float tileHeight)
{
	auto percent = pDistX / lDistY;
	auto colHeight = lDistY * percent + slopeY;
	return ((currY - colHeight) / 16);
}


