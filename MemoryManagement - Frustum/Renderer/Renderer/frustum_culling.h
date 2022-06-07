#pragma once

#include <array>
#include <iostream>
#include <math.h>
#include"d3d11_renderer_impl.h"
#include"math_types.h"
#include"view.h"
// Note: You are free to make adjustments/additions to the declarations provided here.

namespace end
{
	struct sphere_t 
	{ 
		float3 center; float radius; 
	}; //Alterative: using sphere_t = float4;

	struct aabb_t 
	{ 
		float3 center; float3 extents; 
	}; //Alternative: aabb_t { float3 min; float3 max; };

	struct plane_t 
	{ 
		float3 normal; float offset; 
	};  //Alterative: using plane_t = float4;

	using frustum_t = std::array<plane_t, 6>;

	float FOV = 90.0f * 3.14/180;
	float VR = (4.0f / 3.0f);
	float nearDist = 1.0f;
	float farDist = 10.0f;

	float3 crossProduct(float3 a, float3 b)
	{
		float3 cross;

		cross.x = (a.y * b.z) - (a.z * b.y);
		cross.y = (a.z * b.x) - (a.x * b.z);
		cross.z = (a.x * b.y) - (a.y * b.x);

		return cross;
	}
	float dotProduct(float3 a, float3 b)
	{
		float result;
		return result = (a.x * b.x + a.y * b.y + a.z * b.z);
	}
	float3 normalize(float3 a)
	{
		float3 normal;
		float d = sqrtf(((a.x * a.x) + (a.y * a.y) + (a.z * a.z)));
		return normal = { (a.x / d), (a.y / d), (a.z / d) };
	}

	// Calculates the plane of a triangle from three points.
	plane_t calculate_plane(float3 a, float3 b, float3 c)
	{
		plane_t Tplane;
		Tplane.normal = crossProduct((b - a), (c - b));
		Tplane.normal = normalize(Tplane.normal);
		Tplane.offset = dotProduct(Tplane.normal, a);
		return Tplane;
	}

	// Calculates a frustum (6 planes) from the input view parameter.
	//
	// Calculate the eight corner points of the frustum. 
	// Use your debug renderer to draw the edges.
	// 
	// Calculate the frustum planes.
	// Use your debug renderer to draw the plane normals as line segments.
	void calculate_frustum(frustum_t& frustum, float4x4 Fmatrix)
	{
		float4 White =	{ 1,1,1,1 };
		float4 Black =	{ 0,0,0,1 };
		float4 Red =	{ 1,0,0,1 };
		float4 Green =	{ 0,1,0,1 };
		float4 Blue =	{ 0,0,1,1 };
		std::array<float3, 8> edges;

		float3 viewX =		{ Fmatrix[0][0], Fmatrix[0][1], Fmatrix[0][2] };
		float3 viewY =		{ Fmatrix[1][0], Fmatrix[1][1], Fmatrix[1][2] };
		float3 viewZ =		{ Fmatrix[2][0], Fmatrix[2][1], Fmatrix[2][2] };
		float3 viewPos =	{ Fmatrix[3][0], Fmatrix[3][1], Fmatrix[3][2] };
		//near center
		float3 nc = viewPos + viewZ * nearDist;
		//far center
		float3 fc = viewPos + viewZ * farDist;

		float Near = (2 * tanf((FOV / 2)) * nearDist);
		float Far = (2 * tanf((FOV / 2)) * farDist);
		float NearF = Near * VR;
		float FarF = Far * VR;


		edges[7] = (fc + viewY * (Far / 2) - viewX * (FarF / 2));
		float3 tempRight = viewX * FarF;
		edges[6] = edges[7] + tempRight;
		float3 tempDown = viewY * -Far;
		edges[5] = edges[6] + tempDown;
		edges[4] = edges[5] - tempRight;

		edges[3] = (nc + viewY * (Near / 2) - viewX * (NearF / 2));
		float3 nearRight = viewX * NearF;
		edges[2] = edges[3] + nearRight;
		float3 nearDown = viewY * -Near;
		edges[1] = edges[2] + nearDown;
		edges[0] = edges[1] - nearRight;

		//White to blue are the NORMAL DIRECTIONS
		//left plane
		frustum[0] = calculate_plane(edges[4], edges[0], edges[7]);
		float3 p1 = (edges[0] + edges[4] + edges[3] + edges[7]) / 4;
		debug_renderer::add_line(p1, (p1 + frustum[0].normal), Blue, White);

		//bottom plane
		frustum[1] = calculate_plane(edges[0], edges[4], edges[5]);
		float3 p2 = (edges[0] + edges[4] + edges[5] + edges[1]) / 4;
		debug_renderer::add_line(p2, (p2 + frustum[1].normal), Blue, White);

		//right plane
		frustum[2] = calculate_plane(edges[2], edges[1], edges[5]);
		float3 p3 = (edges[1] + edges[2] + edges[5] + edges[6]) / 4;
		debug_renderer::add_line(p3, (p3 + frustum[2].normal), Blue, White);

		//top plane
		frustum[3] = calculate_plane(edges[3], edges[2], edges[6]);
		float3 p4 = (edges[3] + edges[2] + edges[7] + edges[6]) / 4;
		debug_renderer::add_line(p4, (p4 + frustum[3].normal), Blue, White);

		//near plane
		frustum[4] = calculate_plane(edges[0], edges[1], edges[2]);
		float3 p5 = (edges[3] + edges[2] + edges[1] + edges[0]) / 4;
		debug_renderer::add_line(p5, (p5 + frustum[4].normal), Blue, White);

		//far plane
		frustum[5] = calculate_plane(edges[5], edges[4], edges[6]);
		float3 p6 = (edges[4] + edges[5] + edges[6] + edges[7]) / 4;
		debug_renderer::add_line(p6, (p6 + frustum[5].normal), Blue, White);

		//Create Frustum
		//near edges
		debug_renderer::add_line(edges[0], edges[1], Red, Red);
		debug_renderer::add_line(edges[1], edges[2], Red, Red);
		debug_renderer::add_line(edges[2], edges[3], Red, Red);
		debug_renderer::add_line(edges[3], edges[0], Red, Red);
		//side edges									 	  
		debug_renderer::add_line(edges[0], edges[4], Red, Red);
		debug_renderer::add_line(edges[1], edges[5], Red, Red);
		debug_renderer::add_line(edges[2], edges[6], Red, Red);
		debug_renderer::add_line(edges[3], edges[7], Red, Red);
		//far edges										 	  
		debug_renderer::add_line(edges[4], edges[5], Red, Red);
		debug_renderer::add_line(edges[5], edges[6], Red, Red);
		debug_renderer::add_line(edges[6], edges[7], Red, Red);
		debug_renderer::add_line(edges[7], edges[4], Red, Red);
	}

	// Calculates which side of a plane the sphere is on.
	//
	// Returns -1 if the sphere is completely behind the plane.
	// Returns 1 if the sphere is completely in front of the plane.
	// Otherwise returns 0 (Sphere overlaps the plane)
	int classify_sphere_to_plane(const sphere_t& sphere, const plane_t& plane)
	{
		float distance = dotProduct(sphere.center, plane.normal);

		distance -= plane.offset;
		if (distance > sphere.radius)
			return 1;
		if (distance < -sphere.radius)
			return -1;
		else
			return 0;
	}

	// Calculates which side of a plane the aabb is on.
	//
	// Returns -1 if the aabb is completely behind the plane.
	// Returns 1 if the aabb is completely in front of the plane.
	// Otherwise returns 0 (aabb overlaps the plane)
	// MUST BE IMPLEMENTED UsING THE PROJECTED RADIUS TEST
	int classify_aabb_to_plane(const aabb_t& aabb, const plane_t& plane)
	{
		sphere_t sphere;
		sphere.center = aabb.center;
		sphere.radius = ((aabb.extents.x * abs(plane.normal.x)) + (aabb.extents.y * abs(plane.normal.y)) + (aabb.extents.z * abs(plane.normal.z)));
		int test = classify_sphere_to_plane(sphere, plane);
		//if (test == 1)
		//	return 1;
		//if (test == -1)
		//	return -1;
		//else
		//	return 0;
		return test;
	}

	// Determines if the aabb is inside the frustum.
	//
	// Returns false if the aabb is completely behind any plane.
	// Otherwise returns true.
	bool aabb_to_frustum(const aabb_t& aabb, const frustum_t& frustum)
	{
		for (size_t i = 0; i < 6; i++)
		{
			int ret = classify_aabb_to_plane(aabb, frustum[i]);
			if (ret == -1)
				return false;
		}
		return true;
	}
}