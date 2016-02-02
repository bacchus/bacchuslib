#pragma once

namespace bacchus {
/*
uchar* readBinFile(const char* fileName, long& fileSize) {
    FILE* inFile = fopen(fileName, "rb");
    if (!inFile)
        printf("err 1");
    fileSize = getFileSize(inFile);
    uchar* buff = new uchar[fileSize];
    fread(buff, 1, fileSize, inFile);
    fclose(inFile);
    return buff;
}

void writeCppFile(const char* fileName, const char* name, uchar* buff, long buffSize) {
    FILE* outFile = fopen(fileName, "w");
    if (!outFile)
        printf("err 2");
    fprintf(outFile, "// %s\n", name);
    fprintf(outFile, "#pragma once\n");
    fprintf(outFile, "static uchar %s[] = {", name);
    for (long i=0; i<buffSize-1; ++i) {
        fprintf(outFile, "0x%02x,", buff[i]);
        if (i % 10 == 0)
            fprintf(outFile, "\n");
    }
    fprintf(outFile, "0x%02x", buff[buffSize-1]);
    //fprintf(outFile, "0x00"); // zero-terminated
    fprintf(outFile, "\n};\n");
    fclose(outFile);
}

int main(int argc, char* argv[]) {
    if (argc < 4)
        printf("usage: %s <in> <out> <name>", argv[0]);
    long fileSize;
    uchar* buff = readBinFile(argv[1], fileSize);
    writeCppFile(argv[2], argv[3], buff, fileSize);
    printf("%i bytes written", fileSize);
    return 0;
}
*/
} // namespace bacchus

