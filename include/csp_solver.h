#ifndef CSP_SOLVER_H
#define CSP_SOLVER_H

#include "models.h"
#include <vector>
#include <unordered_map>
#include <optional>
#include <chrono>
#include <unordered_set>
#include <string>

// Represents a variable in the CSP (a session that needs to be scheduled)
struct LectureVar {
    string varID;           // unique identifier
    string courseID;        // e.g., "CSC111"
    int year;               // 1, 2, or 3
    int groupId;            // 1-3 for Y1/Y2, 0 for Y3 specializations
    int sectionId;          // for Japanese: 1-3, for labs: 1-3, else 0
    string specialization;  // "AID", "BIF", "CSC", "CNC" for Year 3, empty otherwise
    string sessionType;     // "LECTURE", "LAB"
    int lengthMin;          // 90 minutes
};

// Represents a value that can be assigned to a variable
struct AssignmentValue {
    int timeslotIndex;      // index into timeSlots vector
    string roomID;
    string instructorID;
};

struct CSPResult {
    bool success;
    std::unordered_map<std::string, AssignmentValue> assignments; // varID -> value
    int hardViolations;
    int softCost;
    double solveSeconds;
};

class CSPSolver {
public:
    CSPSolver(const std::vector<Course>& courses,
              const std::vector<Instructor>& instructors,
              const std::vector<InstructorCourse>& instructorCourses,
              const std::vector<Room>& rooms,
              const std::vector<TimeSlot>& timeSlots);

    // build the variable set (one LectureVar per required session)
    void buildLectureVariables();

    // generate domains for each variable (timeslot x room x instructor combinations)
    void buildDomains();

    // run solver (backtracking with MRV + forward checking)
    CSPResult solve(int maxSolutions = 1);

    // helper: pretty print assignment
    void printResult(const CSPResult& r, const std::vector<LectureVar>& vars,
                     const std::vector<TimeSlot>& timeSlots, const std::vector<Room>& rooms);

    // expose variables read-only so callers (main.cpp) can print results
    const std::vector<LectureVar>& getVariables() const { return variables; }

private:
    const std::vector<Course>& courses;
    const std::vector<Instructor>& instructors;
    const std::vector<InstructorCourse>& instructorCourses;
    const std::vector<Room>& rooms;
    const std::vector<TimeSlot>& timeSlots;

    std::vector<LectureVar> variables;
    std::vector<std::vector<AssignmentValue>> domains; // domain per variable

    // helper indices
    std::unordered_map<std::string, const Course*> courseIndex;
    std::unordered_map<std::string, std::vector<std::string>> courseToInstructors; // courseID -> list of instructorIDs

    // internal backtracking
    CSPResult backtrackSearch();

    // utilities
    bool isHardConflict(const AssignmentValue& a, const AssignmentValue& b, 
                        const LectureVar& va, const LectureVar& vb) const;
    int computeSoftCost(const std::unordered_map<std::string, AssignmentValue>& assignments) const;
};

#endif // CSP_SOLVER_H