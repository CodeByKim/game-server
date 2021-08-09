#pragma once

#define MOVE_DIR_LEFT 0
#define MOVE_DIR_UP 1
#define MOVE_DIR_RIGHT 2
#define MOVE_DIR_DOWN 3

#define MOVE_SPEED 20

struct GridLocation
{
	short x;
	short y;

	GridLocation operator+(GridLocation& other)
	{
		GridLocation pos;
		pos.x = x + other.x;
		pos.y = y + other.y;

		return pos;
	}

	GridLocation operator-(GridLocation& other)
	{
		GridLocation pos;
		pos.x = x - other.x;
		pos.y = y - other.y;

		return pos;
	}

	GridLocation& operator+=(GridLocation other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	GridLocation& operator-=(GridLocation other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}

	bool operator==(GridLocation& other)
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(GridLocation& other)
	{
		return !(*this == other);
	}
};

struct Position
{
	float x;
	float y;

	void Multiply(float offset)
	{
		x *= offset;
		y *= offset;
	}

	Position operator+(Position& other)
	{
		Position pos;
		pos.x = x + other.x;
		pos.y = y + other.y;

		return pos;
	}

	Position operator-(Position& other)
	{
		Position pos;
		pos.x = x - other.x;
		pos.y = y - other.y;

		return pos;
	}

	Position& operator+=(Position other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	Position& operator-=(Position other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}

	bool operator==(Position& other)
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(Position& other)
	{
		return !(*this == other);
	}
};

