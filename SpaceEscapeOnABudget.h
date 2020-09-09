#pragma once

#include <vector>
#include <cmath>

#include "resource.h"

#define lerp(s, e, l) s + (e - s) * l;

#define RePaint() InvalidateRect(hWnd, NULL, FALSE);

#define DrawRect(l, t, r, b, c) { RECT rt = { l, t, r, b }; HBRUSH br = CreateSolidBrush(c); FrameRect(hdc, &rt, br); DeleteObject(br); }
#define FillRect(l, t, r, b, c) { RECT rt = { l, t, r, b }; HBRUSH br = CreateSolidBrush(c); FillRect(hdc, &rt, br); DeleteObject(br); }

HWND hWnd;

BOOL MouseDown, Alive;

INT
width  = 1280,
height = 720;

POINT Mouse = { width / 6, height / 2 };

#pragma region Player

INT PlayerSpeed = 14;

POINT
PlayerPos  = { width / 6, height / 2 },
PlayerSize = { 100, 50 };

void UpdatePlayer()
{
	PlayerPos.y = (int) lerp((float) PlayerPos.y, (float) Mouse.y, 0.14f);
}

void DrawPlayer(HDC hdc)
{
	DrawRect
	(
		PlayerPos.x - PlayerSize.x / 2, PlayerPos.y - PlayerSize.y / 2,
		PlayerPos.x + PlayerSize.x / 2, PlayerPos.y + PlayerSize.y / 2,
		0x00FF00
	);
}

#pragma endregion

#pragma region Enemy

class Enemy;
std::vector<Enemy*> Enemies;
std::vector<size_t> Destroy;

class Enemy
{
public:
	POINT pos, size;

	Enemy(const POINT& pos, const POINT& size) : pos(pos), size(size)
	{}

	virtual void Update(size_t index)
	{
		pos.x -= PlayerSpeed;

		if (pos.x < -size.x || pos.x > width + size.x || pos.y < -size.y || pos.y > height + size.y)
			Destroy.push_back(index);
	}
	
	virtual void Draw(HDC hdc)
	{
		DrawRect
		(
			pos.x - size.x / 2, pos.y - size.y / 2,
			pos.x + size.x / 2, pos.y + size.y / 2,
			0x0000FF
		);
	}

	virtual bool CheckCollision()
	{
		return
			(PlayerPos.x + PlayerSize.x / 2 > pos.x - size.x / 2 && PlayerPos.x - PlayerSize.x / 2 < pos.x + size.x / 2)
				&&
			(PlayerPos.y + PlayerSize.y / 2 > pos.y - size.y / 2 && PlayerPos.y - PlayerSize.y / 2 < pos.y + size.y / 2);
	}
};

class Astroid : public Enemy
{
public:
	POINT dir;

	Astroid(const POINT& pos, const POINT& size) : Enemy(pos, size)
	{
		double ang = ((rand() % 360) * 3.141592653596 / 180);
		dir = { (LONG) (sin(ang) * PlayerSpeed - 1), (LONG) (cos(ang) * PlayerSpeed - 1) };
	}

	void Update(size_t index) override
	{
		pos.x += dir.x;
		pos.y += dir.y;

		Enemy::Update(index);
	}
};

class Saucer : public Enemy
{
public:
	POINT playerDir;
	double playerAng;

	Saucer(const POINT& pos, const POINT& size) : Enemy(pos, size), playerDir({ 0, 0 }), playerAng(0)
	{}

	void Update(size_t index) override
	{
		playerAng = atan2l(PlayerPos.x - pos.x, PlayerPos.y - pos.y);
		playerDir = { (LONG) (sin(playerAng) * PlayerSpeed / 3), (LONG) (cos(playerAng) * PlayerSpeed / 3) };

		pos.x += playerDir.x;
		pos.y += playerDir.y;

		Enemy::Update(index);
	}
};

#pragma endregion
