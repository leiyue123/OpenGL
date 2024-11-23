#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

#include "ParticSystem.h"

class ExampleLayer : public GLCore::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	GLCore::Utils::OrthographicCameraController m_CameraController;
	std::vector<std::shared_ptr<ParticSystem>> m_ParticSystem;
	glm::vec4 m_StartColor = { 0.1, 0.2, 0.5, 1.0 };
	glm::vec4 m_EndColor = { 0.4, 0.2, 0.8, 0.0 };
};