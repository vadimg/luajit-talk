local ffi = require('ffi')

ffi.cdef([[
size_t data_size();
const char* data_elem(size_t i);
]])
local C = ffi.C

function data_elem(i)
    return ffi.string(C.data_elem(i))
end

function data_size()
    return tonumber(C.data_size())
end

local algo = require('algo')

return algo.getTop(100)
