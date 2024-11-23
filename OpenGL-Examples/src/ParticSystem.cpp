#include "ParticSystem.h"

#include <Glad/glad.h>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/type_ptr.hpp>

unsigned int ParticSystem::m_VAO;
std::shared_ptr<GLCore::Utils::Shader> ParticSystem::m_Shader;

int ParticSystem::m_TransformLocation;
int ParticSystem::m_ViewProjectionLocation;
int ParticSystem::m_ColorLocation;

ParticSystem::ParticSystem(ParticSpawnType spawnType, float rate, float lifetime, glm::vec2 position, glm::vec2 startsize, glm::vec2 endsize, glm::vec4 startcolor, glm::vec4 endcolor)
	:m_SpawnType(spawnType), m_Rate(rate), m_LifeTime(lifetime), m_Position(position), m_StartSize(startsize), m_EndSize(endsize), m_StartColor(startcolor), m_EndColor(endcolor)
{
	//Init();
	if (spawnType == ParticSpawnType::Burst)
	{
		m_Partics.reserve(rate);
		for (int i = 0; i < (int)m_Rate; i++)
		{
			m_Partics.emplace_back(m_LifeTime, m_Position);
		}
	}
}

ParticSystem::~ParticSystem()
{
	
}

void ParticSystem::OnUpdate(float ts)
{
	for (auto& partic : m_Partics)
	{
		if (partic.isActive)
		{
			partic.Position += partic.Velocity * ts;
			partic.radition += partic.RotationVelocity * ts;
			partic.RemainLifeTime -= ts;
			if (partic.RemainLifeTime < 0.0f)
			{
				partic.isActive = false;
			}
		}
	}
}

void ParticSystem::OnRender(const glm::mat4& viewprojection)
{
	unsigned int shader = m_Shader->GetRendererID();
	glUseProgram(shader);
	glUniformMatrix4fv(m_ViewProjectionLocation, 1, GL_FALSE, glm::value_ptr(viewprojection));
	for (auto& partic : m_Partics)
	{
		if (partic.isActive)
		{
			glm::vec4 color = glm::lerp(m_EndColor, m_StartColor, partic.RemainLifeTime / m_LifeTime);
			glm::vec2 scale = glm::lerp(m_EndSize, m_StartSize, partic.RemainLifeTime / m_LifeTime);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(partic.Position, 0.0f)) * glm::rotate(glm::mat4(1.0f), partic.radition, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));
			glUniform4f(m_ColorLocation, color.r, color.g, color.b, color.a);
			glUniformMatrix4fv(m_TransformLocation, 1, GL_FALSE, glm::value_ptr(transform));
			glBindVertexArray(m_VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
	}
}

void ParticSystem::Init()
{
	glCreateVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	unsigned int vbo;
	glCreateBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	float vertex[4 * 3] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

	unsigned int index[6] =
	{
		0, 1, 2,
		2, 3, 0
	};

	unsigned int ibo;
	glCreateBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

	m_Shader.reset(GLCore::Utils::Shader::FromGLSLTextFiles("assets/shaders/test.vert.glsl", "assets/shaders/test.frag.glsl"));
	unsigned int shader = m_Shader->GetRendererID();
	m_TransformLocation = glGetUniformLocation(shader, "u_Transform");
	m_ViewProjectionLocation = glGetUniformLocation(shader, "u_ViewProjection");
	m_ColorLocation = glGetUniformLocation(shader, "u_Color");
}

void ParticSystem::End()
{
	glDeleteVertexArrays(1, &m_VAO);
	
}
