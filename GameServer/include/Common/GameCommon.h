#pragma once

#define MOVE_DIR_LEFT 0
#define MOVE_DIR_UP 1
#define MOVE_DIR_RIGHT 2
#define MOVE_DIR_DOWN 3

#define MOVE_SPEED 15
#define DAMAGE 10

struct Position2DInt
{
	int x;
	int y;

	Position2DInt operator+(Position2DInt& other)
	{
		Position2DInt pos;
		pos.x = x + other.x;
		pos.y = y + other.y;

		return pos;
	}

	Position2DInt operator-(Position2DInt& other)
	{
		Position2DInt pos;
		pos.x = x - other.x;
		pos.y = y - other.y;

		return pos;
	}

	Position2DInt& operator+=(Position2DInt other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	Position2DInt& operator-=(Position2DInt other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}

	bool operator==(Position2DInt& other)
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(Position2DInt& other)
	{
		return !(*this == other);
	}
};

struct Position2D
{
	float x;
	float y;

	void Multiply(float offset)
	{
		x *= offset;
		y *= offset;
	}

	Position2D operator+(Position2D& other)
	{
		Position2D pos;
		pos.x = x + other.x;
		pos.y = y + other.y;

		return pos;
	}

	Position2D operator-(Position2D& other)
	{
		Position2D pos;
		pos.x = x - other.x;
		pos.y = y - other.y;

		return pos;
	}

	Position2D& operator+=(Position2D other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	Position2D& operator-=(Position2D other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}

	bool operator==(Position2D& other)
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(Position2D& other)
	{
		return !(*this == other);
	}
};

