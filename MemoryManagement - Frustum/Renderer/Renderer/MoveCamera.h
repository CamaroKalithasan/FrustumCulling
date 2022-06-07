#pragma once
#include"dev_app.h"
#include "math_types.h"
#include "debug_renderer.h"
#include "../../pools.h"
#include"XTime.h"
#include<iostream>
#include<WinUser.h>
void MoveCamera()
{
	XTime timer;
	timer.Signal();
	float delta_time = timer.Delta();
	end::VERTEX v;
	if (GetAsyncKeyState('W'))
	{
		v.z = delta_time * 2;
	}
	if (GetAsyncKeyState('S'))
	{
		v.z = -delta_time * 2;
	}
	if (GetAsyncKeyState('A'))
	{
		v.x = delta_time * 2;
	}
	if (GetAsyncKeyState('D'))
	{
		v.x = -delta_time * 2;
	}
	if (GetAsyncKeyState('Q'))
	{
		v.y = -delta_time * 2;
	}
	if (GetAsyncKeyState('E'))
	{
		v.y = delta_time * 2;
	}
}