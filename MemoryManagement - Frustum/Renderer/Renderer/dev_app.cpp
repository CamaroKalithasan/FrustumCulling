#include "dev_app.h"
#include "math_types.h"
#include "debug_renderer.h"
//#include "../../pools.h"
#include"frustum_culling.h"
//#include<DirectXMath.h>
#include <Windows.h>
#include <WinUser.h>
#include <stdlib.h>
#include <iostream>
#include<math.h>

//TODO include debug_renderer.h and pools.h and anything else you might need here
using namespace DirectX;
namespace end
{
	double delta_time = 0.0;
	MATRIX mat;
	float4 Black = { 0,0,0,0 };
	float4 White = { 1,1,1,1 };
	float4 Grey = { 0.3, 0.3, 0.3 ,1 };
	float4 Red = { 1,0,0,0 };
	float4 Green = { 0,1,0,0 };
	float4 Blue = { 0,0,1,0 };
	double dev_app_t::get_delta_time()const
	{
		return delta_time;
	}

	dev_app_t::dev_app_t()
	{
 		//emit[0].spawn_color = float4(1, 0, 0, 1);
		//emit[1].spawn_color = float4(0, 1, 0, 1);
		//emit[2].spawn_color = float4(0, 0, 1, 1);
		//emit[3].spawn_color = float4(1, 0, 1, 1);
		//
		//emit[0].spawn_pos = float3(4, 0, -4);
		//emit[1].spawn_pos = float3(-4, 0, -4);
		//emit[2].spawn_pos = float3(4, 0, 4);
		//emit[3].spawn_pos = float3(-4, 0, 4);
		
		////RACECAR BOI
		mat.matrix1[0] = { 1,0,0,0 };
		mat.matrix1[1] = { 0,1,0,0 };
		mat.matrix1[2] = { 0,0,1,0 };
		mat.matrix1[3] = { 0,0,0,1 };

		////LOOK AT MATRIX
		//mat.matrixLA[0] = { 1,0,0,0 };
		//mat.matrixLA[1] = { 0,1,0,0 };
		//mat.matrixLA[2] = { 0,0,1,0 };
		//mat.matrixLA[3] = { 4,3,-4,1 };

		////TURN TO MATRIX
		//mat.matrixTT[0] = { 1,0,0,0 };
		//mat.matrixTT[1] = { 0,1,0,0 };
		//mat.matrixTT[2] = { 0,0,1,0 };
		//mat.matrixTT[3] = { -4,3,2,1 };

		std::cout << "Log whatever you need here.\n"; // Don’t forget to include <iostream>
	}
	
	double calc_delta_time()
	{
		static std::chrono::time_point<std::chrono::high_resolution_clock> last_time = std::chrono::high_resolution_clock::now();

		std::chrono::time_point<std::chrono::high_resolution_clock> new_time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed_seconds = new_time - last_time;
		last_time = new_time;

		return (((1.0 / 15.0) < (elapsed_seconds.count())) ? (1.0 / 15.0) : (elapsed_seconds.count()));
	}
	float4x4 yawMat(float yaw)
	{
		float4x4 matrix1;
		matrix1[0] = { cos(yaw),0,-sin(yaw),0 };
		matrix1[1] = { 0,1,0,0 };
		matrix1[2] = { sin(yaw),0,cos(yaw),0 };
		matrix1[3] = { 0,0,0,1 };
		return matrix1;
	}
	float4x4 orth(float3 z, float3 up)
	{
		float3 Newx;
		Newx = Newx.cross(up, z);
		Newx = Newx.normalize(Newx);
		float3 Newy;
		Newy = Newy.cross(z, Newx);
		Newy = Newy.normalize(Newy);
		float4x4 lookat;
		lookat[0] = { Newx.x,Newx.y,Newx.z,0 };
		lookat[1] = { Newy.x,Newy.y,Newy.z,0 };
		lookat[2] = { z.x,z.y,z.z,0 };
		lookat[3] = { mat.matrixTT[3].x,mat.matrixTT[3].y,mat.matrixTT[3].z,1 };
		return lookat;
	}
	float4x4_a IdentityMatrix_a()
	{
		float4x4_a identity;
		identity[0].xyz = { 1, 0, 0 };
		identity[0].w = 0;
		identity[1].xyz = { 0, 1, 0 };
		identity[1].w = 0;
		identity[2].xyz = { 0, 0, 1 };
		identity[2].w = 0;
		identity[3].xyz = { 0, 0, 0 };
		identity[3].w = 1;
		return identity;
	}
	float4x4 IdentityMatrix()
	{
		float4x4 identity;
		identity[0].xyz = { 1, 0, 0 };
		identity[0].w = 0;
		identity[1].xyz = { 0, 1, 0 };
		identity[1].w = 0;
		identity[2].xyz = { 0, 0, 1 };
		identity[2].w = 0;
		identity[3].xyz = { 0, 0, 0 };
		identity[3].w = 1;
		return identity;
	}
	float4x4 MatrixRotateX(float deg)
	{
		float rad = deg * (3.14 / 180);
		float4x4 value = IdentityMatrix();
		value[0][0] = 1;
		value[1][1] = cos(rad);
		value[1][2] = sin(rad);
		value[2][1] = -sin(rad);
		value[2][2] = cos(rad);
		return value;
	}
	float4x4 MatrixRotateY(float deg)
	{
		float rad = deg * (3.14 / 180);
		float4x4 value = IdentityMatrix();
		value[0][0] = cos(rad);
		value[0][2] = sin(rad);
		value[1][1] = 1;
		value[2][0] = -sin(rad);
		value[2][2] = cos(rad);
		return value;
	}
	float4x4_a MatrixRotateX_a(float deg)
	{
		float rad = deg * (3.14 / 180);
		float4x4_a value = IdentityMatrix_a();
		value[0][0] = 1;
		value[1][1] = cos(rad);
		value[1][2] = sin(rad);
		value[2][1] = -sin(rad);
		value[2][2] = cos(rad);
		return value;
	}
	float4x4_a MatrixRotateY_a(float deg)
	{
		float rad = deg * (3.14 / 180);
		float4x4_a value = IdentityMatrix_a();
		value[0][0] = cos(rad);
		value[0][2] = sin(rad);
		value[1][1] = 1;
		value[2][0] = -sin(rad);
		value[2][2] = cos(rad);
		return value;
	}
	float4x4 MatrixMultiply(float4x4 matrix1, float4x4 matrix2)
	{
		float4x4 returnMatrix;
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				returnMatrix[x][y] = 0;
				for (int z = 0; z < 4; z++)
				{
					returnMatrix[x][y] += matrix1[x][z] * matrix2[z][y];
				}
			}
		}
		return returnMatrix;
	}
	float4x4_a MatrixMultiply_a(float4x4_a matrix1, float4x4_a matrix2)
	{
		float4x4_a returnMatrix;
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				returnMatrix[x][y] = 0;
				for (int z = 0; z < 4; z++)
				{
					returnMatrix[x][y] += matrix1[x][z] * matrix2[z][y];
				}
			}
		}
		return returnMatrix;
	}
	float4x4 rotateFrustum(float4x4 view, float deltaX, float deltaY)
	{
		float4x4 RotationX;
		float4x4 RotationY;
		RotationX = MatrixRotateX(deltaY * 0.25);
		RotationY = MatrixRotateY(-deltaX * 0.25);
		float3 position = view[3].xyz;
		float4x4 move;
		move = MatrixMultiply(view, RotationY);
		move = MatrixMultiply(RotationX, move);
		move[3].xyz = position;
		return move;
	}
	XMMATRIX target = XMMatrixIdentity();
	int count = 0;
	bool check = false;
	std::array<aabb_t, 8> boxes;
	std::array<float3, 8> box1;
	std::array<float3, 8> box2;
	std::array<float3, 8> box3;
	std::array<float3, 8> box4;
	std::array<float3, 8> box5;
	std::array<float3, 8> box6;
	std::array<float3, 8> box7;
	std::array<float3, 8> box8;
	frustum_t frustumC;
	float4 outColor = { 1,0,0,1 }; //not touching
	float4 inColor = { 0,0,1,1 }; //inside frustum
	void drawBoxes(std::array<float3, 8> points, float4 color)
	{
		//represent bottom points
		float3 topLeft = points[0];
		float3 topRight = points[1];
		float3 botLeft = points[2];
		float3 botRight = points[3];

		//represent top points
		float3 tTopLeft = points[4];
		float3 tTopRight = points[5];
		float3 tBotLeft = points[6];
		float3 tBotRight = points[7];

		//bottom edges
		debug_renderer::add_line(topLeft, topRight, color, color);
		debug_renderer::add_line(topRight, botRight, color, color);
		debug_renderer::add_line(botRight, botLeft, color, color);
		debug_renderer::add_line(botLeft, topLeft, color, color);
		//side edges
		debug_renderer::add_line(topLeft, tTopLeft, color, color);
		debug_renderer::add_line(topRight, tTopRight, color, color);
		debug_renderer::add_line(botRight, tBotRight, color, color);
		debug_renderer::add_line(botLeft, tBotLeft, color, color);
		//top edges
		debug_renderer::add_line(tTopLeft, tTopRight, color, color);
		debug_renderer::add_line(tTopRight, tBotRight, color, color);
		debug_renderer::add_line(tBotRight, tBotLeft, color, color);
		debug_renderer::add_line(tBotLeft, tTopLeft, color, color);
	}
	std::array<float3, 8> initBoxes(std::array<aabb_t, 8>& boxes, float x, float y, float z, float decrease, float increase)
	{
		//represent bottom points
		float3 topLeft = { x, 0,z };
		float3 topRight = { topLeft.x + increase, 0,topLeft.z };
		float3 botLeft = { topLeft.x, 0, topLeft.z - decrease };
		float3 botRight = { topRight.x, 0, botLeft.z };

		//represent top points
		float3 tTopLeft = { topLeft.x, y, topLeft.z };
		float3 tTopRight = { topRight.x, tTopLeft.y, topRight.z };
		float3 tBotLeft = { botLeft.x, tTopLeft.y, botLeft.z };
		float3 tBotRight = { botRight.x, tTopLeft.y, botRight.z };

		boxes[count].center = (tTopRight + botLeft) / 2;
		boxes[count].extents = tTopRight - boxes[count].center;
		count++;

		std::array<float3, 8> points;

		points[0] = topLeft;
		points[1] = topRight;
		points[2] = botLeft;
		points[3] = botRight;
		points[4] = tTopLeft;
		points[5] = tTopRight;
		points[6] = tBotLeft;
		points[7] = tBotRight;

		return points;
	}
	colored_vertex start_vertex = { float3(3,0,0), {1,1,1,1} };
	colored_vertex end_vertex = { float3(3,3,0), {1,1,1,1} };
	float4x4_a local;
	
	void dev_app_t::update()
	{
		delta_time = calc_delta_time();
		
		//This drawn the green checkmark
		//end::debug_renderer::add_line(float3(-2, 0, 0), float3(0, -3, 0), float4(0.1f, 1, 0.1f, 1));
		//end::debug_renderer::add_line(float3(0, -3, 0), float3(3, 4, 0), float4(0.1f, 1, 0.1f, 1));

		//GRID
		static float ColorChange = 0;
		ColorChange += delta_time;
		if (ColorChange > 1.0f)
		{
			ColorChange = 0.0f;
		}

		for (int i = -20; i < 21; i++)
		{
			//end::debug_renderer::add_line(float3(-10, 0, i), float3(10, 0, i), float4(ColorChange, 0.5, 0.2, 1));
			//end::debug_renderer::add_line(float3(i, 0, -10), float3(i, 0, 10), float4(ColorChange, 0.5, 0.2, 1));
			end::debug_renderer::add_line(float3(-20, 0, i), float3(20, 0, i), Grey);
			end::debug_renderer::add_line(float3(i, 0, -20), float3(i, 0, 20), Grey);
		}
		box1 = initBoxes(boxes, -5, 4, 9, 2, 4);
		box2 = initBoxes(boxes, 5, 2, -3, 4, 1);
		box3 = initBoxes(boxes, 8, 5, 2, 1, 5);
		box4 = initBoxes(boxes, -3, 1, -5, 6, 3);
		box5 = initBoxes(boxes, -10, 5, 10, 6, 3);
		box6 = initBoxes(boxes, 8, 6, -9, 4, 3);
		box7 = initBoxes(boxes, -9, 3, -14, 9, 3);
		box8 = initBoxes(boxes, -6, 5, 1, 7, 3);
		count = 0;
		//calculate_frustum(frustumC, (float4x4&)target);
		calculate_frustum(frustumC, mat.matrix1);
		for (int i = 0; i < boxes.size(); i++)
		{
			check = aabb_to_frustum(boxes[i], frustumC);
			if (check)
			{
				if (i == 0)
				{
					drawBoxes(box1, inColor);
					check = false;
				}
				if (i == 1)
				{
					drawBoxes(box2, inColor);
					check = false;
				}
				if (i == 2)
				{
					drawBoxes(box3, inColor);
					check = false;
				}
				if (i == 3)
				{
					drawBoxes(box4, inColor);
					check = false;
				}
				if (i == 4)
				{
					drawBoxes(box5, inColor);
					check = false;
				}
				if (i == 5)
				{
					drawBoxes(box6, inColor);
					check = false;
				}
				if (i == 6)
				{
					drawBoxes(box7, inColor);
					check = false;
				}
				if (i == 7)
				{
					drawBoxes(box8, inColor);
					check = false;
				}
			}
			else
			{
				if (i == 0)
				{
					drawBoxes(box1, outColor);
				}
				if (i == 1)
				{
					drawBoxes(box2, outColor);
				}
				if (i == 2)
				{
					drawBoxes(box3, outColor);
				}
				if (i == 3)
				{
					drawBoxes(box4, outColor);
				}
				if (i == 4)
				{
					drawBoxes(box5, outColor);
				}
				if (i == 5)
				{
					drawBoxes(box6, outColor);
				}
				if (i == 6)
				{
					drawBoxes(box7, outColor);
				}
				if (i == 7)
				{
					drawBoxes(box8, outColor);
				}
			}
		}
		//MATRIX BEHAVIORS
		
		//RACECAR
		float3 positionMat = mat.matrix1[3].xyz;
		float3 positionMatX = mat.matrix1[0].xyz + positionMat;
		float3 positionMatY = mat.matrix1[1].xyz + positionMat;
		float3 positionMatZ = mat.matrix1[2].xyz + positionMat;
		//mat.matrix1 = MatrixRotateX(delta_time * 0.25);
		//mat.matrix1 = MatrixRotateY(-delta_time * 0.25);
		//positionMat = mat.matrix1[3].xyz;
		end::debug_renderer::add_line(positionMat, positionMatX, Red);
		end::debug_renderer::add_line(positionMat, positionMatY, Green);
		end::debug_renderer::add_line(positionMat, positionMatZ, Blue);
		//
		////LOOK AT
		//float3 positionMatLA = mat.matrixLA[3].xyz;
		//float4x4 overPosLA = end::debug_renderer::look_at_matrix(positionMatLA, positionMat, float3{ 0,1,0 });
		//float3 positionMatoverXLA = overPosLA[0].xyz + positionMatLA;
		//float3 positionMatoverYLA = overPosLA[1].xyz + positionMatLA;
		//float3 positionMatoverZLA = overPosLA[2].xyz + positionMatLA;
		//end::debug_renderer::add_line(positionMatLA, positionMatoverXLA, Red);
		//end::debug_renderer::add_line(positionMatLA, positionMatoverYLA, Green);
		//end::debug_renderer::add_line(positionMatLA, positionMatoverZLA, Blue);
		//
		////TURN TO
		//mat.matrixTT = end::debug_renderer::turn_to_matrix(mat.matrixTT, float3(mat.matrix[3].xyz), 0.5f);
		//mat.matrixTT = orth(mat.matrixTT[2].xyz, float3(0.0, 1.0, 0.0));
		//float3 positionMatTT = mat.matrixTT[3].xyz;
		//float3 xLine = mat.matrixTT[0].xyz + positionMatTT;
		//float3 yLine = mat.matrixTT[1].xyz + positionMatTT;
		//float3 zLine = mat.matrixTT[2].xyz + positionMatTT;
		//end::debug_renderer::add_line(positionMatTT, xLine, Red);
		//end::debug_renderer::add_line(positionMatTT, yLine, Green);
		//end::debug_renderer::add_line(positionMatTT, zLine, Blue);

		if (GetAsyncKeyState(VK_UP))
		{
			mat.matrix1[3].z += 0.01;
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			mat.matrix1[3].z -= 0.01;
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			mat.matrix1[3].x += 0.01;
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			mat.matrix1[3].x -= 0.01;
		}
		if (GetAsyncKeyState('Q'))
		{
			float4x4 n = MatrixRotateY(delta_time * 100);
			n = MatrixMultiply(n, mat.matrix1);
			mat.matrix1 = n;
		}
		if (GetAsyncKeyState('E'))
		{
			float4x4 n = MatrixRotateY(delta_time * -100);
			n = MatrixMultiply(n, mat.matrix1);
			mat.matrix1 = n;
		}

		//PARTICLE SYSTEM
		//while (par > 0.0055f)
		//{
		//	for (int i = 0; i < 1; i++)
		//	{
		//		Particle particles;
		//		float x = (rand() % 100) * .02;
		//		float z = (rand() % 100) * .02;
		//		int swapx = rand() % 2;
		//		int swapz = rand() % 2;
		//		if (swapx == 1)
		//			x = -x;
		//		if (swapz == 1)
		//			z = -z;
		//		particles.color = float4(1, 0, 0, 1);
		//		particles.pos = float3(0, 0, 0);
		//		particles.prev_pos = float3(0, 0, 0);
		//		particles.lifetime = 0.0f;
		//		particles.velocity = float3(x, 12, z);
		//		sortedpool[sortedpool.alloc()] = particles;
		//	}
		//
		//	for (int i = 0; i < 4; i++)
		//	{
		//		Particle unsortedParticles;
		//		float x = (rand() % 100) * .02;
		//		float z = (rand() % 100) * .02;
		//		int swapx = rand() % 2;
		//		int swapz = rand() % 2;
		//		if (swapx == 1)
		//			x = -x;
		//		if (swapz == 1)
		//			z = -z;
		//		unsortedParticles.color = emit[i].spawn_color;
		//		unsortedParticles.pos = emit[i].spawn_pos;
		//		//unsortedParticles.prev_pos = emit[i].spawn_pos;
		//		unsortedParticles.lifetime = 1.0f;
		//		unsortedParticles.velocity = float3(x, 12, z);
		//		int indices = unsortedPoolOverall.alloc();
		//		int emitterindices = emit[i].emitPool.alloc();
		//		if (indices != -1 || emitterindices != -1)
		//		{
		//			unsortedPoolOverall[indices] = unsortedParticles;
		//			emit[i].emitPool[emitterindices] = indices;
		//		}
		//	}
		//
		//	par -= 0.0055;
		//}
		//for (int i = 0; i < sortedpool.size(); i++)
		//{
		//	sortedpool[i].lifetime += delta_time;
		//	if (sortedpool[i].lifetime >= 1.0f)
		//	{
		//		sortedpool.free(i);
		//	}
		//	float3 gravity = { 0,-9.8f,0 };
		//	sortedpool[i].prev_pos = sortedpool[i].pos;
		//	sortedpool[i].velocity += gravity * delta_time;
		//	sortedpool[i].pos += sortedpool[i].velocity * delta_time;
		//	end::debug_renderer::add_line(sortedpool[i].pos, sortedpool[i].prev_pos, float4(0.61, 0.83, 1, 1)); //water blue
		//}
		//
		////UNSORTED
		//for (int i = 0; i < emit.size(); i++)
		//{
		//	for (int j = 0; j < emit[i].emitPool.size(); j++)
		//	{
		//		unsortedPoolOverall[emit[i].emitPool[j]].lifetime += delta_time;
		//		if (unsortedPoolOverall[emit[i].emitPool[j]].lifetime >= 2.0f)
		//		{
		//			unsortedPoolOverall.free(emit[i].emitPool[j]);
		//			emit[i].emitPool.free(j);
		//		}
		//		float3 gravity = { 0,-9.8f,0 };
		//		unsortedPoolOverall[emit[i].emitPool[j]].prev_pos = unsortedPoolOverall[emit[i].emitPool[j]].pos;
		//		unsortedPoolOverall[emit[i].emitPool[j]].velocity += gravity * delta_time;
		//		unsortedPoolOverall[emit[i].emitPool[j]].pos += unsortedPoolOverall[emit[i].emitPool[j]].velocity * delta_time;
		//		end::debug_renderer::add_line(unsortedPoolOverall[emit[i].emitPool[j]].pos, unsortedPoolOverall[emit[i].emitPool[j]].prev_pos, float4(1, 0, 0, 1)); //water blue
		//
		//	}
		//	
		//}
	}
	float4x4_a dev_app_t::UpdateCamera(float4x4_a view, float deltaX, float deltaY)
	{
		
		if (GetAsyncKeyState('W'))
		{
			view[3].z += 0.02;
		}
		if (GetAsyncKeyState('A'))
		{
			view[3].x -= 0.02;
		}
		if (GetAsyncKeyState('S'))
		{
			view[3].z -= 0.02;
		}
		if (GetAsyncKeyState('D'))
		{
			view[3].x += 0.02;
		}
		if (GetAsyncKeyState(VK_SPACE))
		{
			view[3].y += 0.02;
		}
		if (GetAsyncKeyState(VK_LCONTROL))
		{
			view[3].y -= 0.02;
		}
		//float windowWidth = 1280;
		//float windowHeight = 720;
		//float aspectRatio = windowHeight / windowWidth;
		//float totalPitch = (aspectRatio * deltaY / windowHeight) * 200;
		//float totalYaw = (aspectRatio * deltaX / windowWidth) * 200;
		float4x4_a RotationX;
		float4x4_a RotationY;
		RotationX = MatrixRotateX_a(deltaY * 0.25);
		RotationY = MatrixRotateY_a(-deltaX * 0.25);
		float3 position = view[3].xyz;
		float4x4_a move;
		move = MatrixMultiply_a(view, RotationY);
		move = MatrixMultiply_a(RotationX, move);
		move[3].xyz = position;
		return move;
	}
}
