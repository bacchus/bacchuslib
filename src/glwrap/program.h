#pragma once

#include <map>
#include <vector>

#include "glinclude.h"

#include "shader.h"
#include "attribute.h"
#include "uniform.h"
#include "values.h"
#include "vertex.h"
#include "texture.h"
#include "frame_buffer.h"

namespace bacchus {

class Program {
public:
    Program(const std::string& vert, const std::string& frag): id(0) {
        m_vert = new Shader(Shader::VERTEX, vert);
        m_frag = new Shader(Shader::FRAGMENT, frag);
    }

    ~Program() {}

    void attach(Shader* shader) {
        shader->attachto(id);
        //glAttachShader(id, shader->id());
    }

    void createGl() {
        m_vert->createGl();
        m_frag->createGl();
        CHECK_GL_ERROR(id = glCreateProgram());
        m_vert->attachto(id);
        m_frag->attachto(id);
        CHECK_GL_ERROR(glLinkProgram(id));
        GLint linked;
        CHECK_GL_ERROR(glGetProgramiv(id, GL_LINK_STATUS, &linked));
        if (!linked) {
            GLint info_len;
            CHECK_GL_ERROR(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_len));
            if (info_len > 1) {
                char* info_log = new char[info_len];
                CHECK_GL_ERROR(glGetProgramInfoLog(id, info_len, nullptr, info_log));
                LOGE("Program error:%s\n", info_log);
                delete[] info_log;
            }
            CHECK_GL_ERROR(glDeleteProgram(id));
            throw ShaderError("Shader link error");
        }
    }

    Attribute* attribute(const std::string& name) {
        if (m_attributes.find(name) == m_attributes.end()) {
            int location;
            CHECK_GL_ERROR(location = glGetAttribLocation(id, name.c_str()));
            m_attributes[name] = new Attribute(location);
        }
        return m_attributes[name];
    }

    Uniform* uniform(const std::string& name) {
        // glGetUniformLocation can be done right after link
        // and cashed to m_uniforms
        // but I do it only first time so its ok
        if (m_uniforms.find(name) == m_uniforms.end()) {
            int location;
            CHECK_GL_ERROR(location = glGetUniformLocation(id, name.c_str()));
            m_uniforms[name] = new Uniform(location);
        }
        return m_uniforms[name];
    }

    //    Attribute* attribute(const std::string& name) {
    //        return new Attribute(glGetAttribLocation(id, name.c_str()));
    //    }

    //    Uniform* uniform(const std::string& name) {
    //        return new Uniform(glGetUniformLocation(id, name.c_str()));
    //    }

    void bind() {
        CHECK_GL_ERROR(glUseProgram(id));
    }

    void destroyGl() {
        CHECK_GL_ERROR(glDeleteProgram(id));
    }

    Value*& parameter(const std::string& name) {
        return m_parameters[name];
    }

    void setupParameters() {
        //for (ParamIt it = m_parameters.begin(); it != m_parameters.end(); ++it) {
        for (auto it : m_parameters) {
            std::string name = it.first;
            Value* val = it.second;
            if (val) {
                val->set_uniform(uniform(name));
            }
        }

        for (uint i = 0; i < m_textures.size(); ++i) {
            Texture* tex = m_textures[i];
            tex->activate(i);
            tex->bind();
        }
    }

    void enableVertexes() {
        //for (VertIt it = m_vertexes.begin(); it != m_vertexes.end(); ++it) {
        for (auto it : m_vertexes) {
            std::string name = it.first;
            Vertex* vert = it.second;
            if (vert) {
                attribute(name)->vertexPointer(vert->size(), vert->stride(), vert->ptr());
                attribute(name)->enable();
            }
        }
    }

    void disableVertexes() {
        //for (VertIt it = m_vertexes.begin(); it != m_vertexes.end(); ++it) {
        for (auto it : m_vertexes) {
            std::string name = it.first;
            Vertex* vert = it.second;
            if (vert) {
                attribute(name)->disable();
            }
        }
    }

    Vertex*& vertex(const std::string& name) {
        return m_vertexes[name];
    }

    void add_texture(const std::string& name, Texture* tex) {
        parameter(name) = new uvec1i(m_textures.size());
        m_textures.push_back(tex);
    }

    void render() {
        if (m_out) {
            m_width = m_out->width();
            m_height = m_out->height();
        }

        CHECK_GL_ERROR(glViewport(0, 0, m_width, m_height));
        CHECK_GL_ERROR(glClear(GL_COLOR_BUFFER_BIT));

        if (m_out) m_out->bind();

        bind();
        setupParameters();
        enableVertexes();
        CHECK_GL_ERROR(glDrawArrays(m_mode, 0, m_vertex_count));
        disableVertexes();

        if (m_out) m_out->unbind();
    }

    void set_out(FrameBufferTexture* fbt) {
        m_out = fbt;
    }

    void set_draw_mode(GLenum mode, int count) {
        m_mode = mode;
        m_vertex_count = count;
    }

    void set_out_size(int w, int h) {
        m_width = w;
        m_height = h;
    }

private:
    int id;
    Shader* m_vert;
    Shader* m_frag;
    FrameBufferTexture* m_out;
    GLenum m_mode;
    int m_vertex_count;
    int m_width, m_height;

    std::vector<Texture*> m_textures;
    std::map<std::string, Vertex*> m_vertexes;
    std::map<std::string, Value*> m_parameters;
    std::map<std::string, Attribute*> m_attributes;
    std::map<std::string, Uniform*> m_uniforms;

    // no need since c++11
    //    typedef std::map<std::string, Vertex*>::iterator VertIt;
    //    typedef std::map<std::string, Value*>::iterator ParamIt;
    //    typedef std::map<std::string, Attribute*>::iterator AttrIt;
    //    typedef std::map<std::string, Uniform*>::iterator UnifIt;
};

} // namespace bacchus
