#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include "../include/database_manager.h"
#include "../include/csp_solver.h"
using namespace std;

string escapeJson(const string& s) {
    ostringstream o;
    for (char c : s) {
        switch (c) {
            case '"': o << "\\\""; break;
            case '\\': o << "\\\\"; break;
            case '\b': o << "\\b"; break;
            case '\f': o << "\\f"; break;
            case '\n': o << "\\n"; break;
            case '\r': o << "\\r"; break;
            case '\t': o << "\\t"; break;
            default:
                if ('\x00' <= c && c <= '\x1f') {
                    o << "\\u" << hex << setw(4) << setfill('0') << static_cast<int>(c);
                } else {
                    o << c;
                }
        }
    }
    return o.str();
}



int main() {
    try {
        DatabaseManager db("../data/database.db");

        auto courses = db.getCourses();
        auto instructors = db.getInstructors();
        auto instructorCourses = db.getInstructorCourses();
        auto rooms = db.getRooms();
        auto timeSlots = db.getTimeSlots();

        cout << "Timetable Generation CSP Solver\n=========================================\n";
        cout << "Loaded:\n" << courses.size() << " courses, " << instructors.size()
             << " instructors, " << rooms.size() << " rooms, " << timeSlots.size() << " time slots\n\n";

        vector<Course> filteredCourses;
        for (const auto& c : courses) {
            if (c.year >= 1 && c.year <= 4) filteredCourses.push_back(c);
        }

        cout << "Scheduling " << filteredCourses.size() << " courses (Years 1 - 4)\n";

        CSPSolver solver(filteredCourses, instructors, instructorCourses, rooms, timeSlots);
        solver.buildLectureVariables();
        solver.buildDomains();

        cout << "\n";
        CSPResult result = solver.solve();

        if (result.success) {
            solver.printResult(result, solver.getVariables(), timeSlots, rooms);
            
            ofstream jsonFile("../data/timetable.json");
            if (jsonFile.is_open()) {
                unordered_map<string, const Room*> roomIndex;
                for (const auto &rm : rooms) roomIndex[rm.id] = &rm;
                
                unordered_map<string, string> instructorNames, courseNames;
                for (const auto &ins : instructors) instructorNames[ins.id] = ins.name;
                for (const auto &c : courses) courseNames[c.id] = c.name;
                
                jsonFile << "{\n";
                jsonFile << "  \"success\": true,\n";
                jsonFile << "  \"stats\": {\n";
                jsonFile << "    \"totalCourses\": " << filteredCourses.size() << ",\n";
                jsonFile << "    \"totalSessions\": " << solver.getVariables().size() << ",\n";
                jsonFile << "    \"violations\": " << result.hardViolations << ",\n";
                jsonFile << "    \"solveTime\": " << result.solveSeconds << "\n";
                jsonFile << "  },\n";
                jsonFile << "  \"schedule\": {\n";
                
                map<int, map<string, vector<string>>> organized;
                
                for (const auto &v : solver.getVariables()) {
                    auto it = result.assignments.find(v.varID);
                    if (it == result.assignments.end()) continue;
                    
                    const AssignmentValue &a = it->second;
                    const TimeSlot &ts = timeSlots[a.timeslotIndex];
                    const Room* rm = roomIndex.count(a.roomID) ? roomIndex[a.roomID] : nullptr;
                    
                    string cname = courseNames.count(v.courseID) ? courseNames[v.courseID] : v.courseID;
                    string insName = a.instructorID.empty() ? "null" : 
                                    (instructorNames.count(a.instructorID) ? instructorNames[a.instructorID] : a.instructorID);
                    
                    string groupKey;
                    if (v.year <= 2) {
                        groupKey = "G" + to_string(v.groupId);
                    } else {
                        groupKey = v.specialization.empty() ? "G1" : v.specialization;
                    }
                    
                    string sessionType;
                    if (v.sessionType == "LECTURE") {
                        if ((v.year == 3 || v.year == 4) && !v.specialization.empty()) {
                            sessionType = v.specialization + " Lecture";
                        } else if (v.sectionId > 0 && v.groupId > 0) {
                            sessionType = "G" + to_string(v.groupId) + " Section " + to_string(v.sectionId);
                        } else {
                            sessionType = "G" + to_string(v.groupId) + " Lecture";
                        }
                    } else if (v.sessionType == "LAB") {
                        if (!v.specialization.empty() && v.sectionId > 0) {
                            sessionType = v.specialization + " Lab";
                        } else if (v.groupId > 0 && v.sectionId > 0) {
                            sessionType = "G" + to_string(v.groupId) + " S" + to_string(v.sectionId) + " Lab";
                        } else {
                            sessionType = "Lab";
                        }
                        if (v.isFullDay) sessionType += " (Full Day)";
                    }
                    
                    ostringstream sessionJson;
                    sessionJson << "      {\n";
                    sessionJson << "        \"code\": \"" << escapeJson(v.courseID) << "\",\n";
                    sessionJson << "        \"name\": \"" << escapeJson(cname) << "\",\n";
                    sessionJson << "        \"type\": \"" << escapeJson(sessionType) << "\",\n";
                    sessionJson << "        \"day\": \"" << escapeJson(ts.day) << "\",\n";
                    if (v.isFullDay) {
                        sessionJson << "        \"time\": \"09:00AM - 03:45PM\",\n";
                        sessionJson << "        \"startTime\": \"09:00AM\",\n";
                        sessionJson << "        \"endTime\": \"03:45PM\",\n";
                    } else {
                        string startTime = minTo12Hour(ts.startMin);
                        string endTime = minTo12Hour(ts.endMin);
                        sessionJson << "        \"time\": \"" << startTime << " - " << endTime << "\",\n";
                        sessionJson << "        \"startTime\": \"" << startTime << "\",\n";
                        sessionJson << "        \"endTime\": \"" << endTime << "\",\n";
                    }
                    sessionJson << "        \"instructor\": \"" << escapeJson(insName) << "\",\n";
                    sessionJson << "        \"room\": \"" << (rm ? escapeJson(rm->roomName) : escapeJson(a.roomID)) 
                                << " (" << (rm ? escapeJson(rm->building) : "") << ")\"\n";
                    sessionJson << "      }";
                    
                    organized[v.year][groupKey].push_back(sessionJson.str());
                }
                
                bool firstYear = true;
                for (const auto& [year, groups] : organized) {
                    if (!firstYear) jsonFile << ",\n";
                    firstYear = false;
                    
                    jsonFile << "    \"" << year << "\": {\n";
                    bool firstGroup = true;
                    for (const auto& [group, sessions] : groups) {
                        if (!firstGroup) jsonFile << ",\n";
                        firstGroup = false;
                        
                        jsonFile << "      \"" << group << "\": [\n";
                        for (size_t i = 0; i < sessions.size(); i++) {
                            jsonFile << sessions[i];
                            if (i < sessions.size() - 1) jsonFile << ",\n";
                            else jsonFile << "\n";
                        }
                        jsonFile << "      ]";
                    }
                    jsonFile << "\n    }";
                }
                
                jsonFile << "\n  }\n";
                jsonFile << "}\n";
                jsonFile.close();

                cout << "\nJSON output written to ../data/timetable.json\n";
            }
            
            cout << "\nSUCCESS | Hard violations: " << result.hardViolations << "\n";
        } else {
            cout << "\nFAILED | No valid schedule found\n";
            
            ofstream jsonFile("../data/timetable.json");
            if (jsonFile.is_open()) {
                jsonFile << "{\n";
                jsonFile << "  \"success\": false,\n";
                jsonFile << "  \"error\": \"No valid schedule found\"\n";
                jsonFile << "}\n";
                jsonFile.close();
            }
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        
        ofstream jsonFile("../data/timetable.json");
        if (jsonFile.is_open()) {
            jsonFile << "{\n";
            jsonFile << "  \"success\": false,\n";
            jsonFile << "  \"error\": \"" << escapeJson(e.what()) << "\"\n";
            jsonFile << "}\n";
            jsonFile.close();
        }
        
        return 1;
    }

    return 0;
}
