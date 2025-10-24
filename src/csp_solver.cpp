#include "../include/csp_solver.h"
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <limits>
#include <functional>
#include <sstream>
#include <iomanip>

using namespace std;
using clk = chrono::high_resolution_clock;

// Convert minutes since midnight to 12-hour format (e.g., 540 -> "09:00AM")
static string minTo12Hour(int mins) {
    int h = mins / 60, m = mins % 60;
    bool pm = (h >= 12);
    int hh = (h % 12 == 0) ? 12 : h % 12;
    ostringstream ss;
    ss << setw(2) << setfill('0') << hh << ":" << setw(2) << m << (pm ? "PM" : "AM");
    return ss.str();
}

CSPSolver::CSPSolver(const vector<Course>& courses_, const vector<Instructor>& instructors_,
                     const vector<InstructorCourse>& instructorCourses_, const vector<Room>& rooms_,
                     const vector<TimeSlot>& timeSlots_)
        : courses(courses_), instructors(instructors_), instructorCourses(instructorCourses_), 
          rooms(rooms_), timeSlots(timeSlots_) {
    
    // Build course index for fast lookup
    for (const auto &c : courses) courseIndex[c.id] = &c;
    
    // Build instructor-to-course mapping
    for (const auto &ic : instructorCourses) {
        courseToInstructors[ic.courseID].push_back(ic.instructorID);
    }
    
    // Fallback: parse QualifiedCourses if mapping is empty
    if (courseToInstructors.empty()) {
        for (const auto &ins : instructors) {
            string q = ins.qualifiedCourses;
            size_t pos = 0;
            while (pos < q.size()) {
                size_t comma = q.find(',', pos);
                string token = (comma == string::npos) ? q.substr(pos) : q.substr(pos, comma - pos);
                auto l = token.find_first_not_of(" \t");
                auto r = token.find_last_not_of(" \t");
                if (l != string::npos && r != string::npos) {
                    courseToInstructors[token.substr(l, r - l + 1)].push_back(ins.id);
                }
                if (comma == string::npos) break;
                pos = comma + 1;
            }
        }
    }
}

void CSPSolver::buildLectureVariables() {
    variables.clear();
    
    const vector<string> year1 = {"LRA401", "CSC111", "MTH111", "PHY113", "ECE111", "LRA101", "LRA104", "LRA105"};
    const vector<string> year2 = {"MTH212", "ACM215", "LRA403", "CSC211", "CNC111", "CSC114", "CSE214", "LRA306"};
    const vector<string> year3 = {"AID311", "AID312", "BIF311", "CNC311", "CNC312", "CNC314", "CSC314", "CSC317", "ECE324"};
    const vector<string> japaneseLanguages = {"LRA401", "LRA403"};
    const vector<string> specializations = {"AID", "BIF", "CSC", "CNC"};
    
    auto isInList = [](const vector<string>& list, const string& id) {
        return find(list.begin(), list.end(), id) != list.end();
    };

    for (const auto &c : courses) {
        int yr = c.year;
        if (yr < 1 || yr > 4) continue;
        
        // Check if Year 1-3 course is in known lists
        if (yr == 1 && !isInList(year1, c.id)) continue;
        if (yr == 2 && !isInList(year2, c.id)) continue;
        if (yr == 3 && !isInList(year3, c.id)) continue;

        bool isJapanese = isInList(japaneseLanguages, c.id);
        
        // Years 3-4: Specialization-based scheduling
        if (yr == 3 || yr == 4) {
            // Check database specialization field (not just course prefix)
            bool isCommon = (c.specialization == "Common" || c.specialization.empty());
            
            if (isCommon) {
                // Common course: create variable for each specialization
                for (const auto& s : specializations) {
                    LectureVar v;
                    v.courseID = c.id;
                    v.year = yr;
                    v.groupId = 0;
                    v.specialization = s;
                    v.lengthMin = 90;
                    v.sessionType = "LECTURE";
                    v.sectionId = 0;
                    v.isFullDay = false;
                    v.varID = c.id + "_Y" + to_string(yr) + "_" + s + "_LEC";
                    variables.push_back(v);
                }
            } else {
                // Specialization-specific course (use database specialization field)
                LectureVar v;
                v.courseID = c.id;
                v.year = yr;
                v.groupId = 0;
                v.specialization = c.specialization;
                v.lengthMin = 90;
                v.sessionType = c.isGradProject ? "LAB" : "LECTURE";
                v.sectionId = 0;
                v.isFullDay = c.isGradProject;
                v.varID = c.id + "_Y" + to_string(yr) + "_" + c.specialization + (c.isGradProject ? "_GRADPROJ" : "_LEC");
                variables.push_back(v);
            }
        } 
        // Japanese courses: 3 groups × 3 sections
        else if (isJapanese) {
            for (int grp = 1; grp <= 3; grp++) {
                for (int sec = 1; sec <= 3; sec++) {
                    LectureVar v;
                    v.courseID = c.id;
                    v.year = yr;
                    v.groupId = grp;
                    v.lengthMin = 90;
                    v.sessionType = "LECTURE";
                    v.sectionId = sec;
                    v.isFullDay = false;
                    v.varID = c.id + "_Y" + to_string(yr) + "_G" + to_string(grp) + "_S" + to_string(sec);
                    variables.push_back(v);
                }
            }
        } 
        // Years 1-2: Group-based scheduling (3 groups)
        else {
            for (int grp = 1; grp <= 3; grp++) {
                LectureVar v;
                v.courseID = c.id;
                v.year = yr;
                v.groupId = grp;
                v.lengthMin = 90;
                v.sessionType = "LECTURE";
                v.sectionId = 0;
                v.isFullDay = false;
                v.varID = c.id + "_Y" + to_string(yr) + "_G" + to_string(grp) + "_LEC";
                variables.push_back(v);
            }
        }
    }
}

void CSPSolver::buildDomains() {
    domains.clear();
    domains.resize(variables.size());
    
    unordered_map<string, const Room*> roomIndex;
    for (const auto &r : rooms) roomIndex[r.id] = &r;
    
    unordered_map<string, const Instructor*> instructorIndex;
    for (const auto &ins : instructors) instructorIndex[ins.id] = &ins;
    
    for (size_t vi = 0; vi < variables.size(); vi++) {
        const auto &v = variables[vi];
        const Course* course = courseIndex.count(v.courseID) ? courseIndex[v.courseID] : nullptr;
        if (!course) continue;

        vector<string> qualifiedInstructors;
        
        if (v.sessionType == "LECTURE") {
            // Get professors qualified for this course
            if (courseToInstructors.count(course->id)) {
                for (const auto &insID : courseToInstructors[course->id]) {
                    if (instructorIndex.count(insID) && instructorIndex[insID]->role == "Professor") {
                        qualifiedInstructors.push_back(insID);
                    }
                }
            }
            if (qualifiedInstructors.empty()) {
                for (const auto &ins : instructors) {
                    if (ins.role == "Professor") qualifiedInstructors.push_back(ins.id);
                }
            }
            
            // Generate domain: all valid (timeslot, room, instructor) combinations
            for (size_t tsIdx = 0; tsIdx < timeSlots.size(); tsIdx++) {
                const TimeSlot &ts = timeSlots[tsIdx];
                if ((ts.endMin - ts.startMin) < v.lengthMin) continue;
                
                for (const auto &r : rooms) {
                    if (r.roomType != "Classroom" && r.roomType != "Theater" && r.roomType != "Hall") continue;
                    for (const auto &insID : qualifiedInstructors) {
                        domains[vi].push_back({(int)tsIdx, r.id, insID});
                    }
                }
            }
        } 
        else if (v.sessionType == "LAB") {
            // Get assistant professors qualified for this course
            if (courseToInstructors.count(course->id)) {
                for (const auto &insID : courseToInstructors[course->id]) {
                    if (instructorIndex.count(insID) && instructorIndex[insID]->role == "Assistant Professor") {
                        qualifiedInstructors.push_back(insID);
                    }
                }
            }
            if (qualifiedInstructors.empty()) {
                for (const auto &ins : instructors) {
                    if (ins.role == "Assistant Professor") qualifiedInstructors.push_back(ins.id);
                }
            }
            
            for (size_t tsIdx = 0; tsIdx < timeSlots.size(); tsIdx++) {
                const TimeSlot &ts = timeSlots[tsIdx];
                if ((ts.endMin - ts.startMin) < v.lengthMin) continue;
                
                // Graduation projects: only allow first slot of each day (0, 4, 8, 12, 16)
                if (v.isFullDay && tsIdx % 4 != 0) continue;
                
                for (const auto &r : rooms) {
                    bool roomOk = false;
                    if (v.courseID == "ECE111") roomOk = (r.id == "B07-F0");
                    else if (v.courseID == "PHY113") roomOk = (r.id == "COE-F21" || r.id == "COE-F22" || r.id == "COE-F11");
                    else roomOk = (r.roomType == "Lab");
                    
                    if (!roomOk) continue;
                    
                    for (const auto &insID : qualifiedInstructors) {
                        domains[vi].push_back({(int)tsIdx, r.id, insID});
                    }
                }
            }
        }
    }
}

bool CSPSolver::isHardConflict(const AssignmentValue& a, const AssignmentValue& b,
                                const LectureVar& va, const LectureVar& vb) const {
    const TimeSlot& tsA = timeSlots[a.timeslotIndex];
    const TimeSlot& tsB = timeSlots[b.timeslotIndex];
    
    if (tsA.day != tsB.day) return false;
    
    // Full-day projects conflict with anything on the same day
    bool timeOverlap = (va.isFullDay || vb.isFullDay) ? true : 
                       !(tsA.endMin <= tsB.startMin || tsB.endMin <= tsA.startMin);
    if (!timeOverlap) return false;
    
    // Same instructor or room
    if (a.instructorID == b.instructorID || a.roomID == b.roomID) return true;
    
    // Same year and group (Years 1-2)
    if (va.groupId > 0 && vb.groupId > 0 && va.year == vb.year && va.groupId == vb.groupId) return true;
    
    // Same year and specialization (Years 3-4)
    if (!va.specialization.empty() && !vb.specialization.empty() && 
        va.year == vb.year && va.specialization == vb.specialization) return true;
    
    // Same course lectures must have same instructor
    if (va.courseID == vb.courseID && va.sessionType == "LECTURE" && 
        vb.sessionType == "LECTURE" && a.instructorID != b.instructorID) return true;
    
    return false;
}

int CSPSolver::computeSoftCost(const unordered_map<string, AssignmentValue>& assignments) const {
    int cost = 0;
    if (timeSlots.empty()) return cost;
    
    int earliestStartMin = numeric_limits<int>::max();
    for (const auto &ts : timeSlots) earliestStartMin = min(earliestStartMin, ts.startMin);
    
    for (auto &p : assignments) {
        const TimeSlot &ts = timeSlots[p.second.timeslotIndex];
        if (ts.startMin == earliestStartMin) cost += 5;
    }
    
    unordered_map<string, unordered_map<string, int>> courseDayCount;
    for (auto &p : assignments) {
        size_t pos = p.first.find("_Y");
        string courseID = (pos != string::npos) ? p.first.substr(0, pos) : p.first;
        const TimeSlot &ts = timeSlots[p.second.timeslotIndex];
        courseDayCount[courseID][ts.day] += 1;
    }
    
    for (auto &cd : courseDayCount) {
        for (auto &d : cd.second) {
            if (d.second > 1) cost += (d.second - 1) * 2;
        }
    }
    
    return cost;
}

CSPResult CSPSolver::backtrackSearch() {
    cout << "Starting backtrack search (MRV + Forward Checking)\n   -> This may take a little bit ...\n";
    auto start = clk::now();
    
    CSPResult result;
    result.success = false;
    result.hardViolations = 0;
    result.softCost = 0;
    
    // Check for empty domains
    for (size_t i = 0; i < variables.size(); i++) {
        if (domains[i].empty()) {
            cout << "Variable " << variables[i].varID << " has empty domain\n";
            result.hardViolations = 1;
            result.solveSeconds = 0.0;
            return result;
        }
    }
    
    vector<vector<AssignmentValue>> doms = domains;
    unordered_map<string, AssignmentValue> assignments;
    unordered_map<string, string> courseProfessor; // Ensures same course lectures have same professor
    
    function<bool()> dfs = [&]() -> bool {
        // Base case: all variables assigned
        if (assignments.size() == variables.size()) {
            result.success = true;
            result.assignments = assignments;
            result.hardViolations = 0;
            result.softCost = computeSoftCost(assignments);
            return true;
        }
        
        // MRV: choose variable with smallest domain
        int chosen = -1;
        size_t minDomainSize = numeric_limits<size_t>::max();
        for (size_t i = 0; i < variables.size(); i++) {
            if (assignments.count(variables[i].varID)) continue;
            if (doms[i].size() < minDomainSize) {
                minDomainSize = doms[i].size();
                chosen = i;
            }
        }
        if (chosen == -1) return false;
        
        auto domainCopy = doms[chosen];
        for (const auto &val : domainCopy) {
            const LectureVar& chosenVar = variables[chosen];
            
            // Consistency check: same course lectures must have same professor
            if (chosenVar.sessionType == "LECTURE" && courseProfessor.count(chosenVar.courseID)) {
                if (courseProfessor[chosenVar.courseID] != val.instructorID) continue;
            }
            
            // Check conflicts with existing assignments
            bool conflict = false;
            for (const auto &as : assignments) {
                auto itVar = find_if(variables.begin(), variables.end(),
                                     [&](const LectureVar& lv) { return lv.varID == as.first; });
                if (itVar == variables.end()) continue;
                int otherIdx = distance(variables.begin(), itVar);
                if (isHardConflict(val, as.second, chosenVar, variables[otherIdx])) {
                    conflict = true;
                    break;
                }
            }
            if (conflict) continue;
            
            // Make assignment
            assignments[chosenVar.varID] = val;
            if (chosenVar.sessionType == "LECTURE") courseProfessor[chosenVar.courseID] = val.instructorID;
            
            // Forward checking: prune domains
            vector<pair<int, vector<AssignmentValue>>> changed;
            for (size_t j = 0; j < doms.size(); j++) {
                if (assignments.count(variables[j].varID)) continue;
                
                vector<AssignmentValue> newDom;
                for (auto &cand : doms[j]) {
                    bool valid = true;
                    
                    if (isHardConflict(val, cand, chosenVar, variables[j])) valid = false;
                    
                    if (valid && variables[j].sessionType == "LECTURE" && courseProfessor.count(variables[j].courseID)) {
                        if (courseProfessor[variables[j].courseID] != cand.instructorID) valid = false;
                    }
                    
                    if (valid) newDom.push_back(cand);
                }
                
                if (newDom.size() != doms[j].size()) {
                    changed.push_back({j, doms[j]});
                    doms[j] = newDom;
                }
            }
            
            // Check if any domain became empty (dead end)
            bool anyEmpty = false;
            for (size_t j = 0; j < doms.size(); j++) {
                if (!assignments.count(variables[j].varID) && doms[j].empty()) {
                    anyEmpty = true;
                    break;
                }
            }
            
            if (!anyEmpty && dfs()) return true;
            
            // Backtrack: undo assignment and restore domains
            for (auto &p : changed) doms[p.first] = move(p.second);
            assignments.erase(chosenVar.varID);
            
            if (chosenVar.sessionType == "LECTURE") {
                bool otherGroupAssigned = false;
                for (const auto &as : assignments) {
                    auto itVar = find_if(variables.begin(), variables.end(),
                                         [&](const LectureVar& lv) { return lv.varID == as.first; });
                    if (itVar != variables.end() && itVar->courseID == chosenVar.courseID && 
                        itVar->sessionType == "LECTURE") {
                        otherGroupAssigned = true;
                        break;
                    }
                }
                if (!otherGroupAssigned) courseProfessor.erase(chosenVar.courseID);
            }
        }
        
        return false;
    };
    
    bool found = dfs();
    auto end = clk::now();
    result.solveSeconds = chrono::duration<double>(end - start).count();
    
    if (!found) {
        result.success = false;
        cout << "No solution found after " << result.solveSeconds << " seconds\n";
    }
    
    return result;
}

CSPResult CSPSolver::solve(int maxSolutions) {
    return backtrackSearch();
}

void CSPSolver::printResult(const CSPResult& r, const vector<LectureVar>& vars,
                            const vector<TimeSlot>& timeSlots, const vector<Room>& rooms) {
    if (!r.success) {
        cout << "\nNo solution found. Hard violations: " << r.hardViolations 
             << ", time: " << r.solveSeconds << "s\n";
        return;
    }
    
    unordered_map<string, const Room*> roomIndex;
    for (const auto &rm : rooms) roomIndex[rm.id] = &rm;
    
    unordered_map<string, string> instructorNames, courseNames;
    for (const auto &ins : instructors) instructorNames[ins.id] = ins.name;
    for (const auto &c : courses) courseNames[c.id] = c.name;
    
    cout << "\nSolution found in " << r.solveSeconds << "s\n=========================================\n";

    for (const auto &v : vars) {
        auto it = r.assignments.find(v.varID);
        if (it == r.assignments.end()) continue;
        
        const AssignmentValue &a = it->second;
        const TimeSlot &ts = timeSlots[a.timeslotIndex];
        const Room* rm = roomIndex.count(a.roomID) ? roomIndex[a.roomID] : nullptr;
        
        string cname = courseNames.count(v.courseID) ? courseNames[v.courseID] : v.courseID;
        string insName = instructorNames.count(a.instructorID) ? instructorNames[a.instructorID] : a.instructorID;
        
        cout << v.courseID << " | " << cname << " (Y" << v.year << ")";
        
        if (v.sessionType == "LECTURE") {
            if ((v.year == 3 || v.year == 4) && !v.specialization.empty()) {
                cout << " | " << v.specialization << " Lecture";
            } else if (v.sectionId > 0) {
                cout << " | G" << v.groupId << " Section " << v.sectionId;
            } else {
                cout << " | G" << v.groupId << " Lecture";
            }
        } else if (v.sessionType == "LAB") {
            cout << " | " << (v.isFullDay ? v.specialization + " Graduation Project (FULL DAY)" : "Lab S" + to_string(v.sectionId));
        }
        
        if (v.isFullDay) {
            cout << "\n  " << ts.day << " 9:00 AM - 3:45 PM (Full Day)";
        } else {
            cout << "\n  " << ts.day << " " << minTo12Hour(ts.startMin) << " - " << minTo12Hour(ts.endMin);
        }
        cout << " | " << (rm ? rm->roomName : a.roomID) << " (" << (rm ? rm->building : "") << ")"
             << " | " << insName << "\n\n";
    }
    
    cout << "=========================================\n";
}
