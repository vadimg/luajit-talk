#include <string>
#include <fstream>
#include <iostream>
#include <sys/time.h>

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#include "data.h"

using namespace std;

bool read_file(std::string& output, const std::string& filename) {
    std::ifstream t(filename.c_str());
    if (!t.good()) {
        return false;
    }

    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    output.resize(size);
    t.seekg(0);
    t.read(&output[0], size);
    return true;
}

string runLua(const string& script) {
    // create a new lua environment
    lua_State* l = luaL_newstate();

    // load the standard library
    luaL_openlibs(l);

    // load the script, compile it as a function,
    // and push the compiled function onto the stack
    // then,
    // call that compiled function
    if (luaL_loadbuffer(l, script.c_str(), script.size(), "script") || lua_pcall(l, 0, LUA_MULTRET, 0)) {
        // there was an error, pop it off the stack
        const char* ret = lua_tostring(l, -1);
        cerr << "ERROR: " << ret << endl;
        lua_close(l);
        exit(1);
    }

    // pop the result off the stack and convert it to a string
    string ret = lua_tostring(l, -1);

    // close the lua environment
    lua_close(l);

    return ret;
}

int main(int argc, char** argv) {
    struct timeval start, end, diff;

    load_data(argc > 1 ? argv[1] : "data_10k");

    string script;

    read_file(script, "lua-ffi.lua");

    gettimeofday(&start, NULL);
    string ret = runLua(script);
    gettimeofday(&end, NULL);

    cout << ret << endl;

    timersub(&end, &start, &diff);
    cerr << "ran in " << 1e3 * (diff.tv_sec + diff.tv_usec/1e6) << " ms" << endl;

    return 0;
}
