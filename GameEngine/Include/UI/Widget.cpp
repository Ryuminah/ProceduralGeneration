
#include "Widget.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/UITransformConstantBuffer.h"
#include "../Scene/Viewport.h"
#include "../Component/Camera.h"
#include "WidgetWindow.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneCollision.h"
#include "../Component/WidgetComponent.h"
#include "../Scene/CameraManager.h"
#include "../Device.h"

CWidget::CWidget() :
	m_Viewport(nullptr),
	m_Owner(nullptr),
	m_Scene(nullptr),
	m_Rotation(0.f),
	m_Size(100.f, 50.f),
	m_ZOrder(0),
	m_Start(false),
	m_WorldZ(0.f)
{
}

CWidget::CWidget(const CWidget& widget)
{
	m_Viewport = widget.m_Viewport;
	m_Mesh = widget.m_Mesh;
	m_Shader = widget.m_Shader;
	m_Pos = widget.m_Pos;
	m_Rotation = widget.m_Rotation;
	m_Size = widget.m_Size;

	if (widget.m_TransformCBuffer)
		m_TransformCBuffer = widget.m_TransformCBuffer->Clone();

	if (widget.m_TintCBuffer)
		m_TintCBuffer = widget.m_TintCBuffer->Clone();
}

CWidget::~CWidget()
{
	SAFE_DELETE(m_TransformCBuffer);
	SAFE_DELETE(m_TintCBuffer);
}

void CWidget::Start()
{
	m_Start = true;

	if (m_Scene)
		m_Scene->GetCollisionManager()->AddWidget(this);
}

bool CWidget::Init()
{
	m_Mesh = (CSpriteMesh*)CResourceManager::GetInst()->FindMesh("TextureRect");

	m_TransformCBuffer = new CUITransformConstantBuffer;

	m_TransformCBuffer->Init();

	m_TintCBuffer = new CUIColorTintCBuffer;

	m_TintCBuffer->Init();

	return true;
}

void CWidget::Update(float DeltaTime)
{
}

void CWidget::PostUpdate(float DeltaTime)
{
	Matrix	matScale, matRot, matTranslation, matWorld;

	m_RenderPos = m_Pos;
	
	if (m_Owner)
		m_RenderPos += m_Owner->GetPos();

	matScale.Scaling(m_Size.x, m_Size.y, 1.f);
	matRot.Rotation(0.f, 0.f, m_Rotation);

	if (m_Owner)
	{
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
	}

	matTranslation.Translation(m_RenderPos.x, m_RenderPos.y, m_WorldZ);

	matWorld = matScale * matRot * matTranslation;

	Matrix	matProj;

	if (m_Viewport)
		matProj = m_Viewport->GetCamera()->GetProjMatrix();

	else
		matProj = CSceneManager::GetInst()->GetScene()->GetViewport()->GetCamera()->GetProjMatrix();

	m_TransformCBuffer->SetWVP(matWorld * matProj);
}

void CWidget::Render()
{
	m_TransformCBuffer->SetUISize(m_Size);
	m_TransformCBuffer->UpdateCBuffer();

	m_TintCBuffer->UpdateCBuffer();

	m_Shader->SetShader();
}

CWidget* CWidget::Clone()
{
	return nullptr;
}

void CWidget::SetZOrder(int ZOrder)
{
	m_ZOrder = ZOrder;

	if (m_Owner)
		m_ZOrder += m_Owner->GetZOrder();
}

void CWidget::CollisionMouse(const Vector2& MousePos, float DeltaTime)
{
}

void CWidget::CollisionReleaseMouse(const Vector2& MousePos, float DeltaTime)
{
}

void CWidget::UpdateWorldTransformZ()
{
	Matrix	matScale, matRot, matTranslation, matWorld;

	Vector2	RenderPos = m_Pos + m_Owner->GetPos();

	CCamera* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();

	float	ConvertY = m_RenderPos.y + Camera->GetWorldPos().y;

	ConvertY -= Camera->GetCameraBottom();

	Resolution  RS = CDevice::GetInst()->GetResolution();

	float   Ratio = (ConvertY - 1.f) / (RS.Height * 2.f);

	m_WorldZ = (0.69999f - 0.3f) * Ratio + 0.3f;

	matScale.Scaling(m_Size.x, m_Size.y, 1.f);
	matRot.Rotation(0.f, 0.f, m_Rotation);
	matTranslation.Translation(RenderPos.x, RenderPos.y, m_WorldZ);

	matWorld = matScale * matRot * matTranslation;

	Matrix	matProj = m_Viewport->GetCamera()->GetProjMatrix();

	m_TransformCBuffer->SetWVP(matWorld * matProj);
}

void CWidget::CallAnimNotify(const std::string& Name)
{
	if (m_Owner)
		m_Owner->CallAnimNotify(Name);
}
