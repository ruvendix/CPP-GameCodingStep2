#include "PCH.h"
#include "Scene.h"

#include "GameObject/GameObject2D.h"

Scene::Scene(const std::string& strName, Graphics* pGfx) :
	m_strName(strName)
{
	m_pGfx = pGfx;
}

void Scene::StartUp()
{
	spGameObject2D = std::make_shared<GameObject2D>("Kirby");
	spGameObject2D->StartUp();
	//spGameObject2D->SetTexture2D("KirbyTitle");
}

void Scene::Update()
{
	spGameObject2D->Update(m_pGfx);
}

void Scene::Render()
{
	spGameObject2D->Render();
}

void Scene::CleanUp()
{
	spGameObject2D->CleanUp();
}