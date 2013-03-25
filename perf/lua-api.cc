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

extern "C" {

static int lua_data_size(lua_State *lua) {
    lua_pushnumber(lua, data_size());
    return 1; // number of results
}

static int lua_data_elem(lua_State *lua) {
    int n = luaL_checkinteger(lua, 1);
    lua_pushstring(lua, data_elem(n));
    return 1; // number of results
}

}

string runLua(const string& script) {
    lua_State* l = luaL_newstate();
    luaL_openlibs(l);

    lua_pushcfunction(l, lua_data_size);
    lua_setglobal(l, "data_size");

    lua_pushcfunction(l, lua_data_elem);
    lua_setglobal(l, "data_elem");

    if (luaL_loadbuffer(l, script.c_str(), script.size(), "script") || lua_pcall(l, 0, LUA_MULTRET, 0)) {
        const char* ret = lua_tostring(l, -1);
        cerr << "ERROR: " << ret << endl;
        lua_close(l);
        exit(1);
    }

    string ret = lua_tostring(l, -1);

    lua_close(l);

    return ret;
}

int main(int argc, char** argv) {
    struct timeval start, end, diff;

    load_data(argc > 1 ? argv[1] : "data_10k");

    string script;

    read_file(script, "lua-api.lua");

    gettimeofday(&start, NULL);
    string ret = runLua(script);
    gettimeofday(&end, NULL);

    cout << ret << endl;

    timersub(&end, &start, &diff);
    cerr << "ran in " << 1e3 * (diff.tv_sec + diff.tv_usec/1e6) << " ms" << endl;

    return 0;
}
