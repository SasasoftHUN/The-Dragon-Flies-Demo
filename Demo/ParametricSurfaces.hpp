#pragma once
#include <glm/glm.hpp>
#include <SDL\SDL_stdinc.h>

namespace ParametricSurfaces
{
	glm::vec3 GetSpherePos(float u, float v)
	{
		u *= float(2 * M_PI);
		v *= float(M_PI);
		float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
		float r = 2;

		return glm::vec3(r * cu * sv, r * cv, r * su * sv);
	}
	glm::vec3 GetSphereNorm(float u, float v)
	{
		u *= float(2 * M_PI);
		v *= float(M_PI);
		float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
		return glm::vec3(cu * sv, cv, su * sv);
	}


	glm::vec2 GetTex(float u, float v)
	{
		return glm::vec2(1 - u, 1 - v);
	}
}