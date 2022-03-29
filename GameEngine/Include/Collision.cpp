#include "Collision.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "Component/ColliderSphere.h"
#include "Component/ColliderCube.h"
#include "GameObject.h"

float CCollision::deltaTime = 0.f;

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

bool CCollision::CollisionSpherePicking(HitResult& SrcResult, 
	CColliderSphere* Sphere, const Ray& ray)
{
	SphereInfo	Info = Sphere->GetInfo();

	Vector3	M = ray.Pos - Info.Center;

	float	b = 2.f * M.Dot(ray.Dir);
	float	c = M.Dot(M) - Info.Radius * Info.Radius;

	// ��Ʈ b^2 - 4c
	float	Det = b * b - 4.f * c;

	if (Det < 0.f)
		return false;

	Det = sqrtf(Det);

	float t1, t2;

	t1 = (-b + Det) / 2.f;
	t2 = (-b - Det) / 2.f;

	if (t1 < 0.f && t2 < 0.f)
		return false;

	float	Dist = t1 < t2 ? t1 : t2;

	SrcResult.HitPoint = ray.Pos + ray.Dir * Dist;

	return true;
}

bool CCollision::CollisionSphereToSphere(HitResult& SrcResult, HitResult& DestResult, 
	CColliderSphere* Src, CColliderSphere* Dest)
{
	if (CollisionSphereToSphere(SrcResult, DestResult, Src->GetInfo(), Dest->GetInfo()))
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

bool CCollision::CollisionCubeToCube(HitResult& SrcResult, HitResult& DestResult, CColliderCube* Src, CColliderCube* Dest)
{
	if (CollisionCubeToCube(SrcResult, DestResult, Src->GetInfo(), Dest->GetInfo()))
	{
		SrcResult.DestCollider = Dest;
		SrcResult.DestObject = Dest->GetOwner();

		DestResult.DestCollider = Src;
		DestResult.DestObject = Src->GetOwner();

		// Block
		if (true)
		{
			// ������ �غ� �װ��� ���� ������
			std::vector<Vector3> srcBottomPoint, destBottomPoint;

			Src->GetBottomPoint(srcBottomPoint);
			Dest->GetBottomPoint(destBottomPoint);

			// ������ �簢���� ���� ������� �� ���� ����
			LinePoint srcLinePoint[4];
			srcLinePoint[0].Point[0] = srcBottomPoint[0];
			srcLinePoint[0].Point[1] = srcBottomPoint[1];

			srcLinePoint[1].Point[0] = srcBottomPoint[1];
			srcLinePoint[1].Point[1] = srcBottomPoint[2];

			srcLinePoint[2].Point[0] = srcBottomPoint[2];
			srcLinePoint[2].Point[1] = srcBottomPoint[3];

			srcLinePoint[3].Point[0] = srcBottomPoint[3];
			srcLinePoint[3].Point[1] = srcBottomPoint[0];

			LinePoint destLinePoint[4];
			destLinePoint[0].Point[0] = destBottomPoint[0];
			destLinePoint[0].Point[1] = destBottomPoint[1];

			destLinePoint[1].Point[0] = destBottomPoint[1];
			destLinePoint[1].Point[1] = destBottomPoint[2];

			destLinePoint[2].Point[0] = destBottomPoint[2];
			destLinePoint[2].Point[1] = destBottomPoint[3];

			destLinePoint[3].Point[0] = destBottomPoint[3];
			destLinePoint[3].Point[1] = destBottomPoint[0];

			std::vector<Vector2> vecIntersectPoint;
			std::vector<LinePoint> vecSrcHitline;
			std::vector<LinePoint> vecDestHitline;

			// Src�� Dest�� ���е� �� �������� �Ǵ� (�� ���߿� �ü� ���Ϳ� ������ �������� �ٲٱ�)
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					Vector2 IntersectPoint;

					// Vector3 -> Vector2 �� ����
					Vector2 PointSrc1 = srcLinePoint[i].GetPointVector3ToVector2()[0];
					Vector2 PointSrc2 = srcLinePoint[i].GetPointVector3ToVector2()[1];
					Vector2 PointDest1 = destLinePoint[j].GetPointVector3ToVector2()[0];
					Vector2 PointDest2 = destLinePoint[j].GetPointVector3ToVector2()[1];

					if (Vector2::GetIntersectPoint(PointSrc1, PointSrc2, PointDest1, PointDest2, IntersectPoint))
					{
						// ��Ʈ ����Ʈ ����
						vecIntersectPoint.push_back(IntersectPoint);

						bool srcLineSave = true;
						bool destLineSave = true;

						// �ε�ģ ������ ������ ����
						if (vecSrcHitline.size() > 0)
						{
							for (size_t index = 0; index < vecSrcHitline.size(); ++index)
							{
								// ���� �ȿ� �̹� ������ ����Ǿ� ���� ��� �ش� ������ ��ø (������ �ʿ� x)
								if (vecSrcHitline[index].Point[0] == srcLinePoint[i].Point[0] &&
									vecSrcHitline[index].Point[1] == srcLinePoint[i].Point[1])
								{
									srcLineSave = false;
									break;
								}
							}
						}

						if (vecDestHitline.size() > 0)
						{
							for (size_t index = 0; index < vecDestHitline.size(); ++index)
							{
								if (vecDestHitline[index].Point[0] == destLinePoint[j].Point[0] &&
									vecDestHitline[index].Point[1] == destLinePoint[j].Point[1])
								{
									// �ϳ��� ��ġ�� ��� 
									destLineSave = false;
									break;
								}
							}
						}

						if (srcLineSave)
							vecSrcHitline.push_back(srcLinePoint[i]);

						if (destLineSave)
							vecDestHitline.push_back(destLinePoint[j]);
					}
				}
			}

			std::vector<Vector3> vecPlaneNormal;
			std::vector<LinePoint> vecDetectedLine;

			for (int i = 0; i < vecSrcHitline.size(); ++i)
			{
				// �ε�ģ ������ �ٶ󺸴� ������ ����.
				// �ٴ� ���� �� ���� �������� ���� �ٶ󺸴� ������ ���ؼ� ����
				Vector3 TopHorizontalLine = vecSrcHitline[i].Point[1] - vecSrcHitline[i].Point[0];
				Vector3 TopStartPoint = vecSrcHitline[i].Point[0];
				TopStartPoint.y += Src->GetInfo().Length[AXIS_Y] * 2;			// Y�� �÷���

				// ���� ���� ���� (��� ���������� ���� �������� ���� ����)
				Vector3 VerticalLine = vecSrcHitline[i].Point[0] - TopStartPoint;
				Vector3 PlaneNormal = TopHorizontalLine.Cross(VerticalLine);
				PlaneNormal.Normalize();

				vecPlaneNormal.push_back(PlaneNormal);
			}


			// Ű�Է��� ������ ���� ���¿��� �и��� ������ �ذ��ϱ�
			// �÷��̾��� ���� ���� (�Ի� ����)
			if (vecPlaneNormal.size() > 0)
			{
				Vector3 DestFowardVector = Dest->GetOwner()->GetAxis(AXIS_Z);
				//DestFowardVector.Normalize();
				Vector3 PlaneNormal = vecPlaneNormal[0];

				Vector3 ProjVector = PlaneNormal * (DestFowardVector.Dot(PlaneNormal));

				Vector3 SlidingVector = DestFowardVector + PlaneNormal * (-DestFowardVector.Dot(PlaneNormal));
				Vector3 SlidingPos = Dest->GetPrevWorldPos() + (SlidingVector * deltaTime * 3.f);
				Dest->GetOwner()->SetWorldPos(SlidingPos);

			}

			
			return true;

		}

		SrcResult.DestCollider = Dest;
		SrcResult.DestObject = Dest->GetOwner();

		DestResult.DestCollider = Src;
		DestResult.DestObject = Src->GetOwner();

		return false;
	}
}

bool CCollision::CollisionSphereToCube(HitResult& SrcResult, HitResult& DestResult, CColliderSphere* Src, CColliderCube* Dest)
{
	return false;
}

bool CCollision::CollisionBox2DToBox2D(HitResult& SrcResult, HitResult& DestResult, 
	const Box2DInfo& Src, const Box2DInfo& Dest)
{
	Vector2 CenterDir = Src.Center - Dest.Center;

	// Src�� X������ ����
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

	// Src�� X������ ����
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

	// ������ �簢���� ���Ѵ�.
	float	Left = Src.Min.x < Dest.Min.x ? Dest.Min.x : Src.Min.x;
	float	Right = Src.Max.x > Dest.Max.x ? Dest.Max.x : Src.Max.x;
	float	Top = Src.Max.y > Dest.Max.y ? Dest.Max.y : Src.Max.y;
	float	Bottom = Src.Min.y < Dest.Min.y ? Dest.Min.y : Src.Min.y;

	// �ȼ� �浹ü�� �� �ϴ� ��ǥ�� ���Ѵ�.
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

				// ������ ������ �ƴ϶�� �� �ȼ��� ���� Box2D �ȿ� ������ �Ǵ��Ѵ�.
				// �ȼ��� ����󿡼��� ��ġ�� ������.
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

				// ������ ������ �ƴ϶�� �� �ȼ��� ���� Box2D �ȿ� ������ �Ǵ��Ѵ�.
				// �ȼ��� ����󿡼��� ��ġ�� ������.
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

				// ������ ������ �ƴ϶�� �� �ȼ��� ���� Box2D �ȿ� ������ �Ǵ��Ѵ�.
				// �ȼ��� ����󿡼��� ��ġ�� ������.
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

				// ������ ������ �ƴ϶�� �� �ȼ��� ���� Box2D �ȿ� ������ �Ǵ��Ѵ�.
				// �ȼ��� ����󿡼��� ��ġ�� ������.
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

	// ������ �簢���� ���Ѵ�.
	float	Left = Src.Min.x < Dest.Min.x ? Dest.Min.x : Src.Min.x;
	float	Right = Src.Max.x > Dest.Max.x ? Dest.Max.x : Src.Max.x;
	float	Top = Src.Max.y > Dest.Max.y ? Dest.Max.y : Src.Max.y;
	float	Bottom = Src.Min.y < Dest.Min.y ? Dest.Min.y : Src.Min.y;

	// �ȼ� �浹ü�� �� �ϴ� ��ǥ�� ���Ѵ�.
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

				// ������ ������ �ƴ϶�� �� �ȼ��� ���� Box2D �ȿ� ������ �Ǵ��Ѵ�.
				// �ȼ��� ����󿡼��� ��ġ�� ������.
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

				// ������ ������ �ƴ϶�� �� �ȼ��� ���� Box2D �ȿ� ������ �Ǵ��Ѵ�.
				// �ȼ��� ����󿡼��� ��ġ�� ������.
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

				// ������ ������ �ƴ϶�� �� �ȼ��� ���� Box2D �ȿ� ������ �Ǵ��Ѵ�.
				// �ȼ��� ����󿡼��� ��ġ�� ������.
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

				// ������ ������ �ƴ϶�� �� �ȼ��� ���� Box2D �ȿ� ������ �Ǵ��Ѵ�.
				// �ȼ��� ����󿡼��� ��ġ�� ������.
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

	// Src�� X������ ����
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

bool CCollision::CollisionSphereToSphere(HitResult& SrcResult, HitResult& DestResult, 
	const SphereInfo& Src, const SphereInfo& Dest)
{
	float	Distance = Src.Center.Distance(Dest.Center);

	bool	Result = Distance <= Src.Radius + Dest.Radius;

	if (Result)
	{
		Vector3	Center = (Src.Center + Dest.Center) / 2.f;
		SrcResult.HitPoint = Center;
		DestResult.HitPoint = Center;
	}

	return Result;
}

bool CCollision::CollisionCubeToCube(HitResult& SrcResult, HitResult& DestResult, const CubeInfo& Src, const CubeInfo& Dest)
{
	// ���⿡ 3D obb�� �����ϱ�
	Vector3 CenterDir = Src.Center - Dest.Center;

	// �и��� ���ϱ�
	return !(GetSeparatingPlane(CenterDir, Src.Axis[AXIS_X], Src, Dest) ||
		GetSeparatingPlane(CenterDir, Src.Axis[AXIS_Y], Src, Dest) ||
		GetSeparatingPlane(CenterDir, Src.Axis[AXIS_Z], Src, Dest) ||
		GetSeparatingPlane(CenterDir, Dest.Axis[AXIS_X], Src, Dest) ||
		GetSeparatingPlane(CenterDir, Dest.Axis[AXIS_Y], Src, Dest) ||
		GetSeparatingPlane(CenterDir, Dest.Axis[AXIS_Z], Src, Dest) ||
		GetSeparatingPlane(CenterDir, Src.Axis[AXIS_X].Cross(Dest.Axis[AXIS_X]), Src, Dest) ||
		GetSeparatingPlane(CenterDir, Src.Axis[AXIS_X].Cross(Dest.Axis[AXIS_Y]), Src, Dest) ||
		GetSeparatingPlane(CenterDir, Src.Axis[AXIS_X].Cross(Dest.Axis[AXIS_Z]), Src, Dest) ||
		GetSeparatingPlane(CenterDir, Src.Axis[AXIS_Y].Cross(Dest.Axis[AXIS_X]), Src, Dest) ||
		GetSeparatingPlane(CenterDir, Src.Axis[AXIS_Y].Cross(Dest.Axis[AXIS_Y]), Src, Dest) ||
		GetSeparatingPlane(CenterDir, Src.Axis[AXIS_Y].Cross(Dest.Axis[AXIS_Z]), Src, Dest) ||
		GetSeparatingPlane(CenterDir, Src.Axis[AXIS_Z].Cross(Dest.Axis[AXIS_X]), Src, Dest) ||
		GetSeparatingPlane(CenterDir, Src.Axis[AXIS_Z].Cross(Dest.Axis[AXIS_Y]), Src, Dest) ||
		GetSeparatingPlane(CenterDir, Src.Axis[AXIS_Z].Cross(Dest.Axis[AXIS_Z]), Src, Dest));
}

bool CCollision::CollisionSphereToCube(HitResult& SrcResult, HitResult& DestResult, SphereInfo* Src, CubeInfo* Dest)
{
	//Vector3 CenterPos = Src->Center;

	//Src->Center.Axis[AXIS_X].x
	//CenterPos.Axis[AXIS_X].x = Src->GetOwner()->GetAxis(AXIS_X).x;
	//CenterPos.Axis[AXIS_X].y = GetAxis(AXIS_X).y;
	//CenterPos.Axis[AXIS_X].z = GetAxis(AXIS_X).z;


	//m_Info.Axis[AXIS_Y].x = GetAxis(AXIS_Y).x;
	//m_Info.Axis[AXIS_Y].y = GetAxis(AXIS_Y).y;
	//m_Info.Axis[AXIS_Y].z = GetAxis(AXIS_Y).z;

	//m_Info.Axis[AXIS_Z].x = GetAxis(AXIS_Z).x;
	//m_Info.Axis[AXIS_Z].y = GetAxis(AXIS_Z).y;
	//m_Info.Axis[AXIS_Z].z = GetAxis(AXIS_Z).z;

	//Vector3   Pos[8] = {};

	//// ���� 4���� �� �ݽð� ����
	//Pos[0] = m_Info.Center + m_Info.Axis[AXIS_X] * m_Info.Length[0]
	//	+ m_Info.Axis[AXIS_Y] * m_Info.Length[1]
	//	- m_Info.Axis[AXIS_Z] * m_Info.Length[2];
	//Pos[1] = m_Info.Center - m_Info.Axis[AXIS_X] * m_Info.Length[0]
	//	+ m_Info.Axis[AXIS_Y] * m_Info.Length[1]
	//	- m_Info.Axis[AXIS_Z] * m_Info.Length[2];
	//Pos[2] = m_Info.Center - m_Info.Axis[AXIS_X] * m_Info.Length[0]
	//	- m_Info.Axis[AXIS_Y] * m_Info.Length[1]
	//	- m_Info.Axis[AXIS_Z] * m_Info.Length[2];
	//Pos[3] = m_Info.Center + m_Info.Axis[AXIS_X] * m_Info.Length[0]
	//	- m_Info.Axis[AXIS_Y] * m_Info.Length[1]
	//	- m_Info.Axis[AXIS_Z] * m_Info.Length[2];

	//Pos[4] = m_Info.Center + m_Info.Axis[AXIS_X] * m_Info.Length[0]
	//	+ m_Info.Axis[AXIS_Y] * m_Info.Length[1]
	//	+ m_Info.Axis[AXIS_Z] * m_Info.Length[2];
	//Pos[5] = m_Info.Center - m_Info.Axis[AXIS_X] * m_Info.Length[0]
	//	+ m_Info.Axis[AXIS_Y] * m_Info.Length[1]
	//	+ m_Info.Axis[AXIS_Z] * m_Info.Length[2];
	//Pos[6] = m_Info.Center - m_Info.Axis[AXIS_X] * m_Info.Length[0]
	//	- m_Info.Axis[AXIS_Y] * m_Info.Length[1]
	//	+ m_Info.Axis[AXIS_Z] * m_Info.Length[2];
	//Pos[7] = m_Info.Center + m_Info.Axis[AXIS_X] * m_Info.Length[0]
	//	- m_Info.Axis[AXIS_Y] * m_Info.Length[1]
	//	+ m_Info.Axis[AXIS_Z] * m_Info.Length[2];


	return false;

	return false;
}

bool CCollision::GetSeparatingPlane(const Vector3& CenterDir, const Vector3 Plane, const CubeInfo& Src, const CubeInfo& Dest)
{
	float CenterDot = abs(CenterDir.Dot(Plane));

	return (CenterDot >
		(abs((Src.Axis[AXIS_X].Dot(Plane)) * Src.Length[AXIS_X]) +
		abs((Src.Axis[AXIS_Y].Dot(Plane)) * Src.Length[AXIS_Y]) +
		abs((Src.Axis[AXIS_Z].Dot(Plane)) * Src.Length[AXIS_Z]) +
		abs((Dest.Axis[AXIS_X].Dot(Plane)) * Dest.Length[AXIS_X]) +
		abs((Dest.Axis[AXIS_Y].Dot(Plane)) * Dest.Length[AXIS_Y]) +
		abs((Dest.Axis[AXIS_Z].Dot(Plane)) * Dest.Length[AXIS_Z])));
}
