$input v_color0, v_texcoord0, v_normal0

#include <bgfx_shader.sh>

SAMPLER2D(s_texColor,  0);
SAMPLER2D(s_texColor2,  1);

void main()
{
    vec3 light_position = vec3(10.0, 10.0, -15.0);
    vec3 v_position0 = vec3(0.0, 0.0, 0.0);
    vec3 light_direction = normalize(light_position - v_position0);
    vec3 light_color = vec3(0.8, 0.8, 0.8);

    vec3 ambient = 1.3 * light_color;

    float diff = max(dot(v_normal0, light_direction), 0.0);
    vec3 diffuse = diff * light_color;
    
	vec4 color = texture2D(s_texColor, v_texcoord0);
	vec4 color2 = texture2D(s_texColor2, v_texcoord0);
    vec4 tmp = color * color2;

    gl_FragColor = vec4(tmp.xyzw);// * (ambient + diffuse), 1.0);
}