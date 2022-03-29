#pragma once

#include "Resource/Animation2D.h"

class CPlayerAnimation2D :
    public CAnimation2D
{
public:
	CPlayerAnimation2D();
	CPlayerAnimation2D(const CPlayerAnimation2D& anim);
	virtual ~CPlayerAnimation2D();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual CPlayerAnimation2D* Clone();
};

