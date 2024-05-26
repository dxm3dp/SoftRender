#include "common/cpplang.h"
#include "common/Logger.h"

int main(int argc, char** argv)
{
    std::cout << "Hello, World!" << std::endl;

    // 读取 obj 格式模型文件
    // 解析文件
    // 实际应用

    // 读取文件
    std::string filePath = "../models/diablo3/diablo3.obj";
    LOGI("Reading file: %s", filePath.c_str());

    return 0;
}