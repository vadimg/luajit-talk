#include "data.h"

#include <string>
#include <vector>
#include <fstream>

using namespace std;

namespace {
static vector<string> lines;
}

extern "C" {

void load_data(const char* filename) {
    ifstream file(filename);
    if (!file) {
        return;
    }

    string line;

    while (file) {
        getline(file, line);
        if (line.length()) {
            lines.push_back(line);
        }
    }
}

size_t data_size() {
    return lines.size();
}

const char* data_elem(size_t i) {
    return lines[i].c_str();
}

}
