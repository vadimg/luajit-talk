module(..., package.seeall)

local heap = require('heap')

function getTop(n)
    local i = 0
    local counts = {}
    local size = data_size()

    -- make a counts hashtable of string -> count of occurrences
    while i < size do
        local s = data_elem(i)
        counts[s] = (counts[s] or 0) + 1
        i = i + 1
    end

    -- insert [count, string] pairs into a heap of size n
    local tops = heap:new(function(a, b) return a[1] < b[1] end)
    for s, c in pairs(counts) do
        if #tops < n then
            tops:insert({c, s})
        elseif tops[1][1] < c then
            tops:pop()
            tops:insert({c, s})
        end
    end

    -- create the output string
    local ret = {}
    while not tops:empty() do
        local s, c = unpack(tops:pop())
        table.insert(ret, s .. ': ' .. tostring(c))
    end

    return table.concat(ret, '\n') .. '\n'
end
