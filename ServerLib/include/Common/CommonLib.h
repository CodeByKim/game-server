#pragma once

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <chrono>
#include <thread>
#include <queue>
#include <stack>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include <string_view>
#include <exception>

#include "Util/Logger/LoggerLib.h"
#include "./Util/Time/Time.h"

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