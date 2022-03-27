#include "PCH.h"
#include "Scene.h"

#include "GameObject/GameObject2D.h"
#include "GameObject/TransformComponent.h"

Scene::Scene(const std::string& strName, Graphics* pGfx) :
	m_strName(strName)
{
	m_pGfx = pGfx;
}

void Scene::StartUp()
{
	spGameObject2D = std::make_shared<GameObject2D>("KirbyTitle");
	spGameObject2D->StartUp({ 400.0f, 400.0f });
	spGameObject2D->GetTransform()->SetPosition({ -400.0f, -200.0f });
	spGameObject2D->SetTexture2D("KirbyTitle");

	spGameObject2D2 = std::make_shared<GameObject2D>("KirbyAlpha");
	spGameObject2D2->StartUp({ 400.0f, 400.0f });
	spGameObject2D2->GetTransform()->SetPosition({ 400.0f, 200.0f });
	spGameObject2D2->SetTexture2D("KirbyAlpha");

	spGameObject2D3 = std::make_shared<GameObject2D>("KirbyTitle");
	spGameObject2D3->StartUp({ 400.0f, 400.0f });
	spGameObject2D3->GetTransform()->SetPosition({ -200.0f, 200.0f });
	spGameObject2D3->SetTexture2D("KirbyAlpha");
}

void Scene::Update()
{
	spGameObject2D->Update();
	spGameObject2D2->Update();
	spGameObject2D3->Update();
}

void Scene::Render()
{
	spGameObject2D->Render(m_pGfx);
	spGameObject2D2->Render(m_pGfx);
	spGameObject2D3->Render(m_pGfx);
}

void Scene::CleanUp()
{
	spGameObject2D->CleanUp();
	spGameObject2D2->CleanUp();
	spGameObject2D3->CleanUp();
}