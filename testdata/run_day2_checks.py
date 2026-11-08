# Day 2 checks for the facility module.
# Run from the project root: python3 testdata/run_day2_checks.py

import subprocess
import sys

def run(folder, lines):
    text = "\n".join(lines) + "\n"
    result = subprocess.run(
        ["./campus", folder],
        input=text,
        text=True,
        capture_output=True,
    )
    if result.returncode != 0:
        return result.returncode, result.stdout + result.stderr
    return 0, result.stdout

checks = []

def expect(name, output, needles, absent=None):
    missing = [n for n in needles if n not in output]
    present = []
    if absent:
        present = [n for n in absent if n in output]
    if missing or present:
        checks.append((name, False, missing, present))
    else:
        checks.append((name, True, [], []))

code, out = run("data", ["1", "2", "F004", "2", "f004", "2", "F999", "2", "004", "2", "F00A", "2", "F001", "0"])
expect(
    "sample load, list, and ID search",
    out,
    [
        "buildings.txt: 7 accepted, 0 rejected",
        "facilities.txt: 10 accepted, 0 rejected (hash capacity 11, entries 10, collisions 1)",
        "F001 | Main Reading Room | Library | Library",
        "F002 | Tutoring Center | Library | Tutoring Center",
        "F003 | Student Services Office | Student Union | Student Services Office",
        "F004 | Computer Lab A | Science Building | Computer Lab",
        "F005 | Room 101 | Engineering Building | Classroom",
        "F006 | Computer Lab B | Engineering Building | Computer Lab",
        "F007 | Room 101 | Business Building | Classroom",
        "F008 | Fitness Center | Gym | Recreation",
        "F009 | Room 205 | Science Building | Classroom",
        "F010 | Research Help Desk | Library | Library",
        "Total facilities: 10",
        "Found: F004 | Computer Lab A | Science Building | Computer Lab",
        "Not found: no facility with ID F999",
        "Invalid format: an ID is F followed by exactly three digits.",
        "Found: F001 | Main Reading Room | Library | Library",
    ],
)
# f004 must hit the same record. Count the Computer Lab A line from searches plus the list.
if out.count("Found: F004 | Computer Lab A | Science Building | Computer Lab") < 2:
    checks.append(("lowercase f004 matches F004", False, ["second Found line"], []))
else:
    checks.append(("lowercase f004 matches F004", True, [], []))

code, out = run("testdata/collision_case", ["1", "2", "F001", "2", "F013", "2", "F025", "2", "f001", "0"])
expect(
    "collision_case keeps every ID and ignores the duplicate",
    out,
    [
        "Warning: facilities.txt line 8: duplicate facility ID F001",
        "facilities.txt: 3 accepted, 1 rejected (hash capacity 11, entries 3, collisions 2)",
        "F001 | Reading Room | Library | Library",
        "F013 | Study Nook | Library | Study Room",
        "F025 | Archive Desk | Library | Library",
        "Total facilities: 3",
        "Found: F001 | Reading Room | Library | Library",
        "Found: F013 | Study Nook | Library | Study Room",
        "Found: F025 | Archive Desk | Library | Library",
    ],
    absent=["Duplicate Should Fail"],
)

code, out = run("testdata/duplicates", ["1", "2", "f001", "0"])
expect(
    "duplicate ID does not replace the first record",
    out,
    [
        "Warning: facilities.txt line 2: duplicate facility ID F001",
        "facilities.txt: 1 accepted, 1 rejected (hash capacity 11, entries 1, collisions 0)",
        "F001 | Main Reading Room | Library | Library",
        "Total facilities: 1",
        "Found: F001 | Main Reading Room | Library | Library",
    ],
    absent=["Should Not Replace"],
)

code, out = run("testdata/unknown_building", ["1", "0"])
expect(
    "unknown building is rejected and later records still load",
    out,
    [
        "Warning: facilities.txt line 2: unknown building Not A Building",
        "facilities.txt: 2 accepted, 1 rejected",
        "F001 | Main Reading Room | Library | Library",
        "F003 | Still Here | Library | Study",
        "Total facilities: 2",
    ],
    absent=["Ghost Lab"],
)

code, out = run("testdata/validation", ["1", "2", "F004", "2", "F006", "0"])
expect(
    "field count, empty field, bad IDs, whitespace, and a Windows line",
    out,
    [
        "Warning: facilities.txt line 4: expected 4 fields separated by |",
        "Warning: facilities.txt line 5: empty field",
        "Warning: facilities.txt line 6: invalid ID format, expected F followed by exactly three digits",
        "Warning: facilities.txt line 7: invalid ID format, expected F followed by exactly three digits",
        "Warning: facilities.txt line 9: expected 4 fields separated by |",
        "facilities.txt: 3 accepted, 5 rejected",
        "F001 | Main Reading Room | Library | Library",
        "F004 | Computer Lab A | Library | Computer Lab",
        "F006 | Windows Line | Library | Lab",
        "Total facilities: 3",
        "Found: F004 | Computer Lab A | Library | Computer Lab",
        "Found: F006 | Windows Line | Library | Lab",
    ],
)

code, out = run("testdata/empty_facilities", ["1", "0"])
expect(
    "empty facilities file",
    out,
    [
        "facilities.txt: 0 accepted, 0 rejected (hash capacity 11, entries 0, collisions 0)",
        "No facilities are loaded.",
    ],
)

code, out = run("testdata/missing_facilities", ["1", "0"])
expect(
    "missing facilities file",
    out,
    [
        "Error: cannot open file facilities.txt",
        "facilities.txt: 0 accepted, 0 rejected (hash capacity 11, entries 0, collisions 0)",
        "No facilities are loaded.",
    ],
)

failed = 0
for name, ok, missing, present in checks:
    if ok:
        print("PASS", name)
    else:
        failed += 1
        print("FAIL", name)
        if missing:
            print("  missing:", missing)
        if present:
            print("  should not contain:", present)

sys.exit(1 if failed else 0)
