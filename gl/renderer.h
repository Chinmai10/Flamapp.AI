#ifndef FLAMAPP_RENDERER_H
#define FLAMAPP_RENDERER_H

#include <GLES2/gl2.h>
#include <opencv2/core/mat.hpp>

class Renderer {
public:
    Renderer();
    ~Renderer();

    void init();
    void set_viewport(int width, int height);
    void draw();
    void update_texture(const cv::Mat& frame);
    void destroy();

private:
    GLuint load_shader(GLenum shader_type, const char* shader_source);
    GLuint create_program(const char* vertex_source, const char* fragment_source);

    GLuint program_ = 0;
    GLuint texture_id_ = 0;
    GLint position_loc_ = -1;
    GLint tex_coord_loc_ = -1;
    GLint sampler_loc_ = -1;

    int viewport_width_ = 0;
    int viewport_height_ = 0;
};

#endif //FLAMAPP_RENDERER_H
