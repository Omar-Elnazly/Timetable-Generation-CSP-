-- ====== CLEAN DATABASE FOR TIMETABLE GENERATION ======
DROP TABLE IF EXISTS "InstructorCourses";
DROP TABLE IF EXISTS "Instructor";
DROP TABLE IF EXISTS "Courses";
DROP TABLE IF EXISTS "TimeSlots";
DROP TABLE IF EXISTS "Rooms";

-- ====== SCHEMA DEFINITION ======

CREATE TABLE IF NOT EXISTS "Courses" (
  "CourseID"    TEXT PRIMARY KEY,
  "CourseName"  TEXT NOT NULL,
  "Credits"     INTEGER NOT NULL CHECK("Credits" >= 1),
  "Type"        TEXT NOT NULL,
  "Year"        INTEGER NOT NULL CHECK("Year" IN (1, 2, 3, 4)),
  "Specialization" TEXT DEFAULT 'Common',
  "HasLecture"  INTEGER NOT NULL DEFAULT 1 CHECK("HasLecture" IN (0, 1)),
  "HasLab"      INTEGER NOT NULL DEFAULT 0 CHECK("HasLab" IN (0, 1)),
  "IsGradProject" INTEGER NOT NULL DEFAULT 0 CHECK("IsGradProject" IN (0, 1))
);

CREATE TABLE IF NOT EXISTS "Instructor" (
  "InstructorID"    TEXT PRIMARY KEY,
  "Name"            TEXT NOT NULL,
  "Role"            TEXT NOT NULL CHECK("Role" IN ('Professor', 'Assistant Professor')),
  "PreferredSlots"  TEXT NOT NULL,
  "QualifiedCourses" TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS "InstructorCourses" (
  "InstructorID" TEXT NOT NULL,
  "CourseID"     TEXT NOT NULL,
  PRIMARY KEY("InstructorID","CourseID"),
  FOREIGN KEY("InstructorID") REFERENCES "Instructor"("InstructorID") ON DELETE CASCADE,
  FOREIGN KEY("CourseID") REFERENCES "Courses"("CourseID") ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS "TimeSlots" (
  "TimeSlotID"   INTEGER PRIMARY KEY AUTOINCREMENT,
  "Day"          TEXT NOT NULL,
  "StartTimeTxt" TEXT NOT NULL,
  "EndTimeTxt"   TEXT NOT NULL,
  "StartMin"     INTEGER NOT NULL,
  "EndMin"       INTEGER NOT NULL,
  UNIQUE("Day", "StartMin", "EndMin")
);

CREATE TABLE IF NOT EXISTS "Rooms" (
  "RoomID"      TEXT PRIMARY KEY,
  "Building"    TEXT NOT NULL,
  "RoomName"    TEXT NOT NULL,
  "Capacity"    INTEGER NOT NULL,
  "RoomType"    TEXT NOT NULL
);

-- ====== COURSES DATA ======
-- Year 1 Courses
INSERT INTO "Courses" VALUES ('LRA401','Japanese Language (1)',1,'Lecture',1,'Common',1,0,0);
INSERT INTO "Courses" VALUES ('CSC111','Fundamentals of Programming',3,'Lecture and Lab',1,'Common',1,1,0);
INSERT INTO "Courses" VALUES ('MTH111','Mathematics (1) (Calculus + Linear Algebra)',3,'Lecture',1,'Common',1,0,0);
INSERT INTO "Courses" VALUES ('PHY113','Physics 1',3,'Lecture and Lab',1,'Common',1,1,0);
INSERT INTO "Courses" VALUES ('ECE111','Digital Logic Design',3,'Lecture and Lab',1,'Common',1,1,0);
INSERT INTO "Courses" VALUES ('LRA101','Japanese Culture',2,'Lecture',1,'Common',1,0,0);
INSERT INTO "Courses" VALUES ('LRA104','Music and Technology',2,'Lecture',1,'Common',1,0,0);
INSERT INTO "Courses" VALUES ('LRA105','Theater and Drama',2,'Lecture',1,'Common',1,0,0);

-- Year 2 Courses
INSERT INTO "Courses" VALUES ('MTH212','Probability and Statistics',3,'Lecture',2,'Common',1,0,0);
INSERT INTO "Courses" VALUES ('ACM215','Ordinary Differential Equations',3,'Lecture',2,'Common',1,0,0);
INSERT INTO "Courses" VALUES ('LRA403','Japanese Language (3)',1,'Lecture',2,'Common',1,0,0);
INSERT INTO "Courses" VALUES ('CSC211','Software Engineering',3,'Lecture and Lab',2,'Common',1,1,0);
INSERT INTO "Courses" VALUES ('CNC111','Networks and Web Programming',3,'Lecture and Lab',2,'Common',1,1,0);
INSERT INTO "Courses" VALUES ('CSC114','Algorithms Analysis and Design',3,'Lecture and Lab',2,'Common',1,1,0);
INSERT INTO "Courses" VALUES ('CSE214','Computer Organization',3,'Lecture and Lab',2,'Common',1,1,0);
INSERT INTO "Courses" VALUES ('LRA306','Natural Resources and Sustainability',2,'Lecture',2,'Common',1,0,0);

-- Year 3 Common Courses
INSERT INTO "Courses" VALUES ('ECE324','Digital Signal Processing',3,'Lecture and Lab',3,'Common',1,1,0);
INSERT INTO "Courses" VALUES ('CSC317','Computer Graphics and Visualization',3,'Lecture and Lab',3,'Common',1,1,0);
INSERT INTO "Courses" VALUES ('AID312','Intelligent Systems',3,'Lecture and Lab',3,'Common',1,1,0);
INSERT INTO "Courses" VALUES ('CNC311','Computer Networks',3,'Lecture and Lab',3,'Common',1,1,0);
INSERT INTO "Courses" VALUES ('CNC314','Database Systems',3,'Lecture and Lab',3,'Common',1,1,0);

-- Year 3 CNC Specialization
INSERT INTO "Courses" VALUES ('CNC312','Foundations of Information Systems',3,'Lecture and Lab',3,'CNC',1,1,0);

-- Year 3 AID Specialization
INSERT INTO "Courses" VALUES ('AID311','Mathematics of Data Science',3,'Lecture and Lab',3,'AID',1,1,0);

-- Year 3 CSC Specialization
INSERT INTO "Courses" VALUES ('CSC314','Software Modeling and Analysis',3,'Lecture and Lab',3,'CSC',1,1,0);

-- Year 3 BIF Specialization
INSERT INTO "Courses" VALUES ('BIF311','Human Biology',3,'Lecture and Lab',3,'BIF',1,1,0);

-- Year 4 CNC Specialization
INSERT INTO "Courses" VALUES ('CNC413','Digital Forensics',3,'Lecture and Lab',4,'CNC',1,1,0);
INSERT INTO "Courses" VALUES ('CNC419','IT Security and Risk Management',3,'Lecture and Lab',4,'CNC',1,1,0);
INSERT INTO "Courses" VALUES ('CNC418','Software Security',3,'Lecture and Lab',4,'CNC',1,1,0);
INSERT INTO "Courses" VALUES ('CNC324','IT Infrastructure',3,'Lecture and Lab',4,'CNC',1,1,0);
INSERT INTO "Courses" VALUES ('CNC411','Fundamentals of Cybersecurity',3,'Lecture and Lab',4,'CNC',1,1,0);
INSERT INTO "Courses" VALUES ('CNC415','Network Design and Management',3,'Lecture and Lab',4,'CNC',1,1,0);
INSERT INTO "Courses" VALUES ('CNC414','Graduation Project (1)',2,'Lab',4,'CNC',0,1,1);

-- Year 4 AID Specialization
INSERT INTO "Courses" VALUES ('AID321','Machine Learning',3,'Lecture and Lab',4,'AID',1,1,0);
INSERT INTO "Courses" VALUES ('AID428','New Trends in AI',3,'Lecture and Lab',4,'AID',1,1,0);
INSERT INTO "Courses" VALUES ('AID411','BIG Data Analytics & Visualization',3,'Lecture and Lab',4,'AID',1,1,0);
INSERT INTO "Courses" VALUES ('AID417','Advanced Data Mining',3,'Lecture and Lab',4,'AID',1,1,0);
INSERT INTO "Courses" VALUES ('AID414','Graduation Project (1)',2,'Lab',4,'AID',0,1,1);
INSERT INTO "Courses" VALUES ('AID413','Data Security',3,'Lecture and Lab',4,'AID',1,1,0);
INSERT INTO "Courses" VALUES ('AID427','New Trends in Data Science',3,'Lecture and Lab',4,'AID',1,1,0);

-- Year 4 CSC Specialization
INSERT INTO "Courses" VALUES ('CSC414','Game Design & Development',3,'Lecture and Lab',4,'CSC',1,1,0);
INSERT INTO "Courses" VALUES ('CSC415','New Trends in Computer Science',3,'Lecture and Lab',4,'CSC',1,1,0);
INSERT INTO "Courses" VALUES ('CSC426','Distributed Systems',3,'Lecture and Lab',4,'CSC',1,1,0);
INSERT INTO "Courses" VALUES ('CSC410','Software Quality',3,'Lecture and Lab',4,'CSC',1,1,0);
INSERT INTO "Courses" VALUES ('CSC412','Software Security',3,'Lecture and Lab',4,'CSC',1,1,0);
INSERT INTO "Courses" VALUES ('CSC413','Graduation Project (1)',2,'Lab',4,'CSC',0,1,1);
INSERT INTO "Courses" VALUES ('CSC411','Software Verification and Validation (V&V)',3,'Lecture and Lab',4,'CSC',1,1,0);

-- Year 4 BIF Specialization
INSERT INTO "Courses" VALUES ('BIF412','Management and Design of Health Care Systems',3,'Lecture and Lab',4,'BIF',1,1,0);
INSERT INTO "Courses" VALUES ('BIF411','Structural Bioinformatics',3,'Lecture and Lab',4,'BIF',1,1,0);
INSERT INTO "Courses" VALUES ('BIF413','Algorithms in Bioinformatics',3,'Lecture and Lab',4,'BIF',1,1,0);
INSERT INTO "Courses" VALUES ('BIF425','New Trends in Bioinformatics',3,'Lecture and Lab',4,'BIF',1,1,0);
INSERT INTO "Courses" VALUES ('BIF410','Graduation Project (1)',2,'Lab',4,'BIF',0,1,1);
INSERT INTO "Courses" VALUES ('BIF424','IT Infrastructure',3,'Lecture and Lab',4,'BIF',1,1,0);

-- ====== INSTRUCTORS DATA ======

-- Professors (for Lectures)
INSERT INTO "Instructor" VALUES ('PROF01','Dr. Reda Elbasiony','Professor','Not on Tuesday','CSC111');
INSERT INTO "Instructor" VALUES ('PROF02','Dr. Ayman Arafa','Professor','Not on Sunday','MTH111,ACM215');
INSERT INTO "Instructor" VALUES ('PROF03','Dr. Adel Fathy','Professor','Not on Wednesday','PHY113');
INSERT INTO "Instructor" VALUES ('PROF04','Sherine Elmotasem','Professor','Not on Monday','LRA101');
INSERT INTO "Instructor" VALUES ('PROF05','Prof. Ahmed Allam','Professor','Not on Thursday','ECE111');
INSERT INTO "Instructor" VALUES ('PROF06','Dr. Sameh Sherif','Professor','Not on Sunday','ECE111,BIF411,BIF412');
INSERT INTO "Instructor" VALUES ('PROF07','Dr. Ahmed Arafa','Professor','Not on Tuesday','CSC211,AID413,AID427,CNC411');
INSERT INTO "Instructor" VALUES ('PROF08','Dr. Ahmed Anter','Professor','Not on Wednesday','AID311');
INSERT INTO "Instructor" VALUES ('PROF09','Prof. Mostafa Soliman','Professor','Not on Monday','CSE214,CSC317');
INSERT INTO "Instructor" VALUES ('PROF10','Dr. Ahmed Abdel-Malk','Professor','Not on Thursday','ECE324');
INSERT INTO "Instructor" VALUES ('PROF11','Dr. Ahmed Bayumi','Professor','Not on Sunday','AID428,AID312,CSC415');
INSERT INTO "Instructor" VALUES ('PROF12','Dr. Hataba','Professor','Not on Tuesday','CNC418,CNC419,CSC317');
INSERT INTO "Instructor" VALUES ('PROF13','Dr. Reda','Professor','Not on Wednesday','CNC312,CSC317');
INSERT INTO "Instructor" VALUES ('PROF14','Prof. Samir Ahmed','Professor','Not on Monday','CNC413,CNC311,CNC324,AID413');
INSERT INTO "Instructor" VALUES ('PROF15','Dr. Mohamed Issa','Professor','Not on Thursday','BIF425,AID417,CNC314');
INSERT INTO "Instructor" VALUES ('PROF16','Dr. Mustafa AlSayed','Professor','Not on Sunday','CSC426,CSC412,CNC415,CSC314,CSC414');
INSERT INTO "Instructor" VALUES ('PROF17','Dr. Mohamed Akhames','Professor','Not on Tuesday','CSC410,CSC411,CNC324,BIF424,AID321');
INSERT INTO "Instructor" VALUES ('PROF18','Prof. Marghany Hassan','Professor','Not on Wednesday','BIF413,AID321,AID411,CSC114');
INSERT INTO "Instructor" VALUES ('PROF19','Dr. Nadia Fawzy','Professor','Not on Monday','CNC111');
INSERT INTO "Instructor" VALUES ('PROF20','Dr. Karim Hamed','Professor','Not on Thursday','MTH212');
INSERT INTO "Instructor" VALUES ('PROF27','Prof. Adel Al-senn','Professor','Not on Tuesday','LRA104');
INSERT INTO "Instructor" VALUES ('PROF28','Dr. Mohamed El-khateeb','Professor','Not on Wednesday','LRA105');
INSERT INTO "Instructor" VALUES ('PROF29','Prof. Said Sadik','Professor','Not on Monday','LRA306');
INSERT INTO "Instructor" VALUES ('PROF33','Dr. Kenji Tanaka','Professor','Not on Sunday','LRA401,LRA403');
INSERT INTO "Instructor" VALUES ('PROF34','Dr. Yumi Yamamoto','Professor','Not on Thursday','LRA401,LRA403');
INSERT INTO "Instructor" VALUES ('PROF35','Dr. Haruto Ito','Professor','Not on Sunday','LRA401,LRA403');

-- Assistant Professors (for Labs)
INSERT INTO "Instructor" VALUES ('AP01','Eng. Fatma Elsayed','Assistant Professor','Not on Sunday','AID417,CNC311');
INSERT INTO "Instructor" VALUES ('AP02','Eng. Nada Essam','Assistant Professor','Not on Tuesday','AID411,BIF425');
INSERT INTO "Instructor" VALUES ('AP03','Eng. Salma Alashry','Assistant Professor','Not on Wednesday','AID321,CNC314,CSC114');
INSERT INTO "Instructor" VALUES ('AP04','Eng. Mariam Ismael','Assistant Professor','Not on Monday','CSC111,CNC411');
INSERT INTO "Instructor" VALUES ('AP05','Eng. Nada Hamdy','Assistant Professor','Not on Thursday','CSC111,CNC311,BIF411');
INSERT INTO "Instructor" VALUES ('AP06','Eng. Salma Waleed','Assistant Professor','Not on Sunday','AID312,AID311,CSC317');
INSERT INTO "Instructor" VALUES ('AP07','Eng. Menna Hamdi','Assistant Professor','Not on Tuesday','CNC312,CNC111');
INSERT INTO "Instructor" VALUES ('AP08','Eng. Omnya Ramadan','Assistant Professor','Not on Wednesday','CSE214');
INSERT INTO "Instructor" VALUES ('AP09','Eng. Heba Abdelkader','Assistant Professor','Not on Monday','CSE214,AID428');
INSERT INTO "Instructor" VALUES ('AP10','Eng. Nourhan Waleed','Assistant Professor','Not on Thursday','AID312,CNC314,CNC324');
INSERT INTO "Instructor" VALUES ('AP11','Eng. Menna Magdy','Assistant Professor','Not on Sunday','CSC211,CSC317');
INSERT INTO "Instructor" VALUES ('AP12','Eng. Nada Ahmed','Assistant Professor','Not on Tuesday','CSC211,CSC317');
INSERT INTO "Instructor" VALUES ('AP13','Eng. Laila Ibrahim','Assistant Professor','Not on Wednesday','PHY113,ECE111');
INSERT INTO "Instructor" VALUES ('AP14','Eng. Tarek Salah','Assistant Professor','Not on Monday','CSE214,AID311,CNC314');
INSERT INTO "Instructor" VALUES ('AP15','Eng. Rana Mohamed','Assistant Professor','Not on Thursday','ECE324');
INSERT INTO "Instructor" VALUES ('AP16','Eng. Saeed Mostafa','Assistant Professor','Not on Sunday','AID321');
INSERT INTO "Instructor" VALUES ('AP17','Eng. Maria Mounir','Assistant Professor','Not on Tuesday','CNC324');
INSERT INTO "Instructor" VALUES ('AP18','Eng. Alaa Essam','Assistant Professor','Not on Wednesday','AID321,BIF413');

-- ====== INSTRUCTOR-COURSE ASSIGNMENTS ======
-- Year 1
INSERT INTO "InstructorCourses" VALUES ('PROF01','CSC111');
INSERT INTO "InstructorCourses" VALUES ('AP04','CSC111');
INSERT INTO "InstructorCourses" VALUES ('AP05','CSC111');
INSERT INTO "InstructorCourses" VALUES ('PROF02','MTH111');
INSERT INTO "InstructorCourses" VALUES ('PROF03','PHY113');
INSERT INTO "InstructorCourses" VALUES ('AP13','PHY113');
INSERT INTO "InstructorCourses" VALUES ('PROF05','ECE111');
INSERT INTO "InstructorCourses" VALUES ('PROF06','ECE111');
INSERT INTO "InstructorCourses" VALUES ('AP13','ECE111');
INSERT INTO "InstructorCourses" VALUES ('PROF04','LRA101');
INSERT INTO "InstructorCourses" VALUES ('PROF27','LRA104');
INSERT INTO "InstructorCourses" VALUES ('PROF28','LRA105');
INSERT INTO "InstructorCourses" VALUES ('PROF33','LRA401');
INSERT INTO "InstructorCourses" VALUES ('PROF34','LRA401');
INSERT INTO "InstructorCourses" VALUES ('PROF35','LRA401');

-- Year 2
INSERT INTO "InstructorCourses" VALUES ('PROF20','MTH212');
INSERT INTO "InstructorCourses" VALUES ('PROF02','ACM215');
INSERT INTO "InstructorCourses" VALUES ('PROF33','LRA403');
INSERT INTO "InstructorCourses" VALUES ('PROF34','LRA403');
INSERT INTO "InstructorCourses" VALUES ('PROF35','LRA403');
INSERT INTO "InstructorCourses" VALUES ('PROF07','CSC211');
INSERT INTO "InstructorCourses" VALUES ('AP11','CSC211');
INSERT INTO "InstructorCourses" VALUES ('AP12','CSC211');
INSERT INTO "InstructorCourses" VALUES ('PROF19','CNC111');
INSERT INTO "InstructorCourses" VALUES ('AP07','CNC111');
INSERT INTO "InstructorCourses" VALUES ('PROF18','CSC114');
INSERT INTO "InstructorCourses" VALUES ('AP03','CSC114');
INSERT INTO "InstructorCourses" VALUES ('PROF09','CSE214');
INSERT INTO "InstructorCourses" VALUES ('AP08','CSE214');
INSERT INTO "InstructorCourses" VALUES ('AP09','CSE214');
INSERT INTO "InstructorCourses" VALUES ('AP14','CSE214');
INSERT INTO "InstructorCourses" VALUES ('PROF29','LRA306');

-- Year 3 Common
INSERT INTO "InstructorCourses" VALUES ('PROF10','ECE324');
INSERT INTO "InstructorCourses" VALUES ('AP15','ECE324');
INSERT INTO "InstructorCourses" VALUES ('PROF09','CSC317');
INSERT INTO "InstructorCourses" VALUES ('PROF12','CSC317');
INSERT INTO "InstructorCourses" VALUES ('PROF13','CSC317');
INSERT INTO "InstructorCourses" VALUES ('AP06','CSC317');
INSERT INTO "InstructorCourses" VALUES ('AP11','CSC317');
INSERT INTO "InstructorCourses" VALUES ('AP12','CSC317');
INSERT INTO "InstructorCourses" VALUES ('PROF11','AID312');
INSERT INTO "InstructorCourses" VALUES ('AP06','AID312');
INSERT INTO "InstructorCourses" VALUES ('AP10','AID312');
INSERT INTO "InstructorCourses" VALUES ('PROF14','CNC311');
INSERT INTO "InstructorCourses" VALUES ('AP01','CNC311');
INSERT INTO "InstructorCourses" VALUES ('AP05','CNC311');
INSERT INTO "InstructorCourses" VALUES ('PROF15','CNC314');
INSERT INTO "InstructorCourses" VALUES ('AP03','CNC314');
INSERT INTO "InstructorCourses" VALUES ('AP10','CNC314');
INSERT INTO "InstructorCourses" VALUES ('AP14','CNC314');

-- Year 3 Specializations
INSERT INTO "InstructorCourses" VALUES ('PROF13','CNC312');
INSERT INTO "InstructorCourses" VALUES ('AP07','CNC312');
INSERT INTO "InstructorCourses" VALUES ('PROF08','AID311');
INSERT INTO "InstructorCourses" VALUES ('AP06','AID311');
INSERT INTO "InstructorCourses" VALUES ('AP14','AID311');
INSERT INTO "InstructorCourses" VALUES ('PROF16','CSC314');
INSERT INTO "InstructorCourses" VALUES ('PROF06','BIF311');
INSERT INTO "InstructorCourses" VALUES ('AP05','BIF311');

-- Year 4 CNC
INSERT INTO "InstructorCourses" VALUES ('PROF14','CNC413');
INSERT INTO "InstructorCourses" VALUES ('PROF12','CNC419');
INSERT INTO "InstructorCourses" VALUES ('PROF07','CNC411');
INSERT INTO "InstructorCourses" VALUES ('AP04','CNC411');
INSERT INTO "InstructorCourses" VALUES ('PROF12','CNC418');
INSERT INTO "InstructorCourses" VALUES ('PROF14','CNC324');
INSERT INTO "InstructorCourses" VALUES ('PROF17','CNC324');
INSERT INTO "InstructorCourses" VALUES ('AP10','CNC324');
INSERT INTO "InstructorCourses" VALUES ('AP17','CNC324');
INSERT INTO "InstructorCourses" VALUES ('PROF16','CNC415');

-- Year 4 AID
INSERT INTO "InstructorCourses" VALUES ('PROF17','AID321');
INSERT INTO "InstructorCourses" VALUES ('PROF18','AID321');
INSERT INTO "InstructorCourses" VALUES ('AP03','AID321');
INSERT INTO "InstructorCourses" VALUES ('AP16','AID321');
INSERT INTO "InstructorCourses" VALUES ('AP18','AID321');
INSERT INTO "InstructorCourses" VALUES ('PROF11','AID428');
INSERT INTO "InstructorCourses" VALUES ('AP09','AID428');
INSERT INTO "InstructorCourses" VALUES ('PROF18','AID411');
INSERT INTO "InstructorCourses" VALUES ('AP02','AID411');
INSERT INTO "InstructorCourses" VALUES ('PROF15','AID417');
INSERT INTO "InstructorCourses" VALUES ('AP01','AID417');
INSERT INTO "InstructorCourses" VALUES ('PROF07','AID413');
INSERT INTO "InstructorCourses" VALUES ('PROF14','AID413');
INSERT INTO "InstructorCourses" VALUES ('PROF07','AID427');

-- Year 4 CSC
INSERT INTO "InstructorCourses" VALUES ('PROF16','CSC414');
INSERT INTO "InstructorCourses" VALUES ('PROF11','CSC415');
INSERT INTO "InstructorCourses" VALUES ('PROF16','CSC426');
INSERT INTO "InstructorCourses" VALUES ('PROF17','CSC410');
INSERT INTO "InstructorCourses" VALUES ('PROF16','CSC412');
INSERT INTO "InstructorCourses" VALUES ('PROF17','CSC411');

-- Year 4 BIF
INSERT INTO "InstructorCourses" VALUES ('PROF06','BIF412');
INSERT INTO "InstructorCourses" VALUES ('PROF06','BIF411');
INSERT INTO "InstructorCourses" VALUES ('PROF18','BIF413');
INSERT INTO "InstructorCourses" VALUES ('AP18','BIF413');
INSERT INTO "InstructorCourses" VALUES ('PROF15','BIF425');
INSERT INTO "InstructorCourses" VALUES ('AP02','BIF425');
INSERT INTO "InstructorCourses" VALUES ('PROF17','BIF424');

-- ====== ROOMS DATA ======
INSERT INTO "Rooms" VALUES ('B07-F0','B07','F0',25,'Lab');
INSERT INTO "Rooms" VALUES ('B07-F1','B07','F1',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B07-F2','B07','F2',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B07-F20','B07','F20',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B07-F21','B07','F21',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B07-F22','B07','F22',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B07-F23','B07','F23',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B07-F24','B07','F24',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B07-G1','B07','G1',75,'Classroom');
INSERT INTO "Rooms" VALUES ('B8-F6','B8','F6',50,'Classroom');
INSERT INTO "Rooms" VALUES ('B8-F7','B8','F7',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B8-F10','B8','F10',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B8-F11','B8','F11',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B8-F25','B8','F25',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B8-G41','B8','G41',75,'Classroom');
INSERT INTO "Rooms" VALUES ('B9-F4','B9','F4',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B9-F5','B9','F5',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B9-F13','B9','F13',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B9-F14','B9','F14',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B9-F22','B9','F22',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B9-G25','B9','G25',75,'Classroom');
INSERT INTO "Rooms" VALUES ('COE-F21','COE','F21',50,'Lab');
INSERT INTO "Rooms" VALUES ('COE-F22','COE','F22',50,'Lab');
INSERT INTO "Rooms" VALUES ('COE-F11','COE','F11',50,'Lab');
INSERT INTO "Rooms" VALUES ('B17-F3','B17','F3',75,'Classroom');
INSERT INTO "Rooms" VALUES ('B17-F8','B17','F8',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B17-F9','B17','F9',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B17-F13','B17','F13',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B17-F14','B17','F14',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B17-F15','B17','F15',75,'Classroom');
INSERT INTO "Rooms" VALUES ('B17-F16','B17','F16',75,'Classroom');
INSERT INTO "Rooms" VALUES ('B17-F17','B17','F17',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B17-F18','B17','F18',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B17-G9','B17','G9',75,'Classroom');
INSERT INTO "Rooms" VALUES ('B17-G12','B17','G12',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B17-G19','B17','G19',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B17-G20','B17','G20',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B17-G21','B17','G21',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B17-G22','B17','G22',75,'Lab');
INSERT INTO "Rooms" VALUES ('B18-F1','B18','F1',50,'Classroom');
INSERT INTO "Rooms" VALUES ('B18-F2','B18','F2',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B18-F5','B18','F5',50,'Classroom');
INSERT INTO "Rooms" VALUES ('B18-F8','B18','F8',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B18-F10','B18','F10',50,'Classroom');
INSERT INTO "Rooms" VALUES ('B18-F12','B18','F12',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B18-F13','B18','F13',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B18-F14','B18','F14',50,'Classroom');
INSERT INTO "Rooms" VALUES ('B18-F19','B18','F19',50,'Classroom');
INSERT INTO "Rooms" VALUES ('B18-F20','B18','F20',25,'Classroom');
INSERT INTO "Rooms" VALUES ('B18-G1','B18','G1',50,'Lab');
INSERT INTO "Rooms" VALUES ('Blue-Hall','Blue','Hall',150,'Hall');
INSERT INTO "Rooms" VALUES ('Green-Hall','Green','Hall',150,'Hall');
INSERT INTO "Rooms" VALUES ('Yellow-Hall','Yellow','Hall',150,'Hall');
INSERT INTO "Rooms" VALUES ('Red-Hall','Red','Hall',150,'Hall');
INSERT INTO "Rooms" VALUES ('B7-Theater','B7','Theater',100,'Theater');
INSERT INTO "Rooms" VALUES ('B8-Theater','B8','Theater',100,'Theater');
INSERT INTO "Rooms" VALUES ('B9-Theater','B9','Theater',100,'Theater');
INSERT INTO "Rooms" VALUES ('B10-Theater','B10','Theater',100,'Theater');
INSERT INTO "Rooms" VALUES ('B25-G21','B25','G21',100,'Theater');
INSERT INTO "Rooms" VALUES ('B25-F19','B25','F19',100,'Theater');

-- ====== TIME SLOTS DATA ======
-- Sunday
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Sunday','9:00','10:30',540,630);
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Sunday','10:45','12:15',645,735);
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Sunday','12:30','2:00',750,840);
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Sunday','2:15','3:45',855,945);

-- Monday
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Monday','9:00','10:30',540,630);
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Monday','10:45','12:15',645,735);
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Monday','12:30','2:00',750,840);
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Monday','2:15','3:45',855,945);

-- Tuesday
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Tuesday','9:00','10:30',540,630);
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Tuesday','10:45','12:15',645,735);
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Tuesday','12:30','2:00',750,840);
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Tuesday','2:15','3:45',855,945);

-- Wednesday
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Wednesday','9:00','10:30',540,630);
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Wednesday','10:45','12:15',645,735);
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Wednesday','12:30','2:00',750,840);
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Wednesday','2:15','3:45',855,945);

-- Thursday
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Thursday','9:00','10:30',540,630);
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Thursday','10:45','12:15',645,735);
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Thursday','12:30','2:00',750,840);
INSERT INTO "TimeSlots" ("Day","StartTimeTxt","EndTimeTxt","StartMin","EndMin") VALUES ('Thursday','2:15','3:45',855,945);
