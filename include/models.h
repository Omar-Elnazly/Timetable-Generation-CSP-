#pragma once
#include <string>
#include <vector>
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

struct ClassSession {
    string courseID;
    string courseName;
    int year;
    int group;          // 1, 2, or 3
    int section;        // 1, 2, or 3 (for Japanese language only)
    string sessionType; // "Lecture" or "Lab"
    string instructorID;
    string instructorName;
    string roomID;
    int timeSlotID;
    string day;
    string timeRange;
};
