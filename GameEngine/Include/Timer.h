#pragma once

#include "GameEngine.h"

class CTimer
{
public:
    CTimer();
    ~CTimer();

private:
    LARGE_INTEGER   m_Second;
    LARGE_INTEGER   m_Time;
    float           m_DeltaTime;
    float           m_FPS;
    float           m_FPSTime;
    int             m_Tick;
    bool            m_FPSEnable;

public:
    float GetDeltaTime()    const
    {
        return m_DeltaTime;
    }

    float GetFPS()  const
    {
        return m_FPS;
    }

    bool IsFPS()    const
    {
        return m_FPSEnable;
    }

public:
    bool Init();
    void Update();
};

