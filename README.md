# Timetable Generation using CSP

A Constraint Satisfaction Problem solver for automatic university timetable generation using backtracking with MRV heuristic and forward checking.

## Features

- **Fast**: Generates conflict-free schedules in ~2.5 seconds
- **Efficient**: Backtracking + MRV + Forward Checking algorithm
- **Flexible**: Supports lectures, labs, and section-based courses
- **Validated**: Zero hard constraint violations

## Quick Start

```bash
cd cmake-build-debug
make
./TestSql
```

## Problem Specifications

### Scheduling Parameters
- Years: 1-2 (3 groups each)
- Time Slots: 20 (5 days × 4 slots, 90-minute blocks)
- Rooms: 60 (Classrooms, Labs, Halls, Theaters)
- Instructors: 44 (Professors & Assistant Professors)

### Session Types
1. Regular Lectures: One per group
2. Japanese Language: 3 sections per group
3. Lab Sections: 3 sections per course

## Constraints

### Hard Constraints (Must Satisfy)
- No time conflicts (room/instructor/student)
- Same professor teaches all groups of each course
- Specialized lab room requirements
- Lab sections independent of student groups

### Soft Constraints (Preferences)
- Avoid 9:00 AM slots (penalty: +5)
- Spread sessions across days (penalty: +2/overlap)

## Architecture

```
data/
  database.db               SQLite database
  updated_database.sql      Schema and data
include/
  csp_solver.h              Solver interface
  database_manager.h        DB layer
  models.h                  Data structures
src/
  csp_solver.cpp            Core CSP implementation
  database_manager.cpp      DB operations
  main.cpp                  Entry point
```

## Algorithm

The solver uses:
1. **Backtracking Search**: Recursive depth-first exploration
2. **MRV Heuristic**: Select variable with minimum remaining values
3. **Forward Checking**: Prune conflicting future assignments
4. **Constraint Propagation**: Enforce professor consistency

```
while (unassigned variables exist):
  select variable with smallest domain (MRV)
  for each value in domain:
    if no conflicts:
      assign value
      forward check and prune domains
      recursively solve remaining variables
    if solved: return success
  backtrack
```

## Technical Details

**Language**: C++17  
**Database**: SQLite3  
**Build System**: CMake 3.28  
**Complexity**: O(d^n) worst case, optimized by MRV and pruning

### Performance
- Variables: 60 sessions
- Domain Size: ~107K possible assignments
- Solve Time: 2.5-3 seconds
- Hard Violations: 0
- Soft Cost: ~145

## Database Schema

Key tables:
- `Courses`: Course metadata and requirements
- `Instructors`: Faculty with qualifications
- `Rooms`: Room types and locations
- `TimeSlots`: Available scheduling blocks
- `InstructorCourse`: Course-instructor assignments

## Build Requirements

- CMake 3.28+
- C++17 compiler
- SQLite3 (included)

## Authors

University Timetable Generation Project
