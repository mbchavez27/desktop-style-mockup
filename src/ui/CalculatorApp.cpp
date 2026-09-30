#include "imgui.h"
#include "core/AppState.h"
#include "ui/CalculatorApp.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace
{

constexpr float kDisplayH = 48.0f;
constexpr size_t kMaxEntry = 31; // entry[32] keeps a NUL

} // namespace

void CalculatorApp::Evaluate()
{
    if (!pending_op)
        return;
    const double rhs = std::strtod(entry, nullptr);
    double result = 0.0;
    switch (pending_op)
    {
    case '+':
        result = accumulator + rhs;
        break;
    case '-':
        result = accumulator - rhs;
        break;
    case '*':
        result = accumulator * rhs;
        break;
    case '/':
        if (rhs == 0.0)
        {
            error = true;
            pending_op = 0;
            std::snprintf(entry, sizeof(entry), "Error");
            return;
        }
        result = accumulator / rhs;
        break;
    default:
        return;
    }
    std::snprintf(entry, sizeof(entry), "%.12g", result);
    accumulator = result;
}

void CalculatorApp::InputDigit(char digit)
{
    if (error)
        AllClear();
    if (fresh_entry)
    {
        entry[0] = digit;
        entry[1] = '\0';
        fresh_entry = false;
        return;
    }
    size_t len = std::strlen(entry);
    if (len == 1 && entry[0] == '0')
        len = 0; // typing after a lone "0" replaces it
    if (len >= kMaxEntry)
        return;
    entry[len] = digit;
    entry[len + 1] = '\0';
}

void CalculatorApp::InputDot()
{
    if (error)
    {
        AllClear();
        return;
    }
    if (fresh_entry)
    {
        std::snprintf(entry, sizeof(entry), "0.");
        fresh_entry = false;
        return;
    }
    if (std::strchr(entry, '.') == nullptr && std::strlen(entry) < kMaxEntry)
    {
        const size_t len = std::strlen(entry);
        entry[len] = '.';
        entry[len + 1] = '\0';
    }
}

void CalculatorApp::InputOp(char op)
{
    if (error)
        return;
    if (pending_op && !fresh_entry)
        Evaluate();
    else if (!pending_op)
        accumulator = std::strtod(entry, nullptr);
    if (error)
        return; // division by zero landed during Evaluate
    pending_op = op;
    fresh_entry = true;
}

void CalculatorApp::Equals()
{
    if (error || !pending_op)
        return;
    Evaluate();
    pending_op = 0;
    fresh_entry = true;
}

void CalculatorApp::ToggleSign()
{
    if (error)
        return;
    const size_t len = std::strlen(entry);
    if (len == 0)
        return;
    if (entry[0] == '-')
    {
        std::memmove(entry, entry + 1, len); // strip '-' (len includes NUL)
    }
    else if (!(len == 1 && entry[0] == '0') && len + 2 <= sizeof(entry))
    {
        std::memmove(entry + 1, entry, len + 1); // room for '-' + chars + NUL
        entry[0] = '-';
    }
}

void CalculatorApp::AllClear()
{
    accumulator = 0.0;
    pending_op = 0;
    std::snprintf(entry, sizeof(entry), "0");
    fresh_entry = true;
    error = false;
}

void CalculatorApp::RenderDisplay()
{
    ImGui::BeginChild("##display", ImVec2(0.0f, kDisplayH), ImGuiChildFlags_Borders);
    const char *text = error ? "Error" : entry;
    const float w = ImGui::CalcTextSize(text).x;
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - w);
    ImGui::TextUnformatted(text);
    ImGui::EndChild();
}

void CalculatorApp::RenderKeypad()
{
    const float gap = ImGui::GetStyle().ItemSpacing.x;
    const float bw = (ImGui::GetContentRegionAvail().x - 3.0f * gap) / 4.0f;
    const ImVec2 sz(bw, 0.0f);
    const ImVec2 wide(2.0f * bw + gap, 0.0f);

    if (ImGui::Button("C", sz))
        AllClear();
    ImGui::SameLine();
    if (ImGui::Button("+/-", sz))
        ToggleSign();
    ImGui::SameLine();
    if (ImGui::Button(".", sz))
        InputDot();
    ImGui::SameLine();
    if (ImGui::Button("/", sz))
        InputOp('/');

    if (ImGui::Button("7", sz))
        InputDigit('7');
    ImGui::SameLine();
    if (ImGui::Button("8", sz))
        InputDigit('8');
    ImGui::SameLine();
    if (ImGui::Button("9", sz))
        InputDigit('9');
    ImGui::SameLine();
    if (ImGui::Button("*", sz))
        InputOp('*');

    if (ImGui::Button("4", sz))
        InputDigit('4');
    ImGui::SameLine();
    if (ImGui::Button("5", sz))
        InputDigit('5');
    ImGui::SameLine();
    if (ImGui::Button("6", sz))
        InputDigit('6');
    ImGui::SameLine();
    if (ImGui::Button("-", sz))
        InputOp('-');

    if (ImGui::Button("1", sz))
        InputDigit('1');
    ImGui::SameLine();
    if (ImGui::Button("2", sz))
        InputDigit('2');
    ImGui::SameLine();
    if (ImGui::Button("3", sz))
        InputDigit('3');
    ImGui::SameLine();
    if (ImGui::Button("+", sz))
        InputOp('+');

    if (ImGui::Button("0", wide))
        InputDigit('0');
    ImGui::SameLine();
    if (ImGui::Button("=", sz))
        Equals();
}

void CalculatorApp::Render()
{
    ImGui::SetNextWindowSize(ImVec2(240, 300), ImGuiCond_FirstUseEver);
    ImGui::Begin("Calculator", &AppState::show_app_1);
    RenderDisplay();
    RenderKeypad();
    ImGui::End();
}
