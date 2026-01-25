#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

struct Course {
    string id;
    string name;
    int credits;
    string type;
    int year;
    string specialization;
    bool hasLecture;
    bool hasLab;
    bool isGradProject;
};

struct Instructor {
    string id;
    string name;
    string role;
    string preferredSlots;
    string qualifiedCourses;
};

struct InstructorCourse {
    string instructorID;
    string courseID;
};

struct Room {
    string id;
    string building;
    string roomName;
    int capacity;
    string roomType;
};

struct TimeSlot {
    int id;
    string day;
    string startTxt;
    string endTxt;
    int startMin;
    int endMin;
};

inline string minTo12Hour(int mins) {
    int h = mins / 60;
    int m = mins % 60;
    bool pm = (h >= 12);
    if (h > 12) h -= 12;
    if (h == 0) h = 12;
    
    ostringstream ss;
    ss << (h < 10 ? "0" : "") << h << ":" << (m < 10 ? "0" : "") << m << (pm ? "PM" : "AM");
    return ss.str();
}
