#include "Createrow.h"
#include "imgui.h"

void CreateSensorRow(int id, char* sensorName,
                     size_t nameSize, bool* sensorValue)
{
    ImGui::PushID(id);

    ImGui::TableNextRow();

    ImGui::TableSetColumnIndex(0);
    ImGui::InputText("##name", sensorName, nameSize);

    ImGui::TableSetColumnIndex(1);
    ImGui::TextUnformatted("BOOL");

    ImGui::TableSetColumnIndex(2);
    ImGui::Checkbox("##value", sensorValue);

    ImGui::PopID();
}
