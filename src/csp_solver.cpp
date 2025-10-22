// src/csp_solver.cpp
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "../include/csp_solver.h"

using namespace std;

// Constructor: initialize domains using string IDs for courses/instructors/rooms
CSPSolver::CSPSolver(
        const vector<Course>& c,
        const vector<Instructor>& i,
        const vector<Room>& r,
        const vector<TimeSlot>& t
) : courses(c), instructors(i), rooms(r), timeSlots(t)
{
    // Build initial domains for each course (all possible instructors, rooms, slots)
    for (const auto &course : courses) {
        vector<string> instIDs;
        vector<string> roomIDs;
        vector<int> slotIDs;

        for (const auto &inst : instructors) instIDs.push_back(inst.id);
        for (const auto &room : rooms) roomIDs.push_back(room.id);
        for (const auto &slot : timeSlots) slotIDs.push_back(slot.id);

        domainInstructors[course.id] = move(instIDs);
        domainRooms[course.id]       = move(roomIDs);
        domainSlots[course.id]       = move(slotIDs);
    }
}

// Check hard constraints for a single assignment against current partial solution
bool CSPSolver::isValidAssignment(const LectureAssignment& a) {

    for (const auto &existing : solution) {
        if (existing.instructorID == a.instructorID && existing.timeSlotID == a.timeSlotID)
            return false;
        if (existing.roomID == a.roomID && existing.timeSlotID == a.timeSlotID)
            return false;
    }


    // check room type vs course type
    string courseType = "Lecture";
    for (const auto &c : courses) {
        if (c.id == a.courseID) {
            courseType = c.hasLab ? "Lab" : "Lecture";
            break;
        }
    }

    string roomType;
    for (const auto &r : rooms) {
        if (r.id == a.roomID) {
            roomType = r.roomType;
            break;
        }
    }

    if (roomType.empty()) return false; // unknown room
    if (roomType != courseType) return false;

    return true;
}

// MRV: choose the unassigned course with the smallest combined domain size
string CSPSolver::selectCourseMRV() {
    string bestCourse = "";
    size_t bestSize = SIZE_MAX;

    for (const auto &course : courses) {
        // skip already assigned
        bool assigned = false;
        for (const auto &a : solution) {
            if (a.courseID == course.id) { assigned = true; break; }
        }
        if (assigned) continue;

        size_t instCount = domainInstructors[course.id].size();
        size_t roomCount = domainRooms[course.id].size();
        size_t slotCount = domainSlots[course.id].size();

        // if any domain is zero, it's an immediate dead end—return that course (most constrained)
        if (instCount == 0 || roomCount == 0 || slotCount == 0) return course.id;

        // measure = product (could overflow but sizes are small here)
        size_t measure = instCount * roomCount * slotCount;
        if (measure < bestSize) {
            bestSize = measure;
            bestCourse = course.id;
        }
    }

    return bestCourse;
}

// Backtracking with forward checking and MRV
bool CSPSolver::backtrack(int depth) {
    // success if all courses assigned
    if (solution.size() == courses.size()) return true;

    // select most constrained unassigned course
    string courseID = selectCourseMRV();
    if (courseID.empty()) return false;

    // find course metadata pointer
    const Course* coursePtr = nullptr;
    for (const auto &c : courses) {
        if (c.id == courseID) { coursePtr = &c; break; }
    }
    if (!coursePtr) return false;
    const Course &course = *coursePtr;

    // iterate domain values for the selected course
    auto &instDomain = domainInstructors[courseID];
    auto &roomDomain = domainRooms[courseID];
    auto &slotDomain = domainSlots[courseID];

    // iterate copies to allow domain modification during forward checking
    vector<string> instDomainCopy = instDomain;
    vector<string> roomDomainCopy = roomDomain;
    vector<int> slotDomainCopy = slotDomain;

    for (const auto &instID : instDomainCopy) {
        for (const auto &roomID : roomDomainCopy) {
            for (const auto slotID : slotDomainCopy) {
                LectureAssignment a{ courseID, instID, roomID, slotID };

                if (!isValidAssignment(a)) continue;

                // Save domains (shallow copy of maps) to restore on backtrack
                auto oldDomainInstructors = domainInstructors;
                auto oldDomainRooms = domainRooms;
                auto oldDomainSlots = domainSlots;

                // Forward checking: prune conflicting values from other courses
                bool ok = true;
                for (const auto &other : courses) {
                    if (other.id == courseID) continue;

                    // remove same instructor at same timeslot
                    auto &odInst = domainInstructors[other.id];
                    odInst.erase(remove(odInst.begin(), odInst.end(), a.instructorID), odInst.end());

                    // remove same room at same timeslot
                    auto &odRoom = domainRooms[other.id];
                    odRoom.erase(remove(odRoom.begin(), odRoom.end(), a.roomID), odRoom.end());

                    // remove the timeslot itself (if you prefer to prevent reuse of same timeslot globally for other courses,
                    // comment this out if timeslots can be reused by separate instructors/rooms)
                    auto &odSlot = domainSlots[other.id];
                    odSlot.erase(remove(odSlot.begin(), odSlot.end(), a.timeSlotID), odSlot.end());

                    // If any domain becomes empty, fail early
                    if (odInst.empty() || odRoom.empty() || odSlot.empty()) {
                        ok = false;
                        break;
                    }
                }

                if (!ok) {
                    // restore domains and continue
                    domainInstructors = move(oldDomainInstructors);
                    domainRooms = move(oldDomainRooms);
                    domainSlots = move(oldDomainSlots);
                    continue;
                }

                // Accept assignment and recurse
                solution.push_back(a);
                if (backtrack(depth + 1)) return true;

                // Backtrack: remove assignment and restore domains
                solution.pop_back();
                domainInstructors = move(oldDomainInstructors);
                domainRooms = move(oldDomainRooms);
                domainSlots = move(oldDomainSlots);
            }
        }
    }

    return false;
}

bool CSPSolver::solve() {
    cout << "Starting CSP Backtracking Solver with Forward Checking + MRV...\n";
    // ensure solution empty before starting
    solution.clear();
    bool ok = backtrack(0);
    if (ok) cout << "Solution found!\n";
    else cout << "No valid timetable.\n";
    return ok;
}

void CSPSolver::printSolution() {
    cout << "\n===== GENERATED TIMETABLE =====\n";
    if (solution.empty()) {
        cout << "No assignments found.\n";
        return;
    }
    for (const auto &a : solution) {
        cout << "Course " << a.courseID
             << " | Instructor " << a.instructorID
             << " | Room " << a.roomID
             << " | TimeSlot " << a.timeSlotID << "\n";
    }
}
