
#include "ColliderPixel.h"
#include "../Scene/Scene.h"
#include "../Collision.h"
#include "../Scene/SceneResource.h"
#include "ColliderBox2D.h"
#include "ColliderCircle.h"
#include "../PathManager.h"

CColliderPixel::CColliderPixel()
{
	m_ColliderShape = Collider_Shape::Pixel;
	m_ColliderSpace = Collider_Space::Collider2D;

	m_PrimitiveType = PrimitiveComponent_Type::Primitive2D;
	m_2DType = RT2D_Default;
}

CColliderPixel::CColliderPixel(const CColliderPixel& com) :
	CCollider(com)
{
	m_Info = com.m_Info;

	if (com.m_Info.Pixel)
	{
		m_Info.Pixel = new unsigned char[m_Info.Width * m_Info.Height * 4];

		memcpy(m_Info.Pixel, com.m_Info.Pixel, m_Info.Width * m_Info.Height * 4);
	}
}

CColliderPixel::~CColliderPixel()
{
	SAFE_DELETE_ARRAY(m_Info.Pixel);
}

void CColliderPixel::SetTexture(const TCHAR* FileName, const std::string& PathName)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	TCHAR	FullPath[MAX_PATH] = {};

	if (Path)
		lstrcpy(FullPath, Path->pPath);
	lstrcat(FullPath, FileName);

	SetTextureFullPath(FullPath);
}

void CColliderPixel::SetTextureFullPath(const TCHAR* FullPath)
{
	ScratchImage Image = {};

	// 경로에서 확장자를 얻어온다.
	char    strExt[_MAX_EXT] = {};

	char	FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE

	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultibyte, ConvertLength, nullptr, nullptr);

#else

	strcpy_s(FullPathMultibyte, FullPath);

#endif

	_splitpath_s(FullPathMultibyte, nullptr, 0, nullptr, 0,
		nullptr, 0, strExt, _MAX_EXT);

	// 확장자를 대문자로 바꿔준다.
	_strupr_s(strExt);

	if (strcmp(strExt, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(FullPath, DDS_FLAGS_NONE, nullptr,
			Image)))
		{
			return;
		}
	}

	else if (strcmp(strExt, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(FullPath, nullptr, Image)))
		{
			return;
		}
	}

	else
	{
		if (FAILED(LoadFromWICFile(FullPath, WIC_FLAGS_NONE, nullptr, Image)))
		{
			return;
		}
	}

	m_Info.Width = Image.GetImages()[0].width;
	m_Info.Height = Image.GetImages()[0].height;

	/*if ((m_Info.Width * m_Info.Height) != Image.GetPixelsSize())
		return;*/

	m_Info.Pixel = new unsigned char[Image.GetPixelsSize()];

	memcpy(m_Info.Pixel, Image.GetPixels(), Image.GetPixelsSize());

	if (strcmp(strExt, ".BMP") == 0)
	{
		size_t	CopySize = m_Info.Width * 4;

		unsigned char* Line = new unsigned char[CopySize + 1];

		size_t	HalfHeight = m_Info.Height / 2;

		for (size_t i = 0; i < HalfHeight; ++i)
		{
			memcpy(Line, &m_Info.Pixel[i * m_Info.Width * 4], CopySize);
			memcpy(&m_Info.Pixel[i * m_Info.Width * 4],
				&m_Info.Pixel[(m_Info.Height - 1 - i) * m_Info.Width * 4], CopySize);
			memcpy(&m_Info.Pixel[(m_Info.Height - 1 - i) * m_Info.Width * 4], Line,
				CopySize);
		}

		SAFE_DELETE_ARRAY(Line);
	}

	SetWorldScale((float)m_Info.Width, (float)m_Info.Height, 1.f);

	m_Info.Box.Length[0] = m_Info.Width / 2.f;
	m_Info.Box.Length[1] = m_Info.Height / 2.f;
}

void CColliderPixel::SetPixelInfoColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_Info.Color[0] = r;
	m_Info.Color[1] = g;
	m_Info.Color[2] = b;
	m_Info.Color[3] = a;
}

void CColliderPixel::SetPixelCollisionType(PixelCollision_Type Type)
{
	m_Info.Type = Type;
}

void CColliderPixel::Start()
{
	CCollider::Start();

	SetWorldScale((float)m_Info.Width, (float)m_Info.Height, 1.f);

	m_DebugMesh = m_pScene->GetResource()->FindMesh("ColliderBox2D");
}

bool CColliderPixel::Init()
{
	if (!CCollider::Init())
		return false;

	m_Info.Box.Axis[AXIS_X] = Vector2(1.f, 0.f);
	m_Info.Box.Axis[AXIS_Y] = Vector2(0.f, 1.f);

	SetUpdatePosZ(true);

	return true;
}

void CColliderPixel::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CColliderPixel::PostUpdate(float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);



	m_Info.Box.Center.x = GetWorldPos().x;
	m_Info.Box.Center.y = GetWorldPos().y;

	m_Info.Box.Axis[AXIS_X].x = GetAxis(AXIS_X).x;
	m_Info.Box.Axis[AXIS_X].y = GetAxis(AXIS_X).y;

	m_Info.Box.Axis[AXIS_Y].x = GetAxis(AXIS_Y).x;
	m_Info.Box.Axis[AXIS_Y].y = GetAxis(AXIS_Y).y;

	Vector2	Pos[4] = {};

	Pos[0] = m_Info.Box.Center - m_Info.Box.Axis[AXIS_X] * m_Info.Box.Length[0] +
		m_Info.Box.Axis[AXIS_Y] * m_Info.Box.Length[1];
	Pos[1] = m_Info.Box.Center + m_Info.Box.Axis[AXIS_X] * m_Info.Box.Length[0] +
		m_Info.Box.Axis[AXIS_Y] * m_Info.Box.Length[1];

	Pos[2] = m_Info.Box.Center - m_Info.Box.Axis[AXIS_X] * m_Info.Box.Length[0] -
		m_Info.Box.Axis[AXIS_Y] * m_Info.Box.Length[1];
	Pos[3] = m_Info.Box.Center + m_Info.Box.Axis[AXIS_X] * m_Info.Box.Length[0] -
		m_Info.Box.Axis[AXIS_Y] * m_Info.Box.Length[1];

	m_Min.x = Pos[0].x;
	m_Min.y = Pos[0].y;

	m_Max.x = Pos[0].x;
	m_Max.y = Pos[0].y;

	for (int i = 1; i < 4; ++i)
	{
		if (m_Min.x > Pos[i].x)
			m_Min.x = Pos[i].x;

		if (m_Min.y > Pos[i].y)
			m_Min.y = Pos[i].y;

		if (m_Max.x < Pos[i].x)
			m_Max.x = Pos[i].x;

		if (m_Max.y < Pos[i].y)
			m_Max.y = Pos[i].y;
	}

	SetWorldRotation(0.f, 0.f, 0.f);

	m_Info.Min = m_Min;
	m_Info.Max = m_Max;
}

void CColliderPixel::Collision(float DeltaTime)
{
	CCollider::Collision(DeltaTime);
}

void CColliderPixel::PrevRender(float DeltaTime)
{
	CCollider::PrevRender(DeltaTime);
}

void CColliderPixel::Render(float DeltaTime)
{
	CCollider::Render(DeltaTime);
}

CColliderPixel* CColliderPixel::Clone()
{
	return new CColliderPixel(*this);
}

bool CColliderPixel::Collision(CCollider* Dest)
{
	bool	result = false;

	switch (Dest->GetColliderShape())
	{
	case Collider_Shape::Box2D:
	{
		HitResult	DestResult = {};
		result = CCollision::CollisionBox2DToPixel(DestResult, m_HitResult,
			(CColliderBox2D*)Dest, this);
		Dest->SetHitResult(DestResult);
	}
	break;
	case Collider_Shape::Circle:
	{
		HitResult	DestResult = {};
		result = CCollision::CollisionCircleToPixel(DestResult, m_HitResult,
			(CColliderCircle*)Dest, this);
		Dest->SetHitResult(DestResult);
	}
	break;
	}

	return result;
}

bool CColliderPixel::CollisionMouse(const Vector2& MousePos)
{
	HitResult	SrcResult = {};
	HitResult	DestResult = {};

	m_MouseCollision = CCollision::CollisionPixelToPoint(SrcResult, DestResult,
		m_Info, MousePos);

	return m_MouseCollision;
}
