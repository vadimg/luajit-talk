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

print('p1 unit', p1:unit())

print('p2 polar', p2:polar())
print('p2 unit polar', p2:unit():polar())
