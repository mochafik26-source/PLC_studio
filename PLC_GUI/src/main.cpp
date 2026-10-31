#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <cstring>
#include <sqlite3.h>
#include <iostream>
#include "addingVar.h"
enum class SensorType
{
    BOOL,
    INT,
    FLOAT
};

struct Sensor
{
    char name[64] = "Sensor";
    SensorType type = SensorType::BOOL;

    bool boolValue = false;
    int intValue = 0;
    float floatValue = 0.0f;
};

int main()
{
    sqlite3_open("src/VariablesTab.db", &db);




    // Window visibility
    bool showAddSensor = false;

    // Initialize GLFW
    if (!glfwInit())
        return 1;

    // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        1280, 720, "PLC GUI", nullptr, nullptr
    );

    if (!window)
    {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // PLC variables
    bool plcRunning = false;

    char motorName[32] = "Motor";
    bool motorValue = false;

    // Sensors stored dynamically
    std::vector<Sensor> sensors;

    // Initial sensor
    Sensor initialSensor;
    std::strncpy(
        initialSensor.name,
        "Sensor_1",
        sizeof(initialSensor.name) - 1
    );
    initialSensor.type = SensorType::BOOL;
    initialSensor.boolValue = false;

    sensors.push_back(initialSensor);

    // New sensor form
    static char newSensorName[64] = "Sensor_2";
    static int newSensorType = 0;

    static bool newBoolValue = false;
    static int newIntValue = 0;
    static float newFloatValue = 0.0f;

    const char* types[] = { "BOOL", "INT", "FLOAT" };

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Main application window
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);

        ImGui::Begin(
            "PLC GUI",
            nullptr,
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoTitleBar
        );

        ImGui::Text("PLC Control Panel");

        ImGui::Checkbox("PLC Running", &plcRunning);

        ImGui::Separator();

        float parentWidth = ImGui::GetContentRegionAvail().x;
        float childWidth = 900.0f;

        if (parentWidth > childWidth)
            ImGui::SetCursorPosX(
                ImGui::GetCursorPosX() + parentWidth - childWidth
            );

        ImGui::BeginChild(
            "MainFrame",
            ImVec2(childWidth, 0),
            true
        );

        ImGui::Text("Variables");
        ImGui::Separator();

        if (ImGui::BeginTable(
            "VariablesTable",
            3,
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_SizingStretchProp
        ))
        {
            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Type");
            ImGui::TableSetupColumn("Value");

            ImGui::TableHeadersRow();

            // Motor row
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::InputText(
                "##motorName",
                motorName,
                sizeof(motorName)
            );

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("BOOL");

            ImGui::TableSetColumnIndex(2);
            ImGui::Checkbox("##motorValue", &motorValue);

            // Dynamic sensor rows
            for (size_t i = 0; i < sensors.size(); ++i)
            {
                Sensor& sensor = sensors[i];

                ImGui::PushID(static_cast<int>(i));

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::InputText(
                    "##SensorName",
                    sensor.name,
                    sizeof(sensor.name)
                );

                ImGui::TableSetColumnIndex(1);

                if (sensor.type == SensorType::BOOL)
                    ImGui::Text("BOOL");
                else if (sensor.type == SensorType::INT)
                    ImGui::Text("INT");
                else
                    ImGui::Text("FLOAT");

                ImGui::TableSetColumnIndex(2);

                if (sensor.type == SensorType::BOOL)
                {
                    ImGui::Checkbox(
                        "##SensorValue",
                        &sensor.boolValue
                    );
                }
                else if (sensor.type == SensorType::INT)
                {
                    ImGui::InputInt(
                        "##SensorValue",
                        &sensor.intValue
                    );
                }
                else
                {
                    ImGui::InputFloat(
                        "##SensorValue",
                        &sensor.floatValue
                    );
                }

                ImGui::PopID();
            }

            ImGui::EndTable();
        }

        ImGui::Spacing();

        if (ImGui::Button("+ Add Sensor"))
        {
            showAddSensor = true;
        }

        ImGui::EndChild();

        ImGui::End();

        // Floating Add Sensor window
        if (showAddSensor)
        {
            ImGui::SetNextWindowSize(
                ImVec2(350, 0),
                ImGuiCond_FirstUseEver
            );

            if (ImGui::Begin(
                "Add New Sensor",
                &showAddSensor,
                ImGuiWindowFlags_AlwaysAutoResize
            ))
            {
                ImGui::Text("Sensor Name");

                ImGui::InputText(
                    "##NewSensorName",
                    newSensorName,
                    sizeof(newSensorName)
                );

                ImGui::Text("Sensor Type");

                ImGui::Combo(
                    "##NewSensorType",
                    &newSensorType,
                    types,
                    IM_ARRAYSIZE(types)
                );

                ImGui::Separator();
                ImGui::Text("Initial Value");

                if (newSensorType == 0)
                {
                    ImGui::Checkbox(
                        "##NewBoolValue",
                        &newBoolValue
                    );
                }
                else if (newSensorType == 1)
                {
                    ImGui::InputInt(
                        "##NewIntValue",
                        &newIntValue
                    );
                }
                else
                {
                    ImGui::InputFloat(
                        "##NewFloatValue",
                        &newFloatValue
                    );
                }

                ImGui::Separator();

                if (ImGui::Button("Create"))
                {
                    if (newSensorName[0] != '\0')
                    {
                        Sensor sensor;

                        std::strncpy(
                            sensor.name,
                            newSensorName,
                            sizeof(sensor.name) - 1
                        );

                        sensor.name[sizeof(sensor.name) - 1] = '\0';

                        sensor.type = static_cast<SensorType>(
                            newSensorType
                        );

                        sensor.boolValue = newBoolValue;
                        sensor.intValue = newIntValue;
                        sensor.floatValue = newFloatValue;

                        sensors.push_back(sensor);

                        // Reset the form for the next sensor
                        std::strncpy(
                            newSensorName,
                            "Sensor",
                            sizeof(newSensorName) - 1
                        );

                        newSensorName[
                            sizeof(newSensorName) - 1
                        ] = '\0';

                        newBoolValue = false;
                        newIntValue = 0;
                        newFloatValue = 0.0f;
                        AddVariable(newSensorName, "INT", "IW0", 25);

                        showAddSensor = false;
                    }
                }

                ImGui::SameLine();

                if (ImGui::Button("Cancel"))
                {
                    showAddSensor = false;
                }
            }

            ImGui::End();
        }

        // Render
        ImGui::Render();

        int width, height;

        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(0.10f, 0.10f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(
            ImGui::GetDrawData()
        );

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
        sqlite3_close(db);
    return 0;
}
