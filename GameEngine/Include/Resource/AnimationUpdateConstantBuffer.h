#pragma once
#include "ConstantBufferData.h"
class CAnimationUpdateConstantBuffer :
    public CConstantBufferData
{
public:
	CAnimationUpdateConstantBuffer();
	CAnimationUpdateConstantBuffer(const CAnimationUpdateConstantBuffer& buffer);
	virtual ~CAnimationUpdateConstantBuffer();

protected:
	AnimationCBuffer    m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CAnimationUpdateConstantBuffer* Clone();

public:	
	void SetBoneCount(int Count)
	{
		m_BufferData.BoneCount = Count;
	}

	void SetCurrentFrame(int CurrentFrame)
	{
		m_BufferData.CurrentFrame = CurrentFrame;
	}

	void SetNextFrame(int NextFrame)
	{
		m_BufferData.NextFrame = NextFrame;
	}

	void SetRatio(float Ratio)
	{
		m_BufferData.Ratio = Ratio;
	}

	void SetFrameCount(int FrameCount)
	{
		m_BufferData.FrameCount = FrameCount;
	}

	void SetRowIndex(int RowIndex)
	{
		m_BufferData.RowIndex = RowIndex;
	}

	void SetChangeAnimation(bool Change)
	{
		m_BufferData.ChangeAnimation = Change;
	}

	void SetChangeRatio(float ChangeRatio)
	{
		m_BufferData.ChangeRatio = ChangeRatio;
	}

	void SetChangeFrameCount(int ChangeFrameCount)
	{
		m_BufferData.ChangeFrameCount = ChangeFrameCount;
	}
};

