#include "common/cpplang.h"
#include "common/Logger.h"
#include "geometry/geometry.h"


int main(int argc, char** argv)
{
    std::cout << "Hello, World!" << std::endl;

    // 读取 obj 格式模型文件
    // 解析文件
    // 实际应用

    // 读取文件
    std::string filePath = "../models/diablo3/diablo3_pose.obj";
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        LOGE("Failed to open file: %s", filePath.c_str());
        return -1;
    }
    std::string line;
    int v, vt, vn, f;
    while (!file.eof())
    {
        std::getline(file, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v "))
        {
            v++;
        }
        else if (!line.compare(0, 3, "vt "))
        {
            vt++;
        }
        else if (!line.compare(0, 3, "vn "))
        {
            vn++;
        }
        else if (!line.compare(0, 2, "f "))
        {
            f++;
        }
    }

    LOGI("v: %d, vt: %d, vn: %d, f: %d", v, vt, vn, f);

    return 0;
}