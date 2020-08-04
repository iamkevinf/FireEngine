$input v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(display_texColor, 0);

void main()
{
    gl_FragColor = texture2D(display_texColor, v_texcoord0);
    //gl_FragColor = vec4(vec3(1 - texture2D(display_texColor, v_texcoord0).rgb), 1.0);
    //float average = 0.2126 * gl_FragColor.r + 0.7152 * gl_FragColor.g + 0.0722 * gl_FragColor.b;
    //gl_FragColor = vec4(average, average, average, 1.0);
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}