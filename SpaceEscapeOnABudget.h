#pragma once

#include <vector>

#include "resource.h"

#define lerp(s, e, l) s + (e - s) * l;

#define RePaint() InvalidateRect(hWnd, NULL, FALSE);

#define DrawRect(l, t, r, b, c) { RECT rt = { l, t, r, b }; HBRUSH br = CreateSolidBrush(c); FrameRect(hdc, &rt, br); DeleteObject(br); }
#define FillRect(l, t, r, b, c) { RECT rt = { l, t, r, b }; HBRUSH br = CreateSolidBrush(c); FillRect(hdc, &rt, br); DeleteObject(br); }

HWND hWnd;

BOOL MouseDown;

INT
width  = 1280,
height = 720;

POINT Mouse;

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

class Enemy
{
public:
	POINT pos, size;

	Enemy(const POINT& pos, const POINT& size) : pos(pos), size(size)
	{}

	virtual void Update(size_t index)
	{
		pos.x -= PlayerSpeed;

		if (pos.x < -size.x)
			Enemies.erase(Enemies.begin() + index);
	}
	virtual void Draw(HDC hdc)
	{}
};

class Astroid : public Enemy
{
public:
	Astroid(const POINT& pos, const POINT& size) : Enemy(pos, size) {}

	void Draw(HDC hdc) override
	{
		DrawRect
		(
			pos.x - size.x / 2, pos.y - size.y / 2,
			pos.x + size.x / 2, pos.y + size.y / 2,
			0x0000FF
		);
	}
};

#pragma endregion
