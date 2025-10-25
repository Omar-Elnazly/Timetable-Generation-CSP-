#pragma once
#include <vector>
#include <string>
#include <map>
#include "models.h"
#include "../sqlite//sqlite3.h"
using namespace std;

class DatabaseManager {
private:
    sqlite3* db;

public:
    explicit DatabaseManager(const string& dbFile);
    ~DatabaseManager();

    vector<Course> getCourses();
    vector<Instructor> getInstructors();
    vector<InstructorCourse> getInstructorCourses();
    vector<Room> getRooms();
    vector<TimeSlot> getTimeSlots();
    
    vector<Instructor> getInstructorsForCourse(const string& courseID);
};
