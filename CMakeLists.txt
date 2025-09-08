cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)


include(CMakePrintHelpers)



project(hand LANGUAGES CXX)
find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets PrintSupport Charts)
find_package(glm)
find_package(Freetype)
#find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets PrintSupport)



#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)


qt_standard_project_setup()

qt_add_executable(hand main.cpp 
                  Render/MyGLWidget.cpp
                  Render/GLProgram.cpp
                  Render/HandObject.cpp
                  Render/GLObject.cpp
                  Render/Camera.cpp
                  Render/InfoBox.cpp
                  Render/Scene.cpp
                  #Render/TextRender.cpp
                  Render/QuadTree.cpp
                  Logic/Triangle.cpp
                  Logic/CoRHandlers.cpp
                  Logic/FileHandler.cpp
                  Logic/AdjList.cpp
                  Logic/Util.cpp
                  Logic/File.cpp
                  MainScreen/MainWidget.cpp
                  #Charts/BarChart.cpp
                  #Charts/PieChart.cpp
                  #Charts/BarPlot.cpp
                  Common/DrawCommands.hpp
                  External/imgui/imgui.cpp
                  External/imgui/imgui_draw.cpp
                  External/imgui/imgui_tables.cpp
                  External/imgui/imgui_widgets.cpp
                  External/imgui/backends/imgui_impl_opengl3.h
)

target_link_libraries(hand PRIVATE Qt6::Core Qt6::Gui Qt6::OpenGL Qt6::Charts Qt6::OpenGLWidgets Qt6::PrintSupport ${FREETYPE_LIBRARIES})
target_include_directories(hand PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/External/imgui ${FREETYPE_INCLUDE_DIRS})
#target_link_libraries(hand PRIVATE Qt6::Core Qt6::Gui Qt6::OpenGL Qt6::OpenGLWidgets Qt6::PrintSupport)
set_property(TARGET hand  PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
#message(PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

