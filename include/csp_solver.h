#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "models.h"

class CSPSolver {
private:
    std::vector<Course> courses;
    std::vector<Instructor> instructors;
    std::vector<Room> rooms;
    std::vector<TimeSlot> timeSlots;
    std::vector<LectureAssignment> solution;

    // Domain tracking for Forward Checking
    std::unordered_map<std::string, std::vector<std::string>> domainInstructors;
    std::unordered_map<std::string, std::vector<std::string>> domainRooms;
    std::unordered_map<std::string, std::vector<int>> domainSlots;

    bool isValidAssignment(const LectureAssignment& a);
    bool backtrack(int index);
    bool forwardCheck(const LectureAssignment& a);
    std::string selectCourseMRV();

public:
    CSPSolver(const std::vector<Course>& c,
              const std::vector<Instructor>& i,
              const std::vector<Room>& r,
              const std::vector<TimeSlot>& t);

    bool solve();
    void printSolution();
};
