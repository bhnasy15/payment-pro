cmake_minimum_required(VERSION 3.10)

# set the project name
project(payment)

# add the executable
add_executable(payment Application/app.c 
						Card/card.c 
						Terminal/terminal.c 
						Server/server.c 
						Util/util.c)

target_include_directories(payment PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Transactions DB.txt"
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Accounts DB.txt"
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

