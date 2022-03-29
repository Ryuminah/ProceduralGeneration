
#include "NumberWidget.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Shader.h"
#include "../Resource/UITransformConstantBuffer.h"
#include "../Scene/Viewport.h"
#include "WidgetWindow.h"
#include "../Component/WidgetComponent.h"
#include "../Scene/Scene.h"
#include "../Device.h"
#include "../Component/Camera.h"
#include "../Scene/CameraManager.h"
#include <stack>

CNumberWidget::CNumberWidget() :
	m_Number(0),
	m_NumberSize(30.f, 50.f),
	m_NumberCBuffer(nullptr),
	m_FrameCountX(10),
	m_FrameCountY(1),
	m_SelectYFrame(0)
{
}

CNumberWidget::CNumberWidget(const CNumberWidget& widget) :
	CWidget(widget)
{
	m_Texture = widget.m_Texture;
	m_Number = widget.m_Number;
	m_NumberSize = widget.m_NumberSize;
	m_FrameCountX = widget.m_FrameCountX;
	m_FrameCountY = widget.m_FrameCountY;
	m_SelectYFrame = widget.m_SelectYFrame;

	if (widget.m_NumberCBuffer)
		m_NumberCBuffer = widget.m_NumberCBuffer->Clone();
}

CNumberWidget::~CNumberWidget()
{
	SAFE_DELETE(m_NumberCBuffer);
}

void CNumberWidget::SetTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
		return;

	m_Texture = m_Scene->GetResource()->FindTexture(Name);

	if (m_Texture)
		m_TransformCBuffer->SetTextureEnable(true);

	else
		m_TransformCBuffer->SetTextureEnable(false);
}

bool CNumberWidget::Init()
{
	CWidget::Init();

	m_Shader = CShaderManager::GetInst()->FindShader("NumberShader");

	m_NumberCBuffer = new CNumberCBuffer;

	m_NumberCBuffer->Init();

	return true;
}

void CNumberWidget::Update(float DeltaTime)
{
	CWidget::Update(DeltaTime);

	// 예를들어 1234 가 있다면 가장 오른쪽 숫자부터 차례로 하나씩 먼저 얻어온다.
	int	Number = m_Number;

	std::stack<int>	stackNumber;

	while (Number > 0)
	{
		stackNumber.push(Number % 10);
		Number /= 10;
	}

	m_vecNumber.clear();

	while (!stackNumber.empty())
	{
		m_vecNumber.push_back(stackNumber.top());
		stackNumber.pop();
	}

	if (m_vecNumber.empty())
		m_vecNumber.push_back(0);
}

void CNumberWidget::PostUpdate(float DeltaTime)
{
	CWidget::PostUpdate(DeltaTime);
}

void CNumberWidget::Render()
{
	if (m_Texture)
	{
		m_Texture->SetShader(0, TST_PIXEL);

		for (int i = 0; i < m_vecNumber.size(); ++i)
		{
			Vector2	StartUV, EndUV;

			unsigned int Height = m_Texture->GetHeight();

			StartUV.x = m_vecNumber[i] / 10.f;
			StartUV.y = m_SelectYFrame / (float)m_FrameCountY;

			EndUV.x = StartUV.x + 0.1f;
			EndUV.y = (m_SelectYFrame + 1) / (float)m_FrameCountY;

			m_NumberCBuffer->SetNumberUV(StartUV, EndUV);

			m_NumberCBuffer->UpdateCBuffer();

			UpdateNumberTransform(i);

			m_Mesh->Render();
		}

		m_Texture->ResetShader(0, TST_PIXEL);
	}
}

CNumberWidget* CNumberWidget::Clone()
{
	return new CNumberWidget(*this);
}

void CNumberWidget::UpdateNumberTransform(int Index)
{
	Matrix	matScale, matRot, matTranslation, matWorld;

	Vector2	Pos = m_Pos;
	Pos.x += m_NumberSize.x * Index;

	m_RenderPos = Pos + m_Owner->GetPos();

	matScale.Scaling(m_NumberSize.x, m_NumberSize.y, 1.f);
	matRot.Rotation(0.f, 0.f, m_Rotation);

	if (m_Owner->GetWidgetComponent())
	{
		if (m_Owner->GetWidgetComponent()->GetWidgetSpace() == WidgetComponent_Space::Space2D)
		{
			CCamera* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();

			float	ConvertY = m_RenderPos.y + Camera->GetWorldPos().y;

			ConvertY -= Camera->GetCameraBottom();

			Resolution  RS = CDevice::GetInst()->GetResolution();

			float   Ratio = ConvertY / (RS.Height * 2.f);

			m_WorldZ = (0.69999f - 0.3f) * Ratio + 0.3f;
		}

		else
		{
		}
	}

	matTranslation.Translation(m_RenderPos.x, m_RenderPos.y, m_WorldZ);

	matWorld = matScale * matRot * matTranslation;

	Matrix	matProj = m_Viewport->GetCamera()->GetProjMatrix();

	m_TransformCBuffer->SetWVP(matWorld * matProj);


	m_TransformCBuffer->UpdateCBuffer();

	m_TintCBuffer->UpdateCBuffer();

	m_Shader->SetShader();
}