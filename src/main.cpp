#include <iostream>
#include "../include/database_manager.h"
#include "../include/csp_solver.h"
using namespace std;

int main() {
    try {
        // Initialize and connect to the SQLite database
        DatabaseManager db("F:\\TestSql\\data\\database.db");

        // Load data from database
        auto courses     = db.getCourses();
        auto instructors = db.getInstructors();
        auto rooms       = db.getRooms();
        auto timeSlots   = db.getTimeSlots();

        // Create and run the CSP solver
        CSPSolver solver(courses, instructors, rooms, timeSlots);

        if (solver.solve()) {
            solver.printSolution();
            cout.flush();
        } else {
            cout << "No valid timetable could be generated.\n";
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    cout << "Program finished successfully.\n";
    return 0;
}