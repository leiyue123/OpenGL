#include "ExampleLayer.h"

using namespace GLCore;
using namespace GLCore::Utils;

ExampleLayer::ExampleLayer()
	: m_CameraController(16.0f / 9.0f)
{
	ParticSystem::Init();
}

ExampleLayer::~ExampleLayer()
{
	ParticSystem::End();
}

void ExampleLayer::OnAttach()
{
	EnableGLDebugging();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	
}

void ExampleLayer::OnDetach()
{
	
}

void ExampleLayer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);

	GLCore::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& e)
	{
		glViewport(0, 0, e.GetWidth(), e.GetHeight());
		return false;
	});
}

void ExampleLayer::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (GLCore::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
	{
		int mouseX = GLCore::Input::GetMouseX();
		int mouseY = GLCore::Input::GetMouseY();
		int width = Application::Get().GetWindow().GetWidth();
		int height = Application::Get().GetWindow().GetHeight();
		glm::vec3 camerapos = m_CameraController.GetCamera().GetPosition();
		float cameraViewHalfWidth = m_CameraController.GetZoomLevel() * (float(width) / height);
		float cameraViewHalfHeight = m_CameraController.GetZoomLevel();

		float posx = ((float(mouseX) / width) - 0.5) * 2.0 * cameraViewHalfWidth + camerapos.x;
		float posy = ((1.0 - float(mouseY) / height) - 0.5) * 2.0 * cameraViewHalfHeight + camerapos.y;
		m_ParticSystem.push_back(std::make_shared<ParticSystem>(ParticSystem::ParticSpawnType::Burst, 5.0f, 1.0f, glm::vec2(posx, posy), glm::vec2(0.05, 0.05), glm::vec2(0.1f, 0.1f), m_StartColor, m_EndColor));

	}

	for (auto system : m_ParticSystem)
	{
		system->OnUpdate(ts);
		system->OnRender(m_CameraController.GetCamera().GetViewProjectionMatrix());
	}
	
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Controls");
	ImGui::ColorEdit4("Start Color", glm::value_ptr(m_StartColor));
	ImGui::ColorEdit4("End Color", glm::value_ptr(m_EndColor));
	ImGui::End();
}
