#include <iostream>
#include "../include/database_manager.h"
#include "../include/csp_solver.h"
using namespace std;

int main() {
    try {
        DatabaseManager db("../data/database.db");

        auto courses = db.getCourses();
        auto instructors = db.getInstructors();
        auto instructorCourses = db.getInstructorCourses();
        auto rooms = db.getRooms();
        auto timeSlots = db.getTimeSlots();

        cout << "Timetable Generation is Connecting ... (I hope)\n";
        cout << "=========================================\n";
        cout << "Loaded Data:\n" << courses.size() << " courses, " << instructors.size()
             << " instructors, " << rooms.size() << " rooms, " << timeSlots.size() << " time slots\n\n";

        vector<Course> filteredCourses;
        for (const auto& c : courses) {
            if (c.year == 1 || c.year == 2) filteredCourses.push_back(c);
        }
        
        cout << "Scheduling " << filteredCourses.size() << " courses (Year 1 and 2)\n";

        CSPSolver solver(filteredCourses, instructors, instructorCourses, rooms, timeSlots);
        solver.buildLectureVariables();
        solver.buildDomains();
        
        cout << "\n";
        CSPResult result = solver.solve();
        
        if (result.success) {
            solver.printResult(result, solver.getVariables(), timeSlots, rooms);
            cout << "\nSUCCESS | Hard violations: "
                 << result.hardViolations << "\n";
        } else {
            cout << "\nFAILED: No valid schedule found\n";
            cout << "Possible causes: insufficient resources, conflicting constraints\n\n";
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
