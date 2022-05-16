#pragma once
#include "Resource/Animation2D.h"

class CAnim_Image :
    public CAnimation2D
{
public:
	CAnim_Image();
	CAnim_Image(const CAnim_Image& anim);
	virtual ~CAnim_Image();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual CAnim_Image* Clone();
};

