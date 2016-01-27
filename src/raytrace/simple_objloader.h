#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "math/vec3.h"

namespace bacchus {

class ObjModel {
public:
    ObjModel() {}

    ObjModel(const std::string& filename) {
        load(filename);
    }

    ~ObjModel() {
        release();
    }

    void load(const std::string& filename) {
        std::string line;
        std::ifstream objFile(filename.c_str());
        if (objFile.is_open()) {
            objFile.seekg(0, std::ios::end);
            long fileSize = objFile.tellg();
            objFile.seekg(0, std::ios::beg);

            int vertAboutNum = fileSize / sizeof(float) / 3;
            mPoints.reserve(vertAboutNum);
            mVertexes.reserve(vertAboutNum);
            mNormals.reserve(vertAboutNum);

            while (!objFile.eof()) {
                std::getline(objFile, line);
                if (line.c_str()[0] == 'v') {
                    vec3f vert;
                    line[0] = ' ';
                    sscanf(line.c_str(), "%f %f %f ",
                           &vert.x, &vert.y, &vert.z);
                    mPoints.push_back(vert/100.0f);
                } else  if (line.c_str()[0] == 'f') {
                    line[0] = ' ';
                    vec3i vertNum;
                    sscanf(line.c_str(), "%i%i%i",
                           &vertNum[0], &vertNum[1], &vertNum[2]);

                    vec3f v0(mPoints[vertNum[0]-1]);
                    vec3f v1(mPoints[vertNum[1]-1]);
                    vec3f v2(mPoints[vertNum[2]-1]);

                    mVertexes.push_back(v0);
                    mVertexes.push_back(v1);
                    mVertexes.push_back(v2);

                    vec3f norm = calculateNormal(v0, v1, v2);

                    mNormals.push_back(norm);
                    mNormals.push_back(norm);
                    mNormals.push_back(norm);
                }
            }
            objFile.close();
        } else {
            ERROR_GL("could not load obj file: " + filename);
        }
    }

    void release() {
        mNormals.clear();
        mVertexes.clear();
        mPoints.clear();
    }

    vec3f calculateNormal(const vec3f& v0, const vec3f& v1, const vec3f& v2) {
        return normalize(cross(v0 - v1, v0 - v2));
    }

    std::vector<vec3f> mNormals;
    std::vector<vec3f> mVertexes;

private:
    std::vector<vec3f> mPoints;// temporary points
};

} // namespace bacchus
