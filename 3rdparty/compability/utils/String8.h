#pragma once

#include <string>

class String8 {
public:
    String8() {

    }

    String8(const char* str)
        : mStr(str)
    {

    }

    const char* string() const {
        return mStr.c_str();
    }

    String8& operator=(const String8& str) {
        mStr = str.mStr;
        return *this;
    }

private:
    std::string mStr;
};
