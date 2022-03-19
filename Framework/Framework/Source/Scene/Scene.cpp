#include "PCH.h"
#include "Scene.h"

#include "Graphics/Graphics.h"
#include "Graphics/DX11/DX11Context.h"
#include "Graphics/DX11/Resource/Shader.h"

Scene::Scene(const std::string& strName, Graphics* pGFX) :
	m_strName(strName)
{
	m_pGFX = pGFX;
}

void Scene::StartUp()
{
	m_pShader = new Shader(L"DefaultVS", EShaderType::VERTEX_SHADER);
	m_pShader->LoadShader();
}

void Scene::CleanUp()
{

}

void Scene::Update()
{

}

void Scene::Render()
{
	static std::default_random_engine randomEngine;
	std::uniform_real_distribution<FLOAT> randNum(0.0f);

	static DWORD dwLocalTime = ::timeGetTime();
	static DWORD elapsedTime = 0;
	elapsedTime += (::timeGetTime() - dwLocalTime);
	dwLocalTime = ::timeGetTime();

	DirectX::XMFLOAT4 clearColor;
	::ZeroMemory(&clearColor, sizeof(DirectX::XMFLOAT4));

	if (elapsedTime > 1000)
	{
		elapsedTime = 0;

		clearColor.x += randNum(randomEngine);
		if (clearColor.x > 1.0f)
		{
			clearColor.x = 0.0f;
		}

		clearColor.y += randNum(randomEngine);
		if (clearColor.y > 1.0f)
		{
			clearColor.y = 0.0f;
		}

		clearColor.z += randNum(randomEngine);
		if (clearColor.z > 1.0f)
		{
			clearColor.z = 0.0f;
		}

		m_pGFX->GetContext()->SetClearColor(clearColor);
	}
}