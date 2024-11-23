#pragma once

#include <glm/glm.hpp>
#include <GLCoreUtils.h>
#include <vector>
#include <memory>

#include "RandomGenerate.h"


class ParticSystem
{
public:
	struct ParticProps
	{
		float RemainLifeTime;
		float RotationVelocity;
		float radition;
		glm::vec2 Velocity;
		glm::vec2 Position;
		bool isActive;
		ParticProps(float lifetime, const glm::vec2& position, bool active = true)
		{
			RemainLifeTime = lifetime;
			Velocity.x = (RandomGenerate::GetRandomFloat() - 0.5);
			Velocity.y = (RandomGenerate::GetRandomFloat() - 0.5);
			RotationVelocity = (RandomGenerate::GetRandomFloat() - 0.5) * 6;
			radition = 0.0f;
			Position = position;
			
			isActive = active;
		}
	};
	enum class ParticSpawnType
	{
		None = 0,
		Burst = 1,
		Continue = 2
	};
public:
	ParticSystem(ParticSpawnType spawnType, float rate, float lifetime, glm::vec2 position, glm::vec2 startsize, glm::vec2 endsize, glm::vec4 startcolor, glm::vec4 endcolor);
	~ParticSystem();
	void OnUpdate(float ts);
	void OnRender(const glm::mat4& viewprojection);
	static void Init();
	static void End();
protected:
	std::vector<ParticProps> m_Partics;
	float m_Rate;
	float m_LifeTime;
	ParticSpawnType m_SpawnType;
	glm::vec2 m_Position;

	glm::vec2 m_StartSize, m_EndSize;
	glm::vec4 m_StartColor, m_EndColor;

	static unsigned int m_VAO;
	static std::shared_ptr<GLCore::Utils::Shader> m_Shader;

	static int m_TransformLocation, m_ViewProjectionLocation, m_ColorLocation;
};