cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)


include(CMakePrintHelpers)

project(hand LANGUAGES CXX)
find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets PrintSupport Charts)
find_package(Qt5 COMPONENTS Widgets)
find_package(glm)
find_package(Freetype)



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
                  External/imgui/backends/imgui_impl_opengl3.cpp

                  External/imgui/imgui_demo.cpp

                  External/implot/implot.cpp
                  External/implot/implot_items.cpp

                  External/imguifiledialog/ImGuiFileDialog.cpp
)

target_link_libraries(hand PRIVATE Qt6::Core Qt6::Gui Qt6::OpenGL Qt6::Charts Qt6::OpenGLWidgets Qt6::PrintSupport ${FREETYPE_LIBRARIES})
target_include_directories(hand PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/External/imgui ${CMAKE_CURRENT_SOURCE_DIR}/External/implot ${CMAKE_CURRENT_SOURCE_DIR}/External/imguifiledialog ${FREETYPE_INCLUDE_DIRS})

#cmake_minimum_required(VERSION 3.16)
#set(CMAKE_CXX_STANDARD 20)
#
#include(CMakePrintHelpers)
#
#project(hand LANGUAGES CXX)
#
## --- Qt6 packages ---
#find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets PrintSupport Charts)
#find_package(glm)
#find_package(Freetype)
#
#qt_standard_project_setup()
#
## --- Add qtimgui submodule (PR) ---
#add_subdirectory(External/qtimgui)
#
## --- Add main executable ---
#qt_add_executable(hand
#    main.cpp 
#    Render/MyGLWidget.cpp
#    Render/GLProgram.cpp
#    Render/HandObject.cpp
#    Render/GLObject.cpp
#    Render/Camera.cpp
#    Render/InfoBox.cpp
#    Render/Scene.cpp
#    Render/QuadTree.cpp
#    Logic/Triangle.cpp
#    Logic/CoRHandlers.cpp
#    Logic/FileHandler.cpp
#    Logic/AdjList.cpp
#    Logic/Util.cpp
#    Logic/File.cpp
#    MainScreen/MainWidget.cpp
#    Common/DrawCommands.hpp
#)
#
## --- Link libraries ---
#target_link_libraries(hand PRIVATE
#    Qt6::Core
#    Qt6::Gui
#    Qt6::OpenGL
#    Qt6::Charts
#    Qt6::OpenGLWidgets
#    Qt6::PrintSupport
#    ${FREETYPE_LIBRARIES}
#    imgui                # <- explicitly link ImGui library from PR
#    qt_imgui_widgets     # <- Qt widget renderer
#)
#
#target_sources(hand PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}/External/imgui/backends/imgui_impl_opengl3.cpp
#    )
## --- Include directories ---
## Needed for your code to include ImGui headers directly
#target_include_directories(hand PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}/External/qtimgui/modules/imgui
#    ${CMAKE_CURRENT_SOURCE_DIR}/External/imgui/backends
#    ${FREETYPE_INCLUDE_DIRS}
#)
#
## --- Set VS debugger working directory ---
#set_property(TARGET hand PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

