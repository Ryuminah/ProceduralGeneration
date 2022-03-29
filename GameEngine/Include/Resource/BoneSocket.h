#pragma once

#include "../GameEngine.h"

class CBoneSocket
{
	friend class CSkeleton;
	friend class CAnimation;

private:
	CBoneSocket();
	CBoneSocket(const CBoneSocket& socket);
	~CBoneSocket();

private:
	std::string		m_Name;
	std::string		m_BoneName;
	int				m_BoneIndex;
	Vector3			m_Offset;
	Vector3			m_OffsetRot;
	Matrix			m_matSocket;

public:
	const std::string& GetName()	const
	{
		return m_Name;
	}

	const std::string& GetBoneName()	const
	{
		return m_BoneName;
	}

	int GetBoneIndex()	const
	{
		return m_BoneIndex;
	}

	const Matrix& GetSocketMatrix()	const
	{
		return m_matSocket;
	}

public:
	void SetOffset(float x, float y, float z)
	{
		m_Offset = Vector3(x, y, z);
	}

	void SetOffset(const Vector3& Offset)
	{
		m_Offset = Offset;
	}

	void SetOffsetRotation(float x, float y, float z)
	{
		m_OffsetRot = Vector3(x, y, z);
	}

	void SetOffsetRotation(const Vector3& Offset)
	{
		m_OffsetRot = Offset;
	}

public:
	void Update(const Matrix& Bone);
	CBoneSocket* Clone();
};



