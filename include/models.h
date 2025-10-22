#pragma once
#include <string>
#include <vector>
using namespace std;

struct Course {
    string id;
    string name;
    int credits;
    bool hasLecture;
    bool hasLab;
    bool hasLongTut;
    bool hasShortTut;
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
    string slotType;
    string startTxt;
    string endTxt;
    int startMin;
    int endMin;
};


// ZINA
struct LectureAssignment {
    string courseID;
    string instructorID;
    string roomID;
    int timeSlotID;
};
