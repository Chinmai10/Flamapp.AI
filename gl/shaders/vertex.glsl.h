#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H

const char* VERTEX_SHADER = R"glsl(
attribute vec4 a_position;
attribute vec2 a_texCoord;
varying vec2 v_texCoord;
void main()
{
   gl_Position = a_position;
   v_texCoord = a_texCoord;
}
)glsl";

#endif // VERTEX_SHADER_H
