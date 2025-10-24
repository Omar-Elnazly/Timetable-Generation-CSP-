#include "../include/database_manager.h"
#include <iostream>
#include <stdexcept>

DatabaseManager::DatabaseManager(const string &dbFile) {
    if (sqlite3_open(dbFile.c_str(), &db)) {
        cerr << "Database error: " << sqlite3_errmsg(db) << endl;
        db = nullptr;
    }
}

DatabaseManager::~DatabaseManager() {
    if (db) sqlite3_close(db);
}

vector<Course> DatabaseManager::getCourses()
{
    vector<Course> list;
    const char *sql = "SELECT CourseID, CourseName, Credits, Type, Year, Specialization, HasLecture, HasLab, IsGradProject FROM Courses;";
    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        cerr << "Failed to prepare getCourses statement: " << sqlite3_errmsg(db) << endl;
        return list;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        Course c;
        const unsigned char *t0 = sqlite3_column_text(stmt, 0);
        const unsigned char *t1 = sqlite3_column_text(stmt, 1);
        const unsigned char *t3 = sqlite3_column_text(stmt, 3);
        const unsigned char *t5 = sqlite3_column_text(stmt, 5);
        
        c.id = t0 ? reinterpret_cast<const char *>(t0) : string();
        c.name = t1 ? reinterpret_cast<const char *>(t1) : string();
        c.credits = sqlite3_column_int(stmt, 2);
        c.type = t3 ? reinterpret_cast<const char *>(t3) : string();
        c.year = sqlite3_column_int(stmt, 4);
        c.specialization = t5 ? reinterpret_cast<const char *>(t5) : string();
        c.hasLecture = sqlite3_column_int(stmt, 6) != 0;
        c.hasLab = sqlite3_column_int(stmt, 7) != 0;
        c.isGradProject = sqlite3_column_int(stmt, 8) != 0;
        
        list.push_back(std::move(c));
    }

    if (rc != SQLITE_DONE)
    {
        cerr << "Error while iterating courses: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
    return list;
}

vector<Instructor> DatabaseManager::getInstructors()
{
    vector<Instructor> list;
    const char *sql = "SELECT InstructorID, Name, Role, PreferredSlots, QualifiedCourses FROM Instructor;";
    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        cerr << "Failed to prepare getInstructors statement: " << sqlite3_errmsg(db) << endl;
        return list;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        Instructor i;
        const unsigned char *t0 = sqlite3_column_text(stmt, 0);
        const unsigned char *t1 = sqlite3_column_text(stmt, 1);
        const unsigned char *t2 = sqlite3_column_text(stmt, 2);
        const unsigned char *t3 = sqlite3_column_text(stmt, 3);
        const unsigned char *t4 = sqlite3_column_text(stmt, 4);
        i.id = t0 ? reinterpret_cast<const char *>(t0) : string();
        i.name = t1 ? reinterpret_cast<const char *>(t1) : string();
        i.role = t2 ? reinterpret_cast<const char *>(t2) : string();
        i.preferredSlots = t3 ? reinterpret_cast<const char *>(t3) : string();
        i.qualifiedCourses = t4 ? reinterpret_cast<const char *>(t4) : string();
        list.push_back(std::move(i));
    }

    if (rc != SQLITE_DONE)
    {
        cerr << "Error while iterating instructors: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
    return list;
}

vector<InstructorCourse> DatabaseManager::getInstructorCourses()
{
    vector<InstructorCourse> list;
    const char *sql = "SELECT InstructorID, CourseID FROM InstructorCourses;";
    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        cerr << "Failed to prepare getInstructorCourses statement: " << sqlite3_errmsg(db) << endl;
        return list;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        InstructorCourse ic;
        const unsigned char *t0 = sqlite3_column_text(stmt, 0);
        const unsigned char *t1 = sqlite3_column_text(stmt, 1);
        ic.instructorID = t0 ? reinterpret_cast<const char *>(t0) : string();
        ic.courseID = t1 ? reinterpret_cast<const char *>(t1) : string();
        list.push_back(std::move(ic));
    }

    if (rc != SQLITE_DONE)
    {
        cerr << "Error while iterating instructor-courses: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
    return list;
}

vector<Room> DatabaseManager::getRooms()
{
    vector<Room> list;
    const char *sql = "SELECT RoomID, Building, RoomName, Capacity, RoomType FROM Rooms;";
    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        cerr << "Failed to prepare getRooms statement: " << sqlite3_errmsg(db) << endl;
        return list;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        Room r;
        const unsigned char *t0 = sqlite3_column_text(stmt, 0);
        const unsigned char *t1 = sqlite3_column_text(stmt, 1);
        const unsigned char *t2 = sqlite3_column_text(stmt, 2);
        r.id = t0 ? reinterpret_cast<const char *>(t0) : string();
        r.building = t1 ? reinterpret_cast<const char *>(t1) : string();
        r.roomName = t2 ? reinterpret_cast<const char *>(t2) : string();
        r.capacity = sqlite3_column_int(stmt, 3);
        const unsigned char *t4 = sqlite3_column_text(stmt, 4);
        r.roomType = t4 ? reinterpret_cast<const char *>(t4) : string();
        list.push_back(std::move(r));
    }

    if (rc != SQLITE_DONE)
    {
        cerr << "Error while iterating rooms: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
    return list;
}

vector<TimeSlot> DatabaseManager::getTimeSlots()
{
    vector<TimeSlot> list;
    const char *sql = "SELECT TimeSlotID, Day, StartTimeTxt, EndTimeTxt, StartMin, EndMin FROM TimeSlots;";
    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        cerr << "Failed to prepare getTimeSlots statement: " << sqlite3_errmsg(db) << endl;
        return list;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        TimeSlot t;
        t.id = sqlite3_column_int(stmt, 0);
        const unsigned char *t1 = sqlite3_column_text(stmt, 1);
        const unsigned char *t2 = sqlite3_column_text(stmt, 2);
        const unsigned char *t3 = sqlite3_column_text(stmt, 3);
        t.day = t1 ? reinterpret_cast<const char *>(t1) : string();
        t.startTxt = t2 ? reinterpret_cast<const char *>(t2) : string();
        t.endTxt = t3 ? reinterpret_cast<const char *>(t3) : string();
        t.startMin = sqlite3_column_int(stmt, 4);
        t.endMin = sqlite3_column_int(stmt, 5);
        list.push_back(std::move(t));
    }

    if (rc != SQLITE_DONE)
    {
        cerr << "Error while iterating time slots: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
    return list;
}

vector<Instructor> DatabaseManager::getInstructorsForCourse(const string& courseID) {
    vector<Instructor> result;
    const char *sql = "SELECT I.InstructorID, I.Name, I.Role, I.PreferredSlots, I.QualifiedCourses "
                      "FROM Instructor I "
                      "INNER JOIN InstructorCourses IC ON I.InstructorID = IC.InstructorID "
                      "WHERE IC.CourseID = ?;";
    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare getInstructorsForCourse statement: " << sqlite3_errmsg(db) << endl;
        return result;
    }
    
    sqlite3_bind_text(stmt, 1, courseID.c_str(), -1, SQLITE_STATIC);
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        Instructor i;
        const unsigned char *t0 = sqlite3_column_text(stmt, 0);
        const unsigned char *t1 = sqlite3_column_text(stmt, 1);
        const unsigned char *t2 = sqlite3_column_text(stmt, 2);
        const unsigned char *t3 = sqlite3_column_text(stmt, 3);
        const unsigned char *t4 = sqlite3_column_text(stmt, 4);
        i.id = t0 ? reinterpret_cast<const char *>(t0) : string();
        i.name = t1 ? reinterpret_cast<const char *>(t1) : string();
        i.role = t2 ? reinterpret_cast<const char *>(t2) : string();
        i.preferredSlots = t3 ? reinterpret_cast<const char *>(t3) : string();
        i.qualifiedCourses = t4 ? reinterpret_cast<const char *>(t4) : string();
        result.push_back(std::move(i));
    }
    
    sqlite3_finalize(stmt);
    return result;
}
