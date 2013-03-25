local ffi = require('ffi')

ffi.cdef([[
void print_hello(const char* who);
]])

ffi.C.print_hello('World')

return "Goodbye World"
