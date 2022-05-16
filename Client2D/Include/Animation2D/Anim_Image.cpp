#include "Anim_Image.h"

CAnim_Image::CAnim_Image()
{
}

CAnim_Image::CAnim_Image(const CAnim_Image& anim) :
	CAnimation2D(anim)
{

}

CAnim_Image::~CAnim_Image()
{
}

void CAnim_Image::Start()
{
	CAnimation2D::Start();
}

bool CAnim_Image::Init()
{
	if (!CAnimation2D::Init())
		return false;

	AddAnimationSequence2D("Image_Background", false);
	AddAnimationSequence2D("Image_BackgroundWhite", false);

	return true;
}

void CAnim_Image::Update(float DeltaTime)
{
	CAnimation2D::Update(DeltaTime);
}

CAnim_Image* CAnim_Image::Clone()
{
	return new CAnim_Image(*this);
}
