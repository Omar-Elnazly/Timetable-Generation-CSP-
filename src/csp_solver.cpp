#include "../include/csp_solver.h"
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <limits>
#include <functional>
#include <numeric>
#include <sstream>
#include <iomanip>

using namespace std;
using clk = std::chrono::high_resolution_clock;

static string minTo12Hour(int mins) {
    int h = mins / 60;
    int m = mins % 60;
    bool pm = (h >= 12);
    int hh = h % 12;
    if (hh == 0) hh = 12;
    ostringstream ss;
    ss << setw(2) << setfill('0') << hh << ":" << setw(2) << setfill('0') << m << (pm ? "PM" : "AM");
    return ss.str();
}

CSPSolver::CSPSolver(const vector<Course>& courses_, const vector<Instructor>& instructors_,
                     const vector<InstructorCourse>& instructorCourses_, const vector<Room>& rooms_,
                     const vector<TimeSlot>& timeSlots_)
        : courses(courses_), instructors(instructors_), instructorCourses(instructorCourses_), 
          rooms(rooms_), timeSlots(timeSlots_) {
    for (const auto &c : courses) courseIndex[c.id] = &c;
    for (const auto &ic : instructorCourses) courseToInstructors[ic.courseID].push_back(ic.instructorID);
    
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
    const vector<string> year3 = {"AID311", "AID312", "BIF311", "CNC311", "CNC312", "CNC314", "CSC317", "ECE324"};
    const vector<string> japaneseLanguages = {"LRA401", "LRA403"};
    const vector<string> specializations = {"AID", "BIF", "CSC", "CNC"};
    
    auto isInList = [](const vector<string>& list, const string& id) {
        return find(list.begin(), list.end(), id) != list.end();
    };
    
    auto getSpecialization = [](const string& courseID) -> string {
        if (courseID.substr(0, 3) == "AID") return "AID";
        if (courseID.substr(0, 3) == "BIF") return "BIF";
        if (courseID.substr(0, 3) == "CSC") return "CSC";
        if (courseID.substr(0, 3) == "CNC") return "CNC";
        return "";
    };

    for (const auto &c : courses) {
        int courseYear = 0;
        if (isInList(year1, c.id)) courseYear = 1;
        else if (isInList(year2, c.id)) courseYear = 2;
        else if (isInList(year3, c.id)) courseYear = 3;
        else continue;

        bool isJapanese = isInList(japaneseLanguages, c.id);
        
        if (courseYear == 3) {
            string spec = getSpecialization(c.id);
            if (spec.empty()) {
                for (const auto& s : specializations) {
                    LectureVar vLec;
                    vLec.courseID = c.id;
                    vLec.year = courseYear;
                    vLec.groupId = 0;
                    vLec.specialization = s;
                    vLec.lengthMin = 90;
                    vLec.sessionType = "LECTURE";
                    vLec.sectionId = 0;
                    vLec.varID = c.id + "_Y3_" + s + "_LEC";
                    variables.push_back(move(vLec));
                }
            } else {
                LectureVar vLec;
                vLec.courseID = c.id;
                vLec.year = courseYear;
                vLec.groupId = 0;
                vLec.specialization = spec;
                vLec.lengthMin = 90;
                vLec.sessionType = "LECTURE";
                vLec.sectionId = 0;
                vLec.varID = c.id + "_Y3_" + spec + "_LEC";
                variables.push_back(move(vLec));
            }
        } else if (isJapanese) {
            for (int grp = 1; grp <= 3; grp++) {
                for (int section = 1; section <= 3; ++section) {
                    LectureVar v;
                    v.courseID = c.id;
                    v.year = courseYear;
                    v.groupId = grp;
                    v.lengthMin = 90;
                    v.sessionType = "LECTURE";
                    v.sectionId = section;
                    v.varID = c.id + "_Y" + to_string(courseYear) + "_G" + to_string(grp) + "_S" + to_string(section);
                    variables.push_back(move(v));
                }
            }
        } else {
            for (int grp = 1; grp <= 3; grp++) {
                LectureVar vLec;
                vLec.courseID = c.id;
                vLec.year = courseYear;
                vLec.groupId = grp;
                vLec.lengthMin = 90;
                vLec.sessionType = "LECTURE";
                vLec.sectionId = 0;
                vLec.varID = c.id + "_Y" + to_string(courseYear) + "_G" + to_string(grp) + "_LEC";
                variables.push_back(move(vLec));
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
    
    for (size_t vi = 0; vi < variables.size(); ++vi) {
        const auto &v = variables[vi];
        const Course* course = nullptr;
        auto it = courseIndex.find(v.courseID);
        if (it != courseIndex.end()) course = it->second;
        if (!course) continue;

        vector<string> qualifiedInstructors;
        
        if (v.sessionType == "LECTURE") {
            auto insIt = courseToInstructors.find(course->id);
            vector<string> qualified;
            if (insIt != courseToInstructors.end()) qualified = insIt->second;
            
            for (const auto &insID : qualified) {
                auto insPtr = instructorIndex.find(insID);
                if (insPtr != instructorIndex.end() && insPtr->second->role == "Professor") {
                    qualifiedInstructors.push_back(insID);
                }
            }
            
            if (qualifiedInstructors.empty()) {
                for (const auto &ins : instructors) {
                    if (ins.role == "Professor") qualifiedInstructors.push_back(ins.id);
                }
            }
            
            for (size_t tsIdx = 0; tsIdx < timeSlots.size(); ++tsIdx) {
                const TimeSlot &ts = timeSlots[tsIdx];
                if ((ts.endMin - ts.startMin) < v.lengthMin) continue;
                
                for (const auto &r : rooms) {
                    if (r.roomType != "Classroom" && r.roomType != "Theater" && r.roomType != "Hall") continue;
                    for (const auto &insID : qualifiedInstructors) {
                        AssignmentValue av;
                        av.timeslotIndex = (int)tsIdx;
                        av.roomID = r.id;
                        av.instructorID = insID;
                        domains[vi].push_back(av);
                    }
                }
            }
        } else if (v.sessionType == "LAB") {
            auto insIt = courseToInstructors.find(course->id);
            vector<string> qualified;
            if (insIt != courseToInstructors.end()) qualified = insIt->second;
            
            for (const auto &insID : qualified) {
                auto insPtr = instructorIndex.find(insID);
                if (insPtr != instructorIndex.end() && insPtr->second->role == "Assistant Professor") {
                    qualifiedInstructors.push_back(insID);
                }
            }
            
            if (qualifiedInstructors.empty()) {
                for (const auto &ins : instructors) {
                    if (ins.role == "Assistant Professor") qualifiedInstructors.push_back(ins.id);
                }
            }
            
            for (size_t tsIdx = 0; tsIdx < timeSlots.size(); ++tsIdx) {
                const TimeSlot &ts = timeSlots[tsIdx];
                if ((ts.endMin - ts.startMin) < v.lengthMin) continue;
                
                for (const auto &r : rooms) {
                    bool roomOk = false;
                    if (v.courseID == "ECE111") roomOk = (r.id == "B07-F0");
                    else if (v.courseID == "PHY113") roomOk = (r.id == "COE-F21" || r.id == "COE-F22" || r.id == "COE-F11");
                    else roomOk = (r.roomType == "Lab");
                    
                    if (!roomOk) continue;
                    
                    for (const auto &insID : qualifiedInstructors) {
                        AssignmentValue av;
                        av.timeslotIndex = (int)tsIdx;
                        av.roomID = r.id;
                        av.instructorID = insID;
                        domains[vi].push_back(av);
                    }
                }
            }
        }
    }
    
    int totalDomain = 0;
    for (const auto &d : domains) totalDomain += d.size();
}

bool CSPSolver::isHardConflict(const AssignmentValue& a, const AssignmentValue& b,
                                const LectureVar& va, const LectureVar& vb) const {
    const TimeSlot& tsA = timeSlots[a.timeslotIndex];
    const TimeSlot& tsB = timeSlots[b.timeslotIndex];
    
    if (tsA.day != tsB.day) return false;
    
    bool timeOverlap = !(tsA.endMin <= tsB.startMin || tsB.endMin <= tsA.startMin);
    if (!timeOverlap) return false;
    
    if (a.instructorID == b.instructorID) return true;
    if (a.roomID == b.roomID) return true;
    
    if (va.groupId > 0 && vb.groupId > 0) {
        if (va.year == vb.year && va.groupId == vb.groupId) return true;
    }
    
    if (!va.specialization.empty() && !vb.specialization.empty()) {
        if (va.year == vb.year && va.specialization == vb.specialization) return true;
    }
    
    if (va.courseID == vb.courseID && va.sessionType == "LECTURE" && vb.sessionType == "LECTURE" &&
        a.instructorID != b.instructorID) {
        return true;
    }
    
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
    cout << "Starting backtrack search (MRV + Forward Checking)\n";
    cout << "   -> This may take a little bit ..." << endl;
    auto start = clk::now();
    CSPResult result;
    result.success = false;
    result.hardViolations = 0;
    result.softCost = 0;
    
    for (size_t i = 0; i < variables.size(); ++i) {
        if (domains[i].empty()) {
            cout << "Variable " << variables[i].varID << " has empty domain\n";
            result.hardViolations = 1;
            result.solveSeconds = 0.0;
            return result;
        }
    }
    
    vector<vector<AssignmentValue>> doms = domains;
    unordered_map<string, AssignmentValue> assignments;
    unordered_map<string, string> courseProfessor;
    
    function<bool()> dfs = [&]() -> bool {
        if (assignments.size() == variables.size()) {
            result.success = true;
            result.assignments = assignments;
            result.hardViolations = 0;
            result.softCost = computeSoftCost(assignments);
            return true;
        }
        
        int chosen = -1;
        size_t bestSize = numeric_limits<size_t>::max();
        for (size_t i = 0; i < variables.size(); ++i) {
            if (assignments.find(variables[i].varID) != assignments.end()) continue;
            if (doms[i].size() < bestSize) {
                bestSize = doms[i].size();
                chosen = (int)i;
            }
        }
        if (chosen == -1) return false;
        
        auto domainCopy = doms[chosen];
        for (const auto &val : domainCopy) {
            const LectureVar& chosenVar = variables[chosen];
            
            if (chosenVar.sessionType == "LECTURE") {
                auto profIt = courseProfessor.find(chosenVar.courseID);
                if (profIt != courseProfessor.end() && profIt->second != val.instructorID) continue;
            }
            
            bool conflict = false;
            for (const auto &as : assignments) {
                auto itVar = find_if(variables.begin(), variables.end(),
                                     [&](const LectureVar& lv) { return lv.varID == as.first; });
                if (itVar == variables.end()) continue;
                int otherIndex = (int)distance(variables.begin(), itVar);
                if (isHardConflict(val, as.second, variables[chosen], variables[otherIndex])) {
                    conflict = true;
                    break;
                }
            }
            if (conflict) continue;
            
            assignments[variables[chosen].varID] = val;
            if (chosenVar.sessionType == "LECTURE") courseProfessor[chosenVar.courseID] = val.instructorID;
            
            vector<pair<int, vector<AssignmentValue>>> changed;
            for (size_t j = 0; j < doms.size(); ++j) {
                if (assignments.find(variables[j].varID) != assignments.end()) continue;
                
                vector<AssignmentValue> newdom;
                for (auto &cand : doms[j]) {
                    bool filterOut = false;
                    
                    if (isHardConflict(val, cand, variables[chosen], variables[j])) {
                        filterOut = true;
                    }
                    
                    if (!filterOut && variables[j].sessionType == "LECTURE") {
                        auto profIt = courseProfessor.find(variables[j].courseID);
                        if (profIt != courseProfessor.end() && profIt->second != cand.instructorID) {
                            filterOut = true;
                        }
                    }
                    
                    if (!filterOut) newdom.push_back(cand);
                }
                
                if (newdom.size() < doms[j].size()) {
                    changed.emplace_back((int)j, doms[j]);
                    doms[j] = move(newdom);
                }
            }
            
            bool anyEmpty = false;
            for (size_t j = 0; j < doms.size(); ++j) {
                if (assignments.find(variables[j].varID) == assignments.end() && doms[j].empty()) {
                    anyEmpty = true;
                    break;
                }
            }
            
            if (!anyEmpty && dfs()) return true;
            
            for (auto &p : changed) doms[p.first] = move(p.second);
            assignments.erase(variables[chosen].varID);
            
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
    
    unordered_map<string, string> instructorNames;
    for (const auto &ins : instructors) instructorNames[ins.id] = ins.name;
    
    unordered_map<string, string> courseNames;
    for (const auto &c : courses) courseNames[c.id] = c.name;
    
    cout << "\nSolution found in " << r.solveSeconds << "\n";
    cout << "=========================================\n";

    for (const auto &v : vars) {
        auto it = r.assignments.find(v.varID);
        if (it == r.assignments.end()) continue;
        
        const AssignmentValue &a = it->second;
        const TimeSlot &ts = timeSlots[a.timeslotIndex];
        const Room* rm = nullptr;
        auto rit = roomIndex.find(a.roomID);
        if (rit != roomIndex.end()) rm = rit->second;
        
        string cname = courseNames.count(v.courseID) ? courseNames[v.courseID] : v.courseID;
        string insName = instructorNames.count(a.instructorID) ? instructorNames[a.instructorID] : a.instructorID;
        
        cout << v.courseID << " | " << cname << " (Y" << v.year << ")";
        
        if (v.sessionType == "LECTURE") {
            if (v.year == 3 && !v.specialization.empty()) {
                cout << " | " << v.specialization << " Lecture";
            } else if (v.sectionId > 0) {
                cout << " | G" << v.groupId << " Section " << v.sectionId;
            } else {
                cout << " | G" << v.groupId << " Lecture";
            }
        } else if (v.sessionType == "LAB") {
            cout << " | Lab S" << v.sectionId;
        }
        
        cout << "\n  " << ts.day << " " << minTo12Hour(ts.startMin) << " - " << minTo12Hour(ts.endMin)
             << " | " << (rm ? rm->roomName : a.roomID) << " (" << (rm ? rm->building : "") << ")"
             << " | " << insName << "\n\n";
    }
    
    cout << "=========================================\n";
}
