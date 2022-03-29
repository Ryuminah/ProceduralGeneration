#pragma once

#include "GameEngine.h"

class CClientManager
{
public:
    bool Init();

private:
    void CreateMouse();

    DECLARE_SINGLE(CClientManager)
};

