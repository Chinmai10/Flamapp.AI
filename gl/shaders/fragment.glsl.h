#ifndef FRAGMENT_SHADER_H
#define FRAGMENT_SHADER_H

const char* FRAGMENT_SHADER = R"glsl(
precision mediump float;
varying vec2 v_texCoord;
uniform sampler2D s_texture;
void main()
{
  gl_FragColor = texture2D(s_texture, v_texCoord);
}
)glsl";

#endif // FRAGMENT_SHADER_H
