Smart Campus Navigation and Facility Analytics System
Project 2, Milestone 1

Purpose
This program loads campus text files and lets a user look up facilities by ID.
The ID lookup uses a custom hash table with separate chaining. Building names
are loaded first so a facility can only refer to an accepted building.

This branch (feature/asanga-facility-hash) is Asanga's facility module.
The campus graph and the department tree are not in this branch yet.
Menu options 3 through 9 print a short message and return to the menu.

Directory layout
include/     header files
src/         implementation files, including main.cpp
data/        instructor files (do not edit)
testdata/    our own test directories
    collision/   IDs that share a bucket, plus a duplicate ID
    bad/         duplicate buildings, bad facility lines, a Windows line ending

Build
From the project root, on a CELL machine or locally:

    make

Or without make:

    g++ -std=c++11 -Wall -Wextra -Iinclude -o campus src/main.cpp src/StringUtil.cpp src/Facility.cpp src/HashTable.cpp src/FacilityDatabase.cpp

Run
The data directory is optional. If you leave it off, the program reads data/.

    ./campus
    ./campus data
    ./campus testdata/collision_case
    ./campus testdata/duplicates
    ./campus testdata/empty_facilities
    ./campus testdata/missing_facilities
    ./campus testdata/bad

Day 2 facility checks, from the project root:

    python3 testdata/run_day2_checks.py

Menu
1 and 2 are the facility options. 0 exits.
Type a full line at every prompt. Names and IDs can include spaces.
A blank line, a word, or a number that is not on the menu prints an error
and shows the menu again.
