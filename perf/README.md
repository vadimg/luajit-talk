Performance
=========

Benchmarks performance of an algorithm with implementations in:

  - C++ (naive)
  - C++ (optimized)
  - V8
  - LuaJIT with Lua API
  - LuaJIT with LuaJIT FFI

The algorithm
-------------

Given a list of 1 million strings, find the top 100 most frequently occurring strings.

### Implementation

   1. Count the occurrences of each string by putting it into a hashtable of string -> count.
   2. Iterate through the hashtable, putting each [count, string] pair into a min-heap (sorted by count) of size 100.

Data
----
There are 3 data files, with 10k, 100k, and 500k unique strings.

Installation
--------------

In the root directory,
`./install.sh`

This will download and build LuaJIT and V8 locally.

In this directory,
`make`

This will build several binaries. Each accepts an argument, which is the data file to read. If no file is specified, it defaults to `data_10k`.

Note
----

The code in here is not the cleanest, especially the v8 code. I wouldn't take an example from it if I were you.

License
-

MIT
