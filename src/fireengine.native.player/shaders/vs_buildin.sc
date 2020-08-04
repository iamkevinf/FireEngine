$input a_position, a_color0, a_normal
$output v_color0, v_normal0

#include <bgfx_shader.sh>

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
	v_color0 = a_color0;
	v_normal0 = a_normal;
}