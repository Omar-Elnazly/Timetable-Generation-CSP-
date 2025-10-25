#ifndef CSP_SOLVER_H
#define CSP_SOLVER_H

#include "models.h"
#include <vector>
#include <unordered_map>
#include <chrono>
#include <string>

struct LectureVar {
    string varID;
    string courseID;
    int year;               // 1-4
    int groupId;            // 1-3 for Y1-2, 0 for Y3-4 specializations
    int sectionId;          // Japanese: 1-3, Labs: 1-3, else 0
    string specialization;  // "AID"|"BIF"|"CSC"|"CNC" for Y3-4
    string sessionType;     // "LECTURE"|"LAB"
    int lengthMin;          // 90
    bool isFullDay;         // true for graduation projects (4 consecutive slots)
};

struct AssignmentValue {
    int timeslotIndex;
    string roomID;
    string instructorID;
};

struct CSPResult {
    bool success;
    std::unordered_map<std::string, AssignmentValue> assignments;
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

    void buildLectureVariables();
    void buildDomains();
    CSPResult solve(int maxSolutions = 1);
    void printResult(const CSPResult& r, const std::vector<LectureVar>& vars,
                     const std::vector<TimeSlot>& timeSlots, const std::vector<Room>& rooms);
    CSPResult scheduleLabsGreedy(const std::vector<LectureVar>& lectureVars,
                                 const std::unordered_map<std::string, AssignmentValue>& lectureAssignments);

    const std::vector<LectureVar>& getVariables() const { return variables; }

private:
    const std::vector<Course>& courses;
    const std::vector<Instructor>& instructors;
    const std::vector<InstructorCourse>& instructorCourses;
    const std::vector<Room>& rooms;
    const std::vector<TimeSlot>& timeSlots;

    std::vector<LectureVar> variables;
    std::vector<std::vector<AssignmentValue>> domains;

    std::unordered_map<std::string, const Course*> courseIndex;
    std::unordered_map<std::string, std::vector<std::string>> courseToInstructors;

    CSPResult backtrackSearch();
    bool isHardConflict(const AssignmentValue& a, const AssignmentValue& b,
                        const LectureVar& va, const LectureVar& vb) const;
    int computeSoftCost(const std::unordered_map<std::string, AssignmentValue>& assignments) const;
};

#endif // CSP_SOLVER_H