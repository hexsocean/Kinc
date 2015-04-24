#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct uniforms_vertex {
	matrix_float4x4 projectionMatrix;
};

struct uniforms_fragment {
	//sampler2D tex;
};

struct vertex_in {
	packed_float3 vertexPosition;
	packed_float2 texPosition;
	packed_float4 vertexColor;
};

struct fragment_in {
	float4 position [[position]];
	float2 texCoord;
	float4 color;
};

vertex fragment_in kore_vertex(device vertex_in* vertices [[buffer(0)]],
							   constant uniforms_vertex& uniforms [[buffer(1)]],
							   unsigned int vid [[vertex_id]]) {
	fragment_in out;
	out.position = uniforms.projectionMatrix * float4(float3(vertices[vid].vertexPosition), 1.0);
	out.texCoord = vertices[vid].texPosition;
	out.color = vertices[vid].vertexColor;
	return out;
}

fragment half4 kore_fragment(fragment_in in [[stage_in]]) {
	half4 texcolor = half4(1, 0, 0, 1); //texture2D(tex, texCoord) * color;
	texcolor.rgb *= in.color.a;
	return texcolor;
}
