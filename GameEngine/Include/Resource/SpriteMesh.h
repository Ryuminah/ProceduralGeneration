#pragma once

#include "Mesh.h"

class CSpriteMesh :
    public CMesh
{
    friend class CResourceManager;

protected:
    CSpriteMesh();
    virtual ~CSpriteMesh();
};

