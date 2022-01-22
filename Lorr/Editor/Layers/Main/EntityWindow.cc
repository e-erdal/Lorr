#include "EntityWindow.hh"

static i32 g_SelectionMask = (1 << 2);

void EntityWindow::Init()
{
    ZoneScoped;
}

void EntityWindow::Delete()
{
    ZoneScoped;
}

/// https://github.com/ocornut/imgui/blob/62d6be3747924552ebb66046af6d9ebbc4ab0eae/imgui_demo.cpp#L819
void EntityWindow::Update()
{
    ZoneScoped;

    constexpr ImGuiTreeNodeFlags kSelectableTreeNodeFlags =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

    ImGui::PushStyleColor(ImGuiCol_FrameBg, kDefaultGray);
    if (ImGui::BeginListBox("##hierarchylist", ImVec2(-FLT_MIN, -FLT_MIN)))
    {
        ImGui::PopStyleColor();

        u32 clickedNode = -1;
        for (i32 i = 0; i < 144; i++)
        {
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) clickedNode = i;
            
            if (ImGui::TreeNodeEx((void *)(intptr_t)i, kSelectableTreeNodeFlags, "Selectable Node %d", i))
            {
                ImGui::BulletText("Blah blah\nBlah Blah");
                ImGui::TreePop();
            }
        }

        ImGui::EndListBox();
    }
}