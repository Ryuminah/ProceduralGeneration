
#include "BoneSocket.h"

CBoneSocket::CBoneSocket()
{
}

CBoneSocket::CBoneSocket(const CBoneSocket& socket)
{
	*this = socket;
}

CBoneSocket::~CBoneSocket()
{
}

void CBoneSocket::Update(const Matrix& Bone)
{
	Matrix	matRot, matTrans;

	matRot.Rotation(m_OffsetRot);
	matTrans.Translation(m_Offset);

	m_matSocket = matRot * matTrans * Bone;
}

CBoneSocket* CBoneSocket::Clone()
{
	return new CBoneSocket(*this);
}
