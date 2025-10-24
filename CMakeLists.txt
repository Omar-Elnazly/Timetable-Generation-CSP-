cmake_minimum_required(VERSION 3.28)
project(TestSql)

set(CMAKE_CXX_STANDARD 17)

add_executable(TestSql 
        sqlite/sqlite3.h 
        sqlite/sqlite3.c
        src/main.cpp
        src/database_manager.cpp
        src/csp_solver.cpp
        include/database_manager.h
        include/models.h
        include/csp_solver.h
)
