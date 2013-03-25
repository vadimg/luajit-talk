#include <iostream>
#include <stdexcept>

#include <math.h>
#include <stdlib.h>

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

using namespace std;

// structs / functions to expose to lua
extern "C" {

typedef struct Point {
    double x;
    double y;
} Point;

// distance between 2 points
double point_distance(const Point* a, const Point* b) {
    return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
}

// get a unit point (where the size = 1 but the angle is the same)
// allocates memory, so be sure to free with free()
Point* point_unit(const Point* p) {
    Point* u = (Point*) malloc(sizeof(Point));
    double size = sqrt(pow(p->x, 2) + pow(p->y, 2));
    u->x = p->x / size;
    u->y = p->y / size;
    return u;
}

// get the polar coordinates of the point
void point_polar(const Point* p, double* size, double* angle) {
    *size = sqrt(pow(p->x, 2) + pow(p->y, 2));
    *angle = atan(p->y / p->x);
}

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

void runLua(const char* filename) {
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
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " [script name]" << endl;
        return 1;
    }

    try {
        runLua(argv[1]);
        return 0;
    } catch (const runtime_error& e) {
        cerr << e.what() << endl;
        return 1;
    }
}
