###1
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_DIR} ${OPENCV_DIR}/build)

include_directories(include)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
#set(CMAKE_AUTOMOC ON)

###3
#Find the QtWidgets library
#find_package(Qt5Widgets REQUIRED)
#find_package(Qt5AxContainer REQUIRED)
#find_package(Qt5Xml)
find_package(Qt5 COMPONENTS Widgets AxContainer Sql)

if(Qt5Widgets_FOUND)
message("<<<Qt5Widgets_FOUND>>>")
endif()

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Bin")

add_definitions(${QT_DEFINITIONS})

###############################################
set(OpenCV_STATIC OFF)
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
message("<<<OpenCV_FOUND>>>")
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})
endif()

###############################################
FILE(GLOB HEADERS "include/*.h")
FILE(GLOB SOURCES "src/*.cpp")
FILE(GLOB FORMS "form/*.ui")
qt5_wrap_cpp(MOC_HEADERS ${HEADERS})
qt5_wrap_ui(FORMS_HEADERS ${FORMS})

###4
add_executable(MYUI WIN32 ${HEADERS} ${SOURCES} ${FORMS} ${MOC_HEADERS} ${FORMS_HEADERS})

###5
target_link_libraries(MYUI Qt5::Widgets Qt5::AxContainer Qt5::Sql)

###Not debug
target_link_libraries(MYUI debug opencv_world300d.lib optimized opencv_world300.lib)

###############################################
FILE(GLOB QT_RUNTIME ${QT_DIR}/bin/*.dll)
#INSTALL(FILES ${QT_RUNTIME} DESTINATION "${PROJECT_SOURCE_DIR}/Bin")
#INSTALL(DIRECTORY ${QT_DIR}/plugins/ DESTINATION .)
FILE(COPY ${QT_RUNTIME} DESTINATION "${PROJECT_SOURCE_DIR}/Bin")
FILE(COPY ${QT_DIR}/plugins/ DESTINATION "${PROJECT_SOURCE_DIR}/Bin")

FILE(GLOB OPENCV_RUNTIME ${OPENCV_DIR}/build/x86/vc12/bin/*.dll)
#INSTALL(FILES ${OPENCV_RUNTIME} DESTINATION .)
FILE(COPY ${OPENCV_RUNTIME} DESTINATION "${PROJECT_SOURCE_DIR}/Bin")

