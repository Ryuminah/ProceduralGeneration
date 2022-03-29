#pragma once

#include "GameEngine.h"

class CIMGUIManager
{
private:
    ImGuiContext* m_Context;
    std::unordered_map<std::string, class CIMGUIWindow*>    m_mapWindow;
    std::unordered_map<std::string, ImFont*>    m_mapFont;

public:
    bool Init(HWND hWnd);
    void Update(float DeltaTime);
    void Render();
    class CIMGUIWindow* FindIMGUIWindow(const std::string& Name);

public:
    bool AddFont(const std::string& Name, const char* FileName,
        float Size, bool Korea = false, int OverH = 2, int OverV = 1,
        float Spacing = 1.f, const std::string& PathName = FONT_PATH);
    bool AddFontFullPath(const std::string& Name, const char* FullPath,
        float Size, bool Korea = false, int OverH = 2, int OverV = 1,
        float Spacing = 1.f);
    ImFont* FindFont(const std::string& Name);

public:
    template <typename T>
    T* AddWindow(const std::string& Name)
    {
        T* Window = (T*)FindIMGUIWindow(Name);

        if (Window)
            return Window;

        Window = new T;

        Window->SetName(Name);
        Window->Init();

        m_mapWindow.insert(std::make_pair(Name, Window));

        return Window;
    }

    DECLARE_SINGLE(CIMGUIManager)
};

