#include "Utils.h"

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[Opengl Error] (" << error << "): " <<
            function << " " << file << ":" << line << '\n';
        return false;
    }
    return true;
}

glm::vec3 GetColor(ActivityState activity)
{
    switch (activity)
    {
    case ActivityState::ACTIVE:
        return glm::vec3(0.f, 1.f, 0.f);
    case ActivityState::NOT_ACTIVE:
        return glm::vec3(0.5f, 0.5f, 0.5f);
    case ActivityState::WILL_BE_ACTIVE:
        return glm::vec3(1.f, 1.f, 0.f);
    }
}

void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}