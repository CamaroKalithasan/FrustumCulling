#include "debug_renderer.h"
#include"dev_app.h"
//#include "frustum_culling.h"
#include <array>
#include <math.h>

// Anonymous namespace
namespace
{
	// Declarations in an anonymous namespace are global BUT only have internal linkage.
	// In other words, these variables are global but are only visible in this source file.

	// Maximum number of debug lines at one time (i.e: Capacity)
	constexpr size_t MAX_LINE_VERTS = 10000; 
	//constexpr size_t MAX_LINES = 2000; 

	// CPU-side buffer of debug-line verts
	// Copied to the GPU and reset every frame.
	size_t line_vert_count = 0;
	std::array< end::colored_vertex, MAX_LINE_VERTS> line_verts;
	//std::array< end::MATRIX, MAX_LINES> vert_lines;
}

namespace end
{
	namespace debug_renderer
	{
		MATRIX mat;
		//TODO Once you finish this file correctly, you should see a green checkmark when you run the game.
		void add_line(float3 point_a, float3 point_b, float4 color_a, float4 color_b)
		{
			//TODO Add points to debug_verts, increments debug_vert_count
			colored_vertex colored_vertsA;
			colored_vertsA.pos = point_a;
			colored_vertsA.color = color_a;
			line_verts[line_vert_count] = colored_vertsA;
			line_vert_count++;
			colored_vertex colored_vertsB;
			colored_vertsB.pos = point_b;
			colored_vertsB.color = color_b;
			line_verts[line_vert_count] = colored_vertsB;
			line_vert_count++;
		}
		//float4x4 copyArr(float4x4 input, float4x4 output)
		//{
		//	float4x4 res;
		//	for (int x = 0; x < 4; x++)
		//	{
		//		for (int y = 0; y < 4; y++)
		//		{
		//			output[x][y] = input[x][y];
		//		}
		//	}
		//	return res = output = input;
		//}
		//void drawBox(aabb_t box)
		//{
		//	box.extents.x = 
		//}
		float4x4 fourbyfourmult(float4x4 input, float4x4 output)
		{
			float4x4 end;
			for (int x = 0; x < 4; x++)
			{
				for (int y = 0; y < 4; y++)
				{
					end[x][y] = 0;
					for (int z = 0; z < 4; z++) 
					{
						end[x][y] += input[x][z] * output[z][y];
					}
				}
			}
			return end;
		}
		float4x4 IdentityMatrix()
		{
			float4x4 id;
			id[0] = { 1,0,0,0 };
			id[1] = { 0,1,0,0 };
			id[2] = { 0,0,1,0 };
			id[3] = { 0,0,0,1 };
			return id;
		}
		float4x4 matrixRotX(float deg)
		{
			float rad = deg * (3.14 / 180);
			float4x4 val = IdentityMatrix();
			val[0][0] = 1;
			val[1][1] = cos(rad);
			val[1][2] = sin(rad);
			val[2][1] = -sin(rad);
			val[2][2] = cos(rad);
			return val;
		}
		float4x4 matrixRotY(float deg)
		{
			float rad = deg * (3.14 / 180);
			float4x4 val = IdentityMatrix();
			val[0][0] = cos(rad);
			val[0][2] = sin(rad);
			val[1][1] = 1;
			val[2][0] = -sin(rad);
			val[2][2] = cos(rad);
			return val;
		}
		float4x4 look_at_matrix(float3 viewerPos, float3 target, float3 localUp)
		{
			float3 zVec;
			float3 xVec;
			float3 yVec;
			zVec = target - viewerPos;
			zVec = zVec.normalize(zVec);
			xVec = xVec.cross(localUp, zVec);
			xVec = xVec.normalize(xVec);
			yVec = yVec.cross(zVec, xVec);
			yVec = yVec.normalize(yVec);
			return { float4(xVec.x, xVec.y, xVec.z, 0),float4(yVec.x, yVec.y, yVec.z, 0),float4(zVec.x, zVec.y, zVec.z, 0),float4(viewerPos.x, viewerPos.y, viewerPos.z, 1) };
		}
		float4x4 turn_to_matrix(float4x4 viewer, float3 target, float speed)
		{
			float3 vector = { viewer[3].xyz - target };
			vector = vector.normalize(vector);
			float x = vector.dot(viewer[0].xyz, vector);
			viewer = fourbyfourmult(matrixRotY(x * speed), viewer);
			float y = vector.dot(viewer[1].xyz, vector);
			viewer = fourbyfourmult(matrixRotX(y * speed), viewer);
			return viewer;
		}
		void clear_lines()
		{
			//TODO Resets debug_vert_count to 0
			line_vert_count = 0;
		}

		const colored_vertex* get_line_verts()
		{ 
			//TODO Returns the line vert array pointer
			return line_verts.data();
		}

		size_t get_line_vert_count() 
		{ 
			//TODO Returns how many vertices there are now
			return line_vert_count;
		}

		size_t get_line_vert_capacity()
		{
			//TODO returns the maximum size the line vert array
			return line_verts.max_size();
		}
	}
}