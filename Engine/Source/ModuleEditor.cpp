#include "Globals.h"
#include "Application.h"
#include "SDL/include/SDL.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include <fstream>

ModuleEditor::ModuleEditor() {}
ModuleEditor::~ModuleEditor() {}

bool ModuleEditor::Init() {
    SDL_DisplayMode DM;
    SDL_GetDesktopDisplayMode(0, &DM);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, App->GetOpenGL()->context);
    ImGui_ImplOpenGL3_Init("#version 460");
    
    //Windows to use
    myWindows[0] = { "Configuration", true, DM.w / 4, (DM.h / 2) -50, 0, DM.h / 2 };
    myWindows[1] = { "Properties", true, DM.w / 4, DM.h / 2, DM.w *3/4, 0 };
    myWindows[2] = { "Logs", true, DM.w / 2, 250, DM.w / 4, DM.h - 300 };
    return true;
}

update_status ModuleEditor::PreUpdate() {
    const char* ghlink = "https://github.com/JordiAlonsoLlovet/GameProgrammingHomework";
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("General");
    static float buttonH = ImGui::GetTextLineHeight() + 5;
    if (ImGui::Button("Go to GitHub", ImVec2(100, buttonH)))
        ShellExecute(0, 0, ghlink, 0, 0, SW_SHOW);
    ImGui::SameLine();
    ImGui::Text(ghlink);
    if (ImGui::Button("Quit", ImVec2(100, buttonH)))
        return UPDATE_STOP;

    if (ImGui::CollapsingHeader("About"))
    {
        ImGui::SeparatorText("ABOUT THIS ENGINE:");
        ImGui::BulletText("NAME: %s", TITLE);
        ImGui::BulletText("AUTHOT: Jordi Alonso llovet");
        ImGui::BulletText("This engine has been developed as part of the UPC School \n"
            "Master in Videogame Programming.");

        ImGui::SeparatorText("MIT License");
        ImGui::BulletText("Copyright (c) 2023 Jordi Alonso Llovet");
        ImGui::BulletText("Permission is hereby granted, free of charge, to any person obtaining a copy \n"
            "of this software and associated documentation files(the \"Software\"), to deal \n"
            "in the Software without restriction, including without limitation the rights \n"
            "to use, copy, modify, merge, publish, distribute, sublicense, and /or sell \n"
            "copies of the Software, and to permit persons to whom the Software is \n"
            "furnished to do so, subject to the following conditions : ");
        ImGui::BulletText("The above copyright notice and this permission notice shall be included in all \n"
            "copies or substantial portions of the Software.");
        ImGui::BulletText("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR \n"
            "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, \n"
            "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE \n"
            "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER \n"
            "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, \n"
            "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE \n"
            "SOFTWARE.");
    }
    ImGui::Separator();
    ImGui::End();
    for (int i = 0; i < NUM_WINDOWS; ++i) {
        if (myWindows[i].show) {
            ImGui::SetNextWindowSize(ImVec2(myWindows[i].w, myWindows[i].h), ImGuiCond_Once);
            ImGui::SetNextWindowPos(ImVec2(myWindows[i].x, myWindows[i].y), ImGuiCond_Once);
            ImGui::Begin(myWindows[i].title, &myWindows[i].show);
            ImGui::End();
        }
    }

    return UPDATE_CONTINUE;
}
update_status ModuleEditor::Update() {
    SDL_DisplayMode DM;
    SDL_GetDesktopDisplayMode(0, &DM);
    static bool showDebug = false;
    static bool AutoScroll = true;
    if (showDebug)
        ImGui::ShowDemoWindow(&showDebug);
    ImGui::SetNextWindowSize(ImVec2(DM.w/4, DM.h/2), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
    ImGui::Begin("General");
    ImGui::BeginChild("Show windows");
    for (int i = 0; i < NUM_WINDOWS; ++i) {
        ImGui::Checkbox(myWindows[i].title, &myWindows[i].show);
    }
    ImGui::Checkbox("Demo Window", &showDebug);
    ImGui::EndChild();
    ImGui::End();
    //TERMINAL
    if (ShowWindow(LOGS_W)) {
        ImGui::Begin(LOGS_W);
        ImVec2 tSize = ImGui::GetWindowSize();
        char* data = nullptr;
        FILE* file = nullptr;
        fopen_s(&file, "logs.txt", "rb");
        if (file)
        {
            fseek(file, 0, SEEK_END);
            int size = ftell(file);
            data = (char*)malloc(size + 1);
            fseek(file, 0, SEEK_SET);
            fread(data, 1, size, file);
            data[size] = 0;
            fclose(file);
        }
        ImGui::BeginChild("logs", ImVec2(0, -30), true, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::TextUnformatted(data);
        if (AutoScroll)
            ImGui::SetScrollHereY(1.0f);
        ImGui::EndChild();
        ImGui::Checkbox("Auto-scroll", &AutoScroll);
        ImGui::SameLine();
        if (ImGui::Button("Clear", ImVec2(50, 20))) { CLEAR_LOG(); }

        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    };
        
    return UPDATE_CONTINUE;
}


bool ModuleEditor::CleanUp()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return true;
}

bool ModuleEditor::ShowWindow(const char* name) {
    for (MyWindow w : myWindows) {
        if (w.title == name)
            return w.show;
    }
    return false;
}