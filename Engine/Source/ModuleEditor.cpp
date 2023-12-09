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

        ImGui::SeparatorText("LICENSE:");
        ImGui::BulletText("See the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!");
        ImGui::BulletText("See comments in imgui.cpp.");
        ImGui::BulletText("See example applications in the examples/ folder.");
        ImGui::BulletText("Read the FAQ at https://www.dearimgui.com/faq/");
        ImGui::BulletText("Set 'io.ConfigFlags |= NavEnableKeyboard' for keyboard controls.");
        ImGui::BulletText("Set 'io.ConfigFlags |= NavEnableGamepad' for gamepad controls.");

    }
    ImGui::End();

    return UPDATE_CONTINUE;
}
update_status ModuleEditor::Update() {
    SDL_DisplayMode DM;
    SDL_GetDesktopDisplayMode(0, &DM);
    static bool showDebug = false;
    static bool showTerminal = true;
    static bool AutoScroll = true;
    if (showDebug)
        ImGui::ShowDemoWindow(&showDebug);
    ImGui::Begin("General");
    ImGui::BeginChild("Show windows");
    ImGui::Checkbox("Demo Window", &showDebug);
    ImGui::Checkbox("Logs Window", &showTerminal);
    ImGui::EndChild();
    ImGui::End();
    //TERMINAL
    if (showTerminal) {
        ImGui::SetNextWindowSize(ImVec2(900, 250), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(DM.w/2 - 450, DM.h-300), ImGuiCond_Once);
    
        ImGui::Begin("Logs", &showTerminal);
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