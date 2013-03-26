#include <string>
#include <iostream>
#include <sys/time.h>

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#include "data.h"

using namespace std;

extern "C" {

static int lua_data_size(lua_State *lua) {
    // push the integer result of data_size onto the stack, as a double
    lua_pushnumber(lua, data_size());

    return 1; // number of results
}

static int lua_data_elem(lua_State *lua) {
    // pop the first argument off the stack,
    // make sure it's an integer, and convert it to an int
    int n = luaL_checkinteger(lua, 1);

    // push the string result of data_elem onto the stack
    lua_pushstring(lua, data_elem(n));

    return 1; // number of results
}

}

string runLua(const char* filename) {
    // create a new lua environment
    lua_State* l = luaL_newstate();

    // load the standard library
    luaL_openlibs(l);

    // push the lua c function wrapping data_size onto the stack
    lua_pushcfunction(l, lua_data_size);

    // set the global 'data_size' to call the data_size wrapper
    lua_setglobal(l, "data_size");

    lua_pushcfunction(l, lua_data_elem);
    lua_setglobal(l, "data_elem");

    // load the script, compile it as a function,
    // and push the compiled function onto the stack
    // then,
    // call that compiled function
    if (luaL_loadfile(l, filename) || lua_pcall(l, 0, LUA_MULTRET, 0)) {
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

    gettimeofday(&start, NULL);
    string ret = runLua("lua-api.lua");
    gettimeofday(&end, NULL);

    cout << ret << endl;

    timersub(&end, &start, &diff);
    cerr << "ran in " << 1e3 * (diff.tv_sec + diff.tv_usec/1e6) << " ms" << endl;

    return 0;
}
