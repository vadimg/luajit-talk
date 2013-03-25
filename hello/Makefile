CXXFLAGS = -I../LuaJIT-2.0.1/src # location of lua.h

LFLAGS = -Wl,-E # put global symbols into dynamic symbol table so LuaJIT's FFI can find them
LFLAGS += -L../LuaJIT-2.0.1/src # location of libluajit.a
LFLAGS += -Wl,-Bstatic -lluajit # link luajit statically
LFLAGS += -Wl,-Bdynamic -lm -ldl # link m and dl dynamically

all: hello

hello: hello.cc
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LFLAGS)

hello-dyn: hello.cc
	$(CXX) -o $@ -I/usr/local/include/luajit-2.0 $^ -Wl,-E -lluajit-5.1

clean:
	rm -f hello hello-dyn
