#version 330 core

#define pi 3.14159265

out vec4 color;

in vec3 user_color;
in vec2 tex_coord;

uniform sampler2D tex_img;

void main()
{

    color = texture(tex_img, tex_coord);//0.5 * (1.1 *  result_tex   + 0.8 * vec4(user_color, 1.0)) ;
    
}