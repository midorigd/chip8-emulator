#include "utils.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void createTestBin(const std::string& filename) {
    std::vector<long> vals { 0xABCD1234, 0x567890EF, 0x83B4AC1F, 0xF72B1AC8 };
    std::ofstream binFile(filename, std::ios::binary);

    if (!binFile) {
        std::cerr << "Test binary file " << filename << " not opened\n";
        exit(1);
    }

    for (long val : vals) {
        binFile.write(reinterpret_cast<char*>(&val), 4);
    }

    binFile.close();
}
