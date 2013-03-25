#include <string>
#include <iostream>
#include <sstream>
#include <queue>
#include <unordered_map>
#include <sys/time.h>
#include <string.h>

#include "data.h"

using namespace std;

struct CountPair {
    unsigned count;
    const char* str;

    CountPair(unsigned c, const char* s) : count(c), str(s) {}
};

bool operator<(const CountPair& a, const CountPair& b) {
    return b.count < a.count;
}

class CStringHash {
  public:
    size_t operator ()(const char* str) const {
        // djb2 string hash algorithm
        size_t hash = 5381;
        int c;

        while (c = *str++) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }

        return hash;
    }
};

class CStringEqual {
  public:
    bool operator ()(const char* a, const char* b) const {
        return strcmp(a, b) == 0;
    }
};


string getTop(int n) {
    unordered_map<const char*, unsigned, CStringHash, CStringEqual> counts;
    priority_queue<CountPair> tops; // priority_queue is implemented as a heap

    size_t size = data_size();

    // make a counts hashtable of string -> count of occurrences
    for (unsigned i=0; i < size; ++i) {
        ++counts[data_elem(i)];
    }

    // insert [count, string] pairs into a heap of size n
    for (auto it = counts.begin(); it != counts.end(); ++it) {
        unsigned count = it->second;
        if (tops.size() < n) {
            CountPair c(count, it->first);
            tops.push(c);
        } else if (tops.top().count < count) {
            tops.pop();
            CountPair c(count, it->first);
            tops.push(c);
        }
    }

    // create the output string
    stringstream ss;
    while (tops.size()) {
        auto& it = tops.top();
        ss << it.count << ": " << it.str << "\n";
        tops.pop();
    }

    return ss.str();
}

int main(int argc, char** argv) {
    struct timeval start, end, diff;

    load_data(argc > 1 ? argv[1] : "data_10k");

    gettimeofday(&start, NULL);
    string top = getTop(100);
    gettimeofday(&end, NULL);

    cout << top << endl;

    timersub(&end, &start, &diff);
    cerr << "ran in " << 1e3 * (diff.tv_sec + diff.tv_usec/1e6) << " ms" << endl;

    return 0;
}
