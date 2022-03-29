#pragma once

#include "GameEngine.h"

class CCollision
{
public:
	static float deltaTime;

public:
	static bool CollisionBox2DToBox2D(HitResult& SrcResult, HitResult& DestResult,
		class CColliderBox2D* Src, class CColliderBox2D* Dest);
	static bool CollisionCircleToCircle(HitResult& SrcResult, HitResult& DestResult,
		class CColliderCircle* Src, class CColliderCircle* Dest);
	static bool CollisionBox2DToCircle(HitResult& SrcResult, HitResult& DestResult,
		class CColliderBox2D* Src, class CColliderCircle* Dest);
	static bool CollisionBox2DToPixel(HitResult& SrcResult, HitResult& DestResult,
		class CColliderBox2D* Src, class CColliderPixel* Dest);
	static bool CollisionCircleToPixel(HitResult& SrcResult, HitResult& DestResult,
		class CColliderCircle* Src, class CColliderPixel* Dest);
	static bool CollisionSpherePicking(HitResult& SrcResult,
		class CColliderSphere* Sphere, const Ray& ray);
	static bool CollisionSphereToSphere(HitResult& SrcResult, HitResult& DestResult,
		class CColliderSphere* Src, class CColliderSphere* Dest);
	static bool CollisionCubeToCube(HitResult& SrcResult, HitResult& DestResult,
		class CColliderCube* Src, class CColliderCube* Dest);
	static bool CollisionSphereToCube(HitResult& SrcResult, HitResult& DestResult,
		class CColliderSphere* Src, class CColliderCube* Dest);


public:
	static bool CollisionBox2DToBox2D(HitResult& SrcResult, HitResult& DestResult, 
		const Box2DInfo& Src, const Box2DInfo& Dest);
	static bool CollisionCircleToCircle(HitResult& SrcResult, HitResult& DestResult,
		const CircleInfo& Src, const CircleInfo& Dest);
	static bool CollisionBox2DToCircle(HitResult& SrcResult, HitResult& DestResult,
		const Box2DInfo& Src, const CircleInfo& Dest);
	static bool CollisionBox2DToPixel(HitResult& SrcResult, HitResult& DestResult,
		const Box2DInfo& Src, const PixelInfo& Dest);
	static bool CollisionCircleToPixel(HitResult& SrcResult, HitResult& DestResult,
		const CircleInfo& Src, const PixelInfo& Dest);
	static bool CollisionBox2DToPoint(HitResult& SrcResult, HitResult& DestResult,
		const Box2DInfo& Src, const Vector2& Dest);
	static bool CollisionCircleToPoint(HitResult& SrcResult, HitResult& DestResult,
		const CircleInfo& Src, const Vector2& Dest);
	static bool CollisionPixelToPoint(HitResult& SrcResult, HitResult& DestResult,
		const PixelInfo& Src, const Vector2& Dest);
	static bool CollisionSphereToSphere(HitResult& SrcResult, HitResult& DestResult,
		const SphereInfo& Src, const SphereInfo& Dest);
	static bool CollisionCubeToCube(HitResult& SrcResult, HitResult& DestResult,
		const CubeInfo& Src, const CubeInfo& Dest);
	static bool CollisionSphereToCube(HitResult& SrcResult, HitResult& DestResult,
		SphereInfo* Src, CubeInfo* Dest);


private:
	static bool GetSeparatingPlane(const Vector3& CenterDir, const Vector3 Plane, const CubeInfo& Src, const CubeInfo& Dest);
};

