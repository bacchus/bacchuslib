#pragma once

#include <iostream>
#include <fstream>
#include <string>

/*
ios::in     Open for input operations.
ios::out	Open for output operations.
ios::binary	Open in binary mode.
ios::ate	Set the initial position at the end of the file.
            If this flag is not set to any value, the initial position is the beginning of the file.
ios::app	All output operations are performed at the end of the file, appending the content to the current content of the file. This flag can only be used in streams open for output-only operations.
ios::trunc	If the file opened for output operations already existed before, its previous content is deleted and replaced by the new one.
*/

/*
defaults

ofstream	ios::out
ifstream	ios::in
fstream     ios::in | ios::out
*/

/*
bad()   Returns true if a reading or writing operation fails. For example in the case that we try to write to a file that is not open for writing or if the device where we try to write has no space left.
fail()  Returns true in the same cases as bad(), but also in the case that a format error happens, like when an alphabetical character is extracted when we are trying to read an integer number.
eof()   Returns true if a file open for reading has reached the end.
good()  Returns false in the same cases in which calling any of the previous functions would return true.
clear() Reset the state flags
*/

/*
ios::beg	offset counted from the beginning of the stream
ios::cur	offset counted from the current position of the stream pointer
ios::end	offset counted from the end of the stream
*/

/*
get and put stream pointers
tellg() and tellp()
seekg() and seekp()
*/

namespace bacchus {

inline void file_write(const std::string& fname, const std::string& str) {
    std::ofstream f(fname);
    if (f.is_open()) {
        f << str;
        f.close();
    } else std::cout << "unable to open" << fname << std::endl;
}

inline void file_append(const std::string& fname, const std::string& str) {
    std::ofstream f(fname, std::ios::out|std::ios::app);
    if (f.is_open()) {
        f << str;
        f.close();
    } else std::cout << "unable to open" << fname << std::endl;
}

inline long file_size(const std::string& fname) {
    std::ifstream f(fname);
    if (f.is_open()) {
        long beg = f.tellg();
        f.seekg(0, std::ios::end);
        long end = f.tellg();
        f.close();
        return end-beg;
    } else std::cout << "unable to open: " << fname << std::endl;
    return 0;
}

inline void file_readline(const std::string& fname) {
    std::ifstream f(fname);
    if (f.is_open()) {
        std::string line;
        while (std::getline(f, line)) {
            std::cout << line << std::endl;
        }
        f.close();
    } else std::cout << "unable to open: " << fname << std::endl;
}

inline std::string file_readbuff(const std::string& fname) {
    //std::ifstream ifs(fname);
    //std::stringstream buffer;
    //buffer << ifs.rdbuf();
    //return buffer.str();

    std::ifstream f(fname);//, std::ios::binary|std::ios::in|std::ios::ate
    if (f.is_open()) {
        long beg = f.tellg();
        f.seekg(0, std::ios::end);
        long end = f.tellg();
        long size = end - beg;
        std::vector<char> vc(size);
        char* memblock = &vc[0];
        f.seekg(0, std::ios::beg);
        f.read(memblock, size);
        f.close();
        vc.push_back('\0');
        return std::string(vc.begin(), vc.end());
    } else std::cout << "unable to open: " << fname << std::endl;
    return "";
}

//TODO: c, not tested
//inline long getFileSize(FILE* inFile) {
//    long curPos = ftell(inFile);
//    fseek(inFile, 0, SEEK_END);
//    long fileSize = ftell(inFile);
//    fseek(inFile, curPos, SEEK_SET);
//    return fileSize;
//}

//TODO: posix, not tested
//inline int file_size(const char* fname) {
//    struct stat fileStat;
//    stat(fname, &fileStat);
//    return fileStat.st_size;
//}

//std::string readTextFile(const std::string& fn) {
//    std::ifstream ifs(fn.c_str());
//    std::ostringstream oss;
//    if (ifs.is_open()) {
//        while (ifs.good()) {
//            std::string tmp;
//            ifs >> tmp;
//            oss << tmp << " ";
//        }
//    }
//    std::cout << oss.str() << std::endl;
//    return oss.str();
//}

} // namespace bacchus
