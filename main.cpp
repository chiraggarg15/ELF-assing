#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

string getArch(uint16_t machine) {
    switch (machine) {
        case 40:  return "armeabi-v7a";
        case 183: return "arm64-v8a";
        case 3:   return "x86";
        case 62:  return "x86-64";
        case 8:   return "mips";
        default:  return "unknown";
    }
}

int main() {
    string folderPath = ".";
    vector<pair<string, string>> results;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.path().extension() != ".so")
            continue;

        ifstream file(entry.path(), ios::binary);
        if (!file)
            continue;

        unsigned char magic[4];
        file.read((char*)magic, 4);

        if (magic[0] != 0x7F || magic[1] != 'E' ||
            magic[2] != 'L'  || magic[3] != 'F')
            continue;

        file.seekg(18); // e_machine offset
        uint16_t machine;
        file.read((char*)&machine, sizeof(machine));

        results.push_back({entry.path().filename().string(),
                           getArch(machine)});
    }

    cout << "Total number of libraries: " << results.size() << "\n\n";
    cout << "File\t\t\t\tArchType\n";
    cout << "------------------------------------------\n";

    for (auto& r : results)
        cout << r.first << "\t\t" << r.second << "\n";

    return 0;
}
