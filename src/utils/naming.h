const int reserved_word_count = 88;
const char* reserved_word_list[reserved_word_count] = {
    "and", "break", "do", "else", "elseif", "end", "false", "for", "case", "sizeof",
    "function", "if", "in", "local", "nil", "not", "or", "repeat", "switch", "return",
    "then", "true", "until", "while", "default", "goto", "catch", "class", "const_cast", "delete",
    "dynamic_cast", "explicit", "export", "friend", "mutable", "namespace", "new", "operator", "private", "protected",
    "public", "signals", "slots", "reinterpret_cast", "static_assert", "static_cast", "template", "this", "throw", "try",
    "typeid", "typename", "using", "virtual", "assert", "collectgarbage", "dofile", "error", "getfenv", "getmetatable",
    "gcinfo", "ipairs", "loadfile", "loadlib", "loadstring", "next", "pairs", "pcall", "print", "rawequal",
    "rawget", "rawset", "require", "setfenv", "setmetatable", "tonumber", "tostring", "type", "unpack", "xpcall",
    "_G", "_VERSION", "LUA_PATH", "_LOADED", "_REQUIREDNAME", "_ALERT", "_ERRORMESSAGE", "_PROMPT"
};

bool check_name_common(const char* str) {
    const char* pch = str;
    char ch = *pch++;
    if (!(ch >= 'a' && ch <= 'z' ||
          ch >= 'A' && ch <= 'Z'))
        return false;

    while(ch = *pch++) {
        if (!(ch >= '0' && ch <= '9' ||
              ch >= 'a' && ch <= 'z' ||
              ch >= 'A' && ch <= 'Z' ||
              ch == '_'))
            return false;
    }
    return true;
}

bool check_reserved_names(const char* str) {
    for (int i=0; i<reserved_word_count; ++i) {
        if (strcmp(str, reserved_word_list[i]) == 0)
            return false;
    }
    return true;
}

bool check_name(const char* str) {
    return (str!=nullptr) && check_name_common(str) && check_reserved_names(str);
}
