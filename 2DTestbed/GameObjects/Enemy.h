#ifndef EnemyH
#define EnemyH

#include "../GameObjects/GameObject.h"

class Camera;
class Enemy : public GameObject
{
public:
	Enemy(std::string filepath, int rows, int cols, float fps, int bTyp, int strloc = 1, bool dir = true, bool symmetrical = true, int initAnim = 0, float animSpd = 1);
	~Enemy();
	virtual void Update(float deltaTime);
	virtual void Render(sf::RenderWindow& window);
	int DecrementLife();
	void ResetLives();
	virtual void Die() = 0;
	bool GetIsAlive();
	virtual void Change();
	BoundingBox *GetBBox();

	int GetLives();
	virtual void Revive();
	int GetEnemyNum();
protected:
	virtual void Animate(float deltaTime) = 0;
	virtual void Reset();


	float m_tillReset;
	bool resetAllowed;

	float m_airtime;
	const float c_maxAirTime = 1.0f;
	bool m_crouched;

	int numLives;
	int maxLives;
	bool m_alive;

	float timeLeftActive;

	BoundingBox* m_CurBox;

	int m_enemyNum;
};

#endif