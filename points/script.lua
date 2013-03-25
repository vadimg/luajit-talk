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
print('p1', p1.x, p1.y)
p1.x = 5
print('p1', p1.x, p1.y)

local p2 = ffi.new('Point', {5, 0})
print('p1-p2 distance', C.point_distance(p1, p2))

local Point = {}
Point.__index = Point

function Point:new(x, y)
    return ffi.new('Point', {x, y})
end

function Point:__tostring()
    return string.format('Point(%s, %s)', tostring(self.x), tostring(self.y))
end

function Point:unit()
    local new_point = C.point_unit(self)
    ffi.gc(new_point, C.free) -- call C.free when new_point gets gc'd
    return new_point
end

function Point:polar()
    local args = ffi.new('double[2]') -- create an array of doubles
    C.point_polar(self, args, args + 1) -- do some pointer arithmetic in lua
    return args[0], args[1] -- C arrays are 0-based!
end

-- set Point to be the metatable for all struct Point cdata objects
ffi.metatype('struct Point', Point)

local p1 = Point:new(1, 2)
print('p1', p1)

local p2 = Point:new(0, 5)
print('p2', p2)

print('p1-p2 distance', C.point_distance(p1, p2))

-- this will leak memory because the C function allocates memory and it doesn't get freed
--[[
print(C.point_unit(p1))
--]]

-- use this method because it registers the allocated struct with lua's GC
print('p1 unit', p1:unit())



-- this won't work because the function expects a double*, not a number
--[[
local size = 0
local angle = 0
C.point_polar(p2, size, angle)
--]]

print('p2 polar', p2:polar())
print('p2 unit polar', p2:unit():polar())
