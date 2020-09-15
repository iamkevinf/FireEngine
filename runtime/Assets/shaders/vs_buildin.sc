$input a_position, a_texcoord0, a_color0, a_texcoord1, a_normal, a_tangent, a_weight, a_indices

$output v_color0, v_texcoord0, v_normal0

#include <bgfx_shader.sh>

uniform vec4 u_time;

void main()
{
	float sx = sin(u_time.x)*0.5+0.5;

	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
	v_color0 = a_color0 * sx;
	v_texcoord0 = a_texcoord0;
	v_normal0 = a_normal;
}
