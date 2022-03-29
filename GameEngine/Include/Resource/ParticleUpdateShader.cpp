
#include "ParticleUpdateShader.h"

CParticleUpdateShader::CParticleUpdateShader()
{
}

CParticleUpdateShader::~CParticleUpdateShader()
{
}

bool CParticleUpdateShader::Init()
{
    if (!LoadComputeShader(m_Name, "ParticleUpdate", TEXT("Particle.fx"), SHADER_PATH))
        return false;

    return true;
}
