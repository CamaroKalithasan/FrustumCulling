#pragma once

#include "math_types.h"

// Interface to the debug renderer
namespace end
{
	namespace debug_renderer
	{
		void add_line(float3 point_a, float3 point_b, float4 color_a, float4 color_b);

		inline void add_line(float3 p, float3 q, float4 color) { add_line(p, q, color, color); }
		float4x4 fourbyfourmult(float4x4 input, float4x4 output);
		float4x4 look_at_matrix(float3 viewerPos, float3 target, float3 localUp);
		float4x4 turn_to_matrix(float4x4 viewer, float3 target, float speed);
		float4x4 IdentityMatrix();
		float4x4 matrixRotX(float deg);
		float4x4 matrixRotY(float deg);
		//float4x4 orth(float3 z, float3 up);
		//float4x4 copyArr(float4x4 input, float4x4 output);



		void clear_lines();

		const colored_vertex* get_line_verts();

		size_t get_line_vert_count();

		size_t get_line_vert_capacity();
	}
}