#include "renderer.h"
#include <android/log.h>
#include "shaders/vertex.glsl.h"
#include "shaders/fragment.glsl.h"

#define LOG_TAG "Renderer"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

Renderer::Renderer() = default;
Renderer::~Renderer() = default;

void Renderer::init() {
    program_ = create_program(VERTEX_SHADER, FRAGMENT_SHADER);
    if (program_ == 0) {
        LOGE("Could not create program.");
        return;
    }

    position_loc_ = glGetAttribLocation(program_, "a_position");
    tex_coord_loc_ = glGetAttribLocation(program_, "a_texCoord");
    sampler_loc_ = glGetUniformLocation(program_, "s_texture");

    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Renderer::set_viewport(int width, int height) {
    viewport_width_ = width;
    viewport_height_ = height;
    glViewport(0, 0, width, height);
}

void Renderer::update_texture(const cv::Mat& frame) {
    if (frame.empty() || texture_id_ == 0) {
        return;
    }

    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Upload the processed frame data to the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame.cols, frame.rows, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, frame.data);
}

void Renderer::draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (program_ == 0 || texture_id_ == 0) {
        return;
    }

    glUseProgram(program_);

    // Simple quad to draw the texture on
    GLfloat vertices[] = {
        -1.0f,  1.0f, 0.0f, // Position 0
         0.0f,  0.0f,       // TexCoord 0
        -1.0f, -1.0f, 0.0f, // Position 1
         0.0f,  1.0f,       // TexCoord 1
         1.0f, -1.0f, 0.0f, // Position 2
         1.0f,  1.0f,       // TexCoord 2
         1.0f,  1.0f, 0.0f, // Position 3
         1.0f,  0.0f        // TexCoord 3
    };

    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

    // Load the vertex position
    glVertexAttribPointer(position_loc_, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vertices);
    // Load the texture coordinate
    glVertexAttribPointer(tex_coord_loc_, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), &vertices[3]);

    glEnableVertexAttribArray(position_loc_);
    glEnableVertexAttribArray(tex_coord_loc_);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glUniform1i(sampler_loc_, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

void Renderer::destroy() {
    if (program_) {
        glDeleteProgram(program_);
        program_ = 0;
    }
    if (texture_id_) {
        glDeleteTextures(1, &texture_id_);
        texture_id_ = 0;
    }
}

GLuint Renderer::load_shader(GLenum shader_type, const char* shader_source) {
    GLuint shader = glCreateShader(shader_type);
    if (shader) {
        glShaderSource(shader, 1, &shader_source, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n", shader_type, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint Renderer::create_program(const char* vertex_source, const char* fragment_source) {
    GLuint vertex_shader = load_shader(GL_VERTEX_SHADER, vertex_source);
    if (!vertex_shader) return 0;

    GLuint fragment_shader = load_shader(GL_FRAGMENT_SHADER, fragment_source);
    if (!fragment_shader) return 0;

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint infoLen = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetProgramInfoLog(program, infoLen, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}
