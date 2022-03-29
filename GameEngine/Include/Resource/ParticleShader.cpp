
#include "ParticleShader.h"

CParticleShader::CParticleShader()
{
}

CParticleShader::~CParticleShader()
{
}

bool CParticleShader::Init()
{
    if (!LoadVertexShader(m_Name, "ParticleVS", TEXT("Particle.fx"), 
        SHADER_PATH))
        return false;

    if (!LoadGeometryShader(m_Name, "ParticleGS", TEXT("Particle.fx"),
        SHADER_PATH))
        return false;

    if (!LoadPixelShader(m_Name, "ParticlePS", TEXT("Particle.fx"),
        SHADER_PATH))
        return false;

    AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, 
        D3D11_INPUT_PER_VERTEX_DATA, 0);

    if (!CreateInputLayout())
        return false;

    return true;
}
