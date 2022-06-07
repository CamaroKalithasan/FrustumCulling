#pragma once
//#define RandomColor D3DCOLOR_RGBA(rand() % 255, rand() % 255, rand() % 255, 1)
#include <cstdint>
#include <chrono>
#include<DirectXMath.h>
#include<math.h>
#include "math_types.h"
#include"../../pools.h"

namespace end
{

	// Simple app class for development and testing purposes
	struct MATRIX
	{
		float4x4 matrix1;
		float4x4 matrixLA;
		float4x4 matrixTT;
		float4x4 matrixYaw;
	};
	struct Particle
	{
		float3 pos;
		float3 prev_pos;
		float4 color;
		float3 velocity;
		float lifetime;
	};
	struct emitter
	{
		float3 spawn_pos;
		float4 spawn_color;
		sorted_pool_t<int16_t, 1024> emitPool;
	};			
	
	struct dev_app_t
	{	
		void update();
		//pool_t<Particle, 4096> unsortedPoolOverall;
		//sorted_pool_t<Particle, 1024> sortedpool;
		//std::array<emitter, 4> emit;
		float4x4_a UpdateCamera(float4x4_a view, float deltaX, float deltaY);
		dev_app_t();
		

		double get_delta_time()const;
	};
}