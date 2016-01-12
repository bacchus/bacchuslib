#pragma once

#include <GLES3/gl3.h>

#include <string>
#include <sstream>
#include <map>
#include <map>
#include <vector>

#include "utils/logger.h"

namespace bacchus {

class Shader {
public:
    enum ShaderType {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
    };

    Shader(ShaderType type, const std::string& src): id(0), m_type(type), m_source(src) {}
    ~Shader() {}

    void attachto(int pid) {
        CHECK_GL_ERROR(glAttachShader(pid, id));
    }

    void createGl() {
        id = glCreateShader(m_type);
        CHECK_GL_ERROR();
        const char* c_str = m_source.c_str();
        CHECK_GL_ERROR(glShaderSource(id, 1, &c_str, NULL));
        CHECK_GL_ERROR(glCompileShader(id));
        GLint compiled;
        glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint info_len = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_len);
            if (info_len > 1) {
                char* info_log = new char[info_len];
                glGetShaderInfoLog(id, info_len, NULL, info_log);
                LOG_ERROR("Shader error:\n%s\n", info_log);
                //LOG_ERROR("Shader:\n%s\n", m_source.c_str());
                std::map<int,int> err_map;
                parseErrLog(info_log, err_map);
                printShader(err_map);
                delete[] info_log;
            }
            CHECK_GL_ERROR(glDeleteShader(id));
        }
    }

    void destroyGl() {
        CHECK_GL_ERROR(glDeleteShader(id));
    }

    void parseErrLog(const char* info_log, std::map<int,int>& err_map) {
        //0:6(21): error: cannot initialize uniforms...
        std::istringstream iss(info_log);
        std::string line;
        std::size_t found = 0;
        while (std::getline(iss, line)) {
            found = line.find("0:");
            if (found!=std::string::npos) {
                line = line.substr(found+2);
                std::istringstream line_iss(line);
                char trash;
                int ln, lp;
                line_iss >> ln >> trash >> lp;
                //TODO: now store only first error on line, show multiply errors on line
                if (err_map.find(ln) == err_map.end()) {
                    err_map[ln] = lp;
                }
            }
        }
    }

    void printShader(const std::map<int,int>& err_map) {
        std::istringstream iss(m_source);
        std::ostringstream oss;
        std::string line;
        int i = 0;
        while (std::getline(iss, line)) {
            ++i;
            oss << (i<10?" ":"") << i << "|" << line.c_str() << "\n";
            int preceding_spaces = line.find_first_not_of(' ');
            auto it = err_map.find(i);
            if (it != err_map.end()) {
                if (preceding_spaces==0) preceding_spaces=1;
                std::vector<char> spacer(it->second+preceding_spaces+1, ' ');
                spacer.push_back('\0');
                oss << &spacer[0] << "^\n";
            }
        }
        LOG_INFO("\n%s\n", oss.str().c_str());
    }

private:
    int id;
    ShaderType m_type;
    std::string m_source;
};

} // namespace bacchus
