#version 330 core

#define ATTR_POSITION 0
#define ATTR_TEXCOORD 1
#define ATTR_COLOR 2

#define pi 3.14159265

layout(location = ATTR_POSITION) in vec3 position;
layout(location = ATTR_TEXCOORD) in vec2 texCoord;

uniform mat4 zrv_UModel, zrv_UView, zrv_UProj;

out vec2 tex_coord;

void main()
{
    gl_Position = zrv_UProj * zrv_UView * zrv_UModel * vec4( position, 1.0);  
    tex_coord = texCoord;
}