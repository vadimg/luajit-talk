#include <string>
#include <iostream>
#include <stdexcept>

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

using namespace std;

// C function which will be exposed to lua
extern "C"
void print_hello(const char* who) {
    cout << "Hello " << who << endl;
}

// traceback function taken straight out of luajit.c
static int traceback(lua_State *L)
{
    if (!lua_isstring(L, 1)) { /* Non-string error object? Try metamethod. */
        if (lua_isnoneornil(L, 1) ||
                !luaL_callmeta(L, 1, "__tostring") ||
                !lua_isstring(L, -1))
            return 1;  /* Return non-string error object. */
        lua_remove(L, 1);  /* Replace object by result of __tostring metamethod. */
    }
    luaL_traceback(L, L, lua_tostring(L, 1), 1);
    return 1;
}

// A RAII class to make sure lua state gets closed when exceptions are thrown
class LuaClose {
    lua_State* L;
public:
    LuaClose(lua_State* state) : L(state) {}
    ~LuaClose() {
        lua_close(L);
    }
};

string runLua(const char* filename) {
    // create a new lua environment
    lua_State* L = luaL_newstate();

    // make sure it gets closed even when exception is thrown
    LuaClose closer(L);

    // load the standard library
    luaL_openlibs(L);

    // add the traceback function to the stack
    lua_pushcfunction(L, traceback);

    // load the file, compile it as a function,
    // and push the compiled function onto the lua stack
    if (luaL_loadfile(L, filename)) {
        throw runtime_error(lua_tostring(L, -1));
    }

    // call the compiled function (safely: prints traceback on error)
    // args: (state, nargs, nresults, error function stack location)
    if (lua_pcall(L, 0, LUA_MULTRET, 1)) {
        throw runtime_error(lua_tostring(L, -1));
    }

    // get the result as a string
    string ret = lua_tostring(L, -1);

    return ret;
}

int main(int argc, char** argv) {
    try {
        string ret = runLua("script.lua");
        cout << ret << endl;
        return 0;
    } catch (const runtime_error& e) {
        cerr << e.what() << endl;
        return 1;
    }
}
