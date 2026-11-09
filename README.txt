Smart Campus Navigation and Facility Analytics System
Project 2, Milestone 1

Purpose
This program loads campus text files and lets a user look up facilities by ID
and explore how campus buildings connect.
The ID lookup uses a custom hash table with separate chaining. Building names
are loaded first so a facility can only refer to an accepted building.
The campus is an undirected graph kept as an adjacency list. The user can view
it, see counts and degrees, and run a BFS to find reachable buildings.

The department tree is not in this branch yet. Menu options 6 through 9 print
a short message and return to the menu.

Directory layout
include/     header files
src/         implementation files, including main.cpp
data/        instructor files (do not edit)
testdata/    our own test directories
    collision/   IDs that share a bucket, plus a duplicate ID
    bad/         duplicate buildings, bad facility lines, a Windows line ending
    graph_bad/   bad connections: duplicates, reversed, self, unknown, wrong fields
    graph_multi/ several connected components
    graph_empty/ buildings with an empty connections file
    graph_nobuildings/ an empty buildings file

Build
From the project root, on a CELL machine or locally:

    make

Or without make:

    g++ -std=c++11 -Wall -Wextra -Iinclude -o campus src/main.cpp src/StringUtil.cpp src/Facility.cpp src/HashTable.cpp src/FacilityDatabase.cpp src/CampusGraph.cpp

Run
The data directory is optional. If you leave it off, the program reads data/.

    ./campus
    ./campus data
    ./campus testdata/collision
    ./campus testdata/bad
    ./campus testdata/graph_bad

Menu
1 and 2 are the facility options. 3, 4, and 5 are the graph options:
view the graph, show counts and degrees, and find reachable buildings (BFS).
0 exits.
Type a full line at every prompt. Names and IDs can include spaces.
A blank line, a word, or a number that is not on the menu prints an error
and shows the menu again.
