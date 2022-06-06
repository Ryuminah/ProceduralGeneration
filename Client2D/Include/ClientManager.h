#pragma once

#include "GameEngine.h"

class CClientManager
{
public:
    bool Init();

private:
    void CreateMouse();
    void CreateKey();
    void CreateCollision();

    DECLARE_SINGLE(CClientManager)
};

