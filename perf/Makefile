CXX_SOURCES = data.cc
CXXFLAGS = -std=c++0x -O3
LFLAGS =

LUA_CXXFLAGS = -I../LuaJIT-2.0.1/src
V8_CXXFLAGS = -I../v8-master/include
LUA_LFLAGS = -Wl,-E -L../LuaJIT-2.0.1/src -Wl,-Bstatic -lluajit -Wl,-Bdynamic -lm -ldl
V8_LFLAGS = -L../v8-master/out/native/obj.target/tools/gyp -lv8_base -lv8_snapshot -lpthread

CXX_OBJECTS = $(CXX_SOURCES:.cc=.o)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: cplusplus1 cplusplus2 lua-ffi lua-api v8

cplusplus1: cplusplus1.cc $(CXX_OBJECTS)
	$(CXX) -o $@ $(CXXFLAGS) $(LFLAGS) $^

cplusplus2: cplusplus2.cc $(CXX_OBJECTS)
	$(CXX) -o $@ $(CXXFLAGS) $(LFLAGS) $^

lua-ffi: lua-ffi.cc $(CXX_OBJECTS)
	$(CXX) -o $@ $(LUA_CXXFLAGS) $(CXXFLAGS) $^ $(LUA_LFLAGS) $(LFLAGS)

lua-api: lua-api.cc $(CXX_OBJECTS)
	$(CXX) -o $@ $(LUA_CXXFLAGS) $(CXXFLAGS) $^ $(LUA_LFLAGS) $(LFLAGS)

v8: v8.cc $(CXX_OBJECTS)
	$(CXX) -o $@ $(V8_CXXFLAGS) $(CXXFLAGS) $^ $(V8_LFLAGS) $(LFLAGS)

clean:
	rm -f cplusplus1 cplusplus2 lua-ffi lua-api v8 *.o
