#include "Collision.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"

bool CCollision::CollisionBox2DToBox2D(HitResult& SrcResult, HitResult& DestResult, 
	CColliderBox2D* Src, CColliderBox2D* Dest)
{
	if (CollisionBox2DToBox2D(SrcResult, DestResult, Src->GetInfo(), Dest->GetInfo()))
	{
		SrcResult.DestCollider = Dest;
		SrcResult.DestObject = Dest->GetOwner();

		DestResult.DestCollider = Src;
		DestResult.DestObject = Src->GetOwner();
		return true;
	}

	SrcResult.DestCollider = Dest;
	SrcResult.DestObject = Dest->GetOwner();

	DestResult.DestCollider = Src;
	DestResult.DestObject = Src->GetOwner();

	return false;
}

bool CCollision::CollisionCircleToCircle(HitResult& SrcResult, HitResult& DestResult, 
	CColliderCircle* Src, CColliderCircle* Dest)
{
	if (CollisionCircleToCircle(SrcResult, DestResult, Src->GetInfo(), Dest->GetInfo()))
	{
		SrcResult.DestCollider = Dest;
		SrcResult.DestObject = Dest->GetOwner();

		DestResult.DestCollider = Src;
		DestResult.DestObject = Src->GetOwner();
		return true;
	}

	SrcResult.DestCollider = Dest;
	SrcResult.DestObject = Dest->GetOwner();

	DestResult.DestCollider = Src;
	DestResult.DestObject = Src->GetOwner();

	return false;
}

bool CCollision::CollisionBox2DToCircle(HitResult& SrcResult, HitResult& DestResult, 
	CColliderBox2D* Src, CColliderCircle* Dest)
{
	if (CollisionBox2DToCircle(SrcResult, DestResult, Src->GetInfo(), Dest->GetInfo()))
	{
		SrcResult.DestCollider = Dest;
		SrcResult.DestObject = Dest->GetOwner();

		DestResult.DestCollider = Src;
		DestResult.DestObject = Src->GetOwner();
		return true;
	}

	SrcResult.DestCollider = Dest;
	SrcResult.DestObject = Dest->GetOwner();

	DestResult.DestCollider = Src;
	DestResult.DestObject = Src->GetOwner();

	return false;
}

bool CCollision::CollisionBox2DToPixel(HitResult& SrcResult, HitResult& DestResult, 
	CColliderBox2D* Src, CColliderPixel* Dest)
{
	if (CollisionBox2DToPixel(SrcResult, DestResult, Src->GetInfo(), Dest->GetInfo()))
	{
		SrcResult.DestCollider = Dest;
		SrcResult.DestObject = Dest->GetOwner();

		DestResult.DestCollider = Src;
		DestResult.DestObject = Src->GetOwner();
		return true;
	}

	SrcResult.DestCollider = Dest;
	SrcResult.DestObject = Dest->GetOwner();

	DestResult.DestCollider = Src;
	DestResult.DestObject = Src->GetOwner();

	return false;
}

bool CCollision::CollisionCircleToPixel(HitResult& SrcResult, HitResult& DestResult, 
	CColliderCircle* Src, CColliderPixel* Dest)
{
	if (CollisionCircleToPixel(SrcResult, DestResult, Src->GetInfo(), Dest->GetInfo()))
	{
		SrcResult.DestCollider = Dest;
		SrcResult.DestObject = Dest->GetOwner();

		DestResult.DestCollider = Src;
		DestResult.DestObject = Src->GetOwner();
		return true;
	}

	SrcResult.DestCollider = Dest;
	SrcResult.DestObject = Dest->GetOwner();

	DestResult.DestCollider = Src;
	DestResult.DestObject = Src->GetOwner();

	return false;
}

bool CCollision::CollisionBox2DToBox2D(HitResult& SrcResult, HitResult& DestResult, 
	const Box2DInfo& Src, const Box2DInfo& Dest)
{
	Vector2 CenterDir = Src.Center - Dest.Center;

	// Src의 X축으로 투영
	Vector2 Axis = Src.Axis[AXIS_X];

	float	CenterDot = abs(CenterDir.Dot(Axis));

	float   r1, r2;

	r1 = Src.Length[0];
	r2 = abs(Dest.Axis[AXIS_X].Dot(Axis)) * Dest.Length[0] +
		abs(Dest.Axis[AXIS_Y].Dot(Axis)) * Dest.Length[1];

	if (CenterDot > r1 + r2)
		return false;

	Axis = Src.Axis[AXIS_Y];

	CenterDot = abs(CenterDir.Dot(Axis));

	r1 = Src.Length[1];
	r2 = abs(Dest.Axis[AXIS_X].Dot(Axis)) * Dest.Length[0] +
		abs(Dest.Axis[AXIS_Y].Dot(Axis)) * Dest.Length[1];

	if (CenterDot > r1 + r2)
		return false;

	Axis = Dest.Axis[AXIS_X];

	CenterDot = abs(CenterDir.Dot(Axis));

	r1 = Dest.Length[0];
	r2 = abs(Src.Axis[AXIS_X].Dot(Axis)) * Src.Length[0] +
		abs(Src.Axis[AXIS_Y].Dot(Axis)) * Src.Length[1];

	if (CenterDot > r1 + r2)
		return false;

	Axis = Dest.Axis[AXIS_Y];

	CenterDot = abs(CenterDir.Dot(Axis));

	r1 = Dest.Length[1];
	r2 = abs(Src.Axis[AXIS_X].Dot(Axis)) * Src.Length[0] +
		abs(Src.Axis[AXIS_Y].Dot(Axis)) * Src.Length[1];

	if (CenterDot > r1 + r2)
		return false;

	return true;
}

bool CCollision::CollisionCircleToCircle(HitResult& SrcResult, HitResult& DestResult,
	const CircleInfo& Src, const CircleInfo& Dest)
{
	float	Distance = Src.Center.Distance(Dest.Center);

	bool	Result = Distance <= Src.Radius + Dest.Radius;

	if (Result)
	{
		Vector2	Center = (Src.Center + Dest.Center) / 2.f;
		SrcResult.HitPoint = Vector3(Center.x, Center.y, 0.f);
		DestResult.HitPoint = Vector3(Center.x, Center.y, 0.f);
	}

	return Result;
}

bool CCollision::CollisionBox2DToCircle(HitResult& SrcResult, HitResult& DestResult, 
	const Box2DInfo& Src, const CircleInfo& Dest)
{
	Vector2 CenterDir = Src.Center - Dest.Center;

	// Src의 X축으로 투영
	Vector2 Axis = Src.Axis[AXIS_X];
	Axis.Normalize();

	float	CenterDot = abs(CenterDir.Dot(Axis));

	float   r1, r2;

	r1 = Src.Length[0];
	r2 = Dest.Radius;

	if (CenterDot > r1 + r2)
		return false;

	Axis = Src.Axis[AXIS_Y];
	Axis.Normalize();

	CenterDot = abs(CenterDir.Dot(Axis));

	r1 = Src.Length[1];
	r2 = Dest.Radius;

	if (CenterDot > r1 + r2)
		return false;


	Axis = CenterDir;
	Axis.Normalize();

	CenterDot = abs(CenterDir.Dot(Axis));

	r1 = abs(Src.Axis[AXIS_X].Dot(Axis)) * Src.Length[0] +
		abs(Src.Axis[AXIS_Y].Dot(Axis)) * Src.Length[1];
	r2 = Dest.Radius;

	if (CenterDot > r1 + r2)
		return false;

	return true;
}

bool CCollision::CollisionBox2DToPixel(HitResult& SrcResult, HitResult& DestResult, 
	const Box2DInfo& Src, const PixelInfo& Dest)
{
	if (!CollisionBox2DToBox2D(SrcResult, DestResult, Src, Dest.Box))
		return false;

	// 교집합 사각형을 구한다.
	float	Left = Src.Min.x < Dest.Min.x ? Dest.Min.x : Src.Min.x;
	float	Right = Src.Max.x > Dest.Max.x ? Dest.Max.x : Src.Max.x;
	float	Top = Src.Max.y > Dest.Max.y ? Dest.Max.y : Src.Max.y;
	float	Bottom = Src.Min.y < Dest.Min.y ? Dest.Min.y : Src.Min.y;

	// 픽셀 충돌체의 좌 하단 좌표를 구한다.
	Vector2	LeftBottom = Dest.Box.Center - Vector2(Dest.Box.Length[0], Dest.Box.Length[1]);

	Left -= LeftBottom.x;
	Right -= LeftBottom.x;

	Top -= LeftBottom.y;
	Bottom -= LeftBottom.y;

	Left = Left < 0.f ? 0.f : Left;
	Right = Right >= (float)Dest.Width ? (float)Dest.Width - 1 : Right;

	Bottom = Bottom < 0.f ? 0.f : Bottom;
	Top = Top >= (float)Dest.Height ? (float)Dest.Height - 1 : Top;

	Top = Dest.Height - Top;
	Bottom = Dest.Height - Bottom;

	for (int y = (int)Top; y < (int)Bottom; ++y)
	{
		for (int x = (int)Left; x <= (int)Right; ++x)
		{
			int	Index = y * (int)Dest.Width * 4 + x * 4;

			switch (Dest.Type)
			{
			case PixelCollision_Type::Color_Ignore:
			{
				if (Dest.Pixel[Index] == Dest.Color[0] &&
					Dest.Pixel[Index + 1] == Dest.Color[1] &&
					Dest.Pixel[Index + 2] == Dest.Color[2])
					continue;

				// 무시할 색상이 아니라면 이 픽셀의 점이 Box2D 안에 들어가는지 판단한다.
				// 픽셀의 월드상에서의 위치를 만들어낸다.
				Vector2	PixelPos = LeftBottom + Vector2((float)x, (float)Dest.Height - y);

				if (CollisionBox2DToPoint(SrcResult, DestResult, Src, PixelPos))
				{
					SrcResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					DestResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					return true;
				}
			}
				break;
			case PixelCollision_Type::Color_Confirm:
			{
				if (Dest.Pixel[Index] != Dest.Color[0] ||
					Dest.Pixel[Index + 1] != Dest.Color[1] ||
					Dest.Pixel[Index + 2] != Dest.Color[2])
					continue;

				// 무시할 색상이 아니라면 이 픽셀의 점이 Box2D 안에 들어가는지 판단한다.
				// 픽셀의 월드상에서의 위치를 만들어낸다.
				Vector2	PixelPos = LeftBottom + Vector2((float)x, (float)Dest.Height - y);

				if (CollisionBox2DToPoint(SrcResult, DestResult, Src, PixelPos))
				{
					SrcResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					DestResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					return true;
				}
			}
				break;
			case PixelCollision_Type::Alpha_Ignore:
			{
				if (Dest.Pixel[Index + 3] == Dest.Color[3])
					continue;

				// 무시할 색상이 아니라면 이 픽셀의 점이 Box2D 안에 들어가는지 판단한다.
				// 픽셀의 월드상에서의 위치를 만들어낸다.
				Vector2	PixelPos = LeftBottom + Vector2((float)x, (float)Dest.Height - y);

				if (CollisionBox2DToPoint(SrcResult, DestResult, Src, PixelPos))
				{
					SrcResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					DestResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					return true;
				}
			}
				break;
			case PixelCollision_Type::Alpha_Confirm:
			{
				if (Dest.Pixel[Index + 3] != Dest.Color[3])
					continue;

				// 무시할 색상이 아니라면 이 픽셀의 점이 Box2D 안에 들어가는지 판단한다.
				// 픽셀의 월드상에서의 위치를 만들어낸다.
				Vector2	PixelPos = LeftBottom + Vector2((float)x, (float)Dest.Height - y);

				if (CollisionBox2DToPoint(SrcResult, DestResult, Src, PixelPos))
				{
					SrcResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					DestResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					return true;
				}
			}
				break;
			}
		}
	}

	return false;
}

bool CCollision::CollisionCircleToPixel(HitResult& SrcResult, HitResult& DestResult, 
	const CircleInfo& Src, const PixelInfo& Dest)
{
	if (!CollisionBox2DToCircle(DestResult, SrcResult, Dest.Box, Src))
		return false;

	// 교집합 사각형을 구한다.
	float	Left = Src.Min.x < Dest.Min.x ? Dest.Min.x : Src.Min.x;
	float	Right = Src.Max.x > Dest.Max.x ? Dest.Max.x : Src.Max.x;
	float	Top = Src.Max.y > Dest.Max.y ? Dest.Max.y : Src.Max.y;
	float	Bottom = Src.Min.y < Dest.Min.y ? Dest.Min.y : Src.Min.y;

	// 픽셀 충돌체의 좌 하단 좌표를 구한다.
	Vector2	LeftBottom = Dest.Box.Center - Vector2(Dest.Box.Length[0], Dest.Box.Length[1]);

	Left -= LeftBottom.x;
	Right -= LeftBottom.x;

	Top -= LeftBottom.y;
	Bottom -= LeftBottom.y;

	Left = Left < 0.f ? 0.f : Left;
	Right = Right >= (float)Dest.Width ? (float)Dest.Width - 1 : Right;

	Bottom = Bottom < 0.f ? 0.f : Bottom;
	Top = Top >= (float)Dest.Height ? (float)Dest.Height - 1 : Top;

	Top = Dest.Height - Top;
	Bottom = Dest.Height - Bottom;

	for (int y = (int)Top; y < (int)Bottom; ++y)
	{
		for (int x = (int)Left; x <= (int)Right; ++x)
		{
			int	Index = y * (int)Dest.Width * 4 + x * 4;

			switch (Dest.Type)
			{
			case PixelCollision_Type::Color_Ignore:
			{
				if (Dest.Pixel[Index] == Dest.Color[0] &&
					Dest.Pixel[Index + 1] == Dest.Color[1] &&
					Dest.Pixel[Index + 2] == Dest.Color[2])
					continue;

				// 무시할 색상이 아니라면 이 픽셀의 점이 Box2D 안에 들어가는지 판단한다.
				// 픽셀의 월드상에서의 위치를 만들어낸다.
				Vector2	PixelPos = LeftBottom + Vector2((float)x, (float)Dest.Height - y);

				if (CollisionCircleToPoint(SrcResult, DestResult, Src, PixelPos))
				{
					SrcResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					DestResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					return true;
				}
			}
			break;
			case PixelCollision_Type::Color_Confirm:
			{
				if (Dest.Pixel[Index] != Dest.Color[0] ||
					Dest.Pixel[Index + 1] != Dest.Color[1] ||
					Dest.Pixel[Index + 2] != Dest.Color[2])
					continue;

				// 무시할 색상이 아니라면 이 픽셀의 점이 Box2D 안에 들어가는지 판단한다.
				// 픽셀의 월드상에서의 위치를 만들어낸다.
				Vector2	PixelPos = LeftBottom + Vector2((float)x, (float)Dest.Height - y);

				if (CollisionCircleToPoint(SrcResult, DestResult, Src, PixelPos))
				{
					SrcResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					DestResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					return true;
				}
			}
			break;
			case PixelCollision_Type::Alpha_Ignore:
			{
				if (Dest.Pixel[Index + 3] == Dest.Color[3])
					continue;

				// 무시할 색상이 아니라면 이 픽셀의 점이 Box2D 안에 들어가는지 판단한다.
				// 픽셀의 월드상에서의 위치를 만들어낸다.
				Vector2	PixelPos = LeftBottom + Vector2((float)x, (float)Dest.Height - y);

				if (CollisionCircleToPoint(SrcResult, DestResult, Src, PixelPos))
				{
					SrcResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					DestResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					return true;
				}
			}
			break;
			case PixelCollision_Type::Alpha_Confirm:
			{
				if (Dest.Pixel[Index + 3] != Dest.Color[3])
					continue;

				// 무시할 색상이 아니라면 이 픽셀의 점이 Box2D 안에 들어가는지 판단한다.
				// 픽셀의 월드상에서의 위치를 만들어낸다.
				Vector2	PixelPos = LeftBottom + Vector2((float)x, (float)Dest.Height - y);

				if (CollisionCircleToPoint(SrcResult, DestResult, Src, PixelPos))
				{
					SrcResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					DestResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					return true;
				}
			}
			break;
			}
		}
	}

	return false;
}

bool CCollision::CollisionBox2DToPoint(HitResult& SrcResult, HitResult& DestResult, 
	const Box2DInfo& Src, const Vector2& Dest)
{
	Vector2 CenterDir = Src.Center - Dest;

	// Src의 X축으로 투영
	Vector2 Axis = Src.Axis[AXIS_X];

	float	CenterDot = abs(CenterDir.Dot(Axis));

	if (CenterDot > Src.Length[0])
		return false;

	Axis = Src.Axis[AXIS_Y];

	CenterDot = abs(CenterDir.Dot(Axis));

	if (CenterDot > Src.Length[1])
		return false;

	return true;
}

bool CCollision::CollisionCircleToPoint(HitResult& SrcResult, HitResult& DestResult, 
	const CircleInfo& Src, const Vector2& Dest)
{
	float	Dist = Src.Center.Distance(Dest);

	return Dist <= Src.Radius;
}

bool CCollision::CollisionPixelToPoint(HitResult& SrcResult,
	HitResult& DestResult, const PixelInfo& Src, const Vector2& Dest)
{
	if (Src.Min.x <= Dest.x && Dest.x <= Src.Max.x &&
		Src.Min.y <= Dest.y && Dest.y <= Src.Max.y)
	{
		int	x = (int)(Dest.x - Src.Min.x);
		int	y = (int)(Dest.y - Src.Min.y);

		int	Index = y * (int)Src.Width * 4 + x * 4;

		switch (Src.Type)
		{
		case PixelCollision_Type::Color_Ignore:
		{
			if (Src.Pixel[Index] == Src.Color[0] &&
				Src.Pixel[Index + 1] == Src.Color[1] &&
				Src.Pixel[Index + 2] == Src.Color[2])
				return false;

			SrcResult.HitPoint = Vector3(Dest.x, Dest.y, 0.f);
			DestResult.HitPoint = Vector3(Dest.x, Dest.y, 0.f);
		}
		break;
		case PixelCollision_Type::Color_Confirm:
		{
			if (Src.Pixel[Index] != Src.Color[0] ||
				Src.Pixel[Index + 1] != Src.Color[1] ||
				Src.Pixel[Index + 2] != Src.Color[2])
				return false;

			SrcResult.HitPoint = Vector3(Dest.x, Dest.y, 0.f);
			DestResult.HitPoint = Vector3(Dest.x, Dest.y, 0.f);
		}
		break;
		case PixelCollision_Type::Alpha_Ignore:
		{
			if (Src.Pixel[Index + 3] == Src.Color[3])
				return false;

			SrcResult.HitPoint = Vector3(Dest.x, Dest.y, 0.f);
			DestResult.HitPoint = Vector3(Dest.x, Dest.y, 0.f);
		}
		break;
		case PixelCollision_Type::Alpha_Confirm:
		{
			if (Src.Pixel[Index + 3] != Src.Color[3])
				return false;

			SrcResult.HitPoint = Vector3(Dest.x, Dest.y, 0.f);
			DestResult.HitPoint = Vector3(Dest.x, Dest.y, 0.f);
		}
		break;
		}

		return true;
	}

	return false;
}
