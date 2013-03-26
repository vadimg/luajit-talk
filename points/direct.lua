local ffi = require('ffi')

ffi.cdef([[

void free(void*);

typedef struct Point {
    double x;
    double y;
} Point;

double point_distance(const Point* a, const Point* b);

Point* point_unit(const Point* p);

void point_polar(const Point* p, double* size, double* angle);

]])
local C = ffi.C

local p1 = ffi.new('Point', {1, 2})
print(p1)
print('p1', p1.x, p1.y)
p1.x = 5
print('p1', p1.x, p1.y)

local p2 = ffi.new('Point', {0, 5})
print('p2', p2.x, p2.y)
print('p1-p2 distance', C.point_distance(p1, p2))

-- this will leak memory because the C function allocates memory and it doesn't get freed
local u = C.point_unit(p1)
--ffi.gc(u, C.free) -- call C.free when u gets gc'd
print('u', u.x, u.y)


-- this won't work because the function expects a double*, not a number
--[[
local size = 0
local angle = 0
C.point_polar(p2, size, angle)
print('p2 polar', size, angle)
--]]


--[[
function polar(p)
    local args = ffi.new('double[2]') -- create an array of doubles
    C.point_polar(p, args, args + 1) -- do some pointer arithmetic in lua
    return args[0], args[1] -- C arrays are 0-based!
end
print('p2 polar', polar(p2))
--]]

