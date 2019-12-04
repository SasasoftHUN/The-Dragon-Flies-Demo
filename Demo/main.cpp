#include <Dragonfly/editor.h>
#include <Dragonfly/detail/buffer.h> //will be replaced
#include <Dragonfly/detail/vao.h> //will be replaced
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include "ParametricSurfaces.hpp"

using namespace df;
using namespace ParametricSurfaces;

struct Vertex
{
	glm::vec3 p;
	glm::vec3 n;
	glm::vec2 t;
};

int main(int argc, char* args[])
{
	Sample sam; //handles Events and such

	Camera cam;
	sam.AddHandlerClass(cam, 5);
	sam.AddStaticHandlerClass<ImGuiHandler>(10);


	//SPHERE CREATION
	static const int N = 20;
	Vertex sphereVertices[(N + 1) * (N + 1)];
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= N; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)N;

			sphereVertices[i + j * (N + 1)].p = GetSpherePos(u, v);
			sphereVertices[i + j * (N + 1)].n = GetSphereNorm(u, v);
			sphereVertices[i + j * (N + 1)].t = GetTex(u, v);
		}

	//¯\_(ツ)_/¯
	std::vector<glm::vec3> spherePos;
	std::vector<glm::vec3> sphereNorm;
	std::vector<glm::vec2> sphereTex;
	spherePos.resize(3 * 2 * (N) * (N));
	sphereNorm.resize(3 * 2 * (N) * (N));
	sphereTex.resize(3 * 2 * (N) * (N));
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
		{
			spherePos[6 * i + j * 3 * 2 * (N)+0] = sphereVertices[(i)+(j) * (N + 1)].p;
			spherePos[6 * i + j * 3 * 2 * (N)+1] = sphereVertices[(i + 1) + (j) * (N + 1)].p;
			spherePos[6 * i + j * 3 * 2 * (N)+2] = sphereVertices[(i)+(j + 1) * (N + 1)].p;
			spherePos[6 * i + j * 3 * 2 * (N)+3] = sphereVertices[(i + 1) + (j) * (N + 1)].p;
			spherePos[6 * i + j * 3 * 2 * (N)+4] = sphereVertices[(i + 1) + (j + 1) * (N + 1)].p;
			spherePos[6 * i + j * 3 * 2 * (N)+5] = sphereVertices[(i)+(j + 1) * (N + 1)].p;

			sphereNorm[6 * i + j * 3 * 2 * (N)+0] = sphereVertices[(i)+(j) * (N + 1)].n;
			sphereNorm[6 * i + j * 3 * 2 * (N)+1] = sphereVertices[(i + 1) + (j) * (N + 1)].n;
			sphereNorm[6 * i + j * 3 * 2 * (N)+2] = sphereVertices[(i)+(j + 1) * (N + 1)].n;
			sphereNorm[6 * i + j * 3 * 2 * (N)+3] = sphereVertices[(i + 1) + (j) * (N + 1)].n;
			sphereNorm[6 * i + j * 3 * 2 * (N)+4] = sphereVertices[(i + 1) + (j + 1) * (N + 1)].n;
			sphereNorm[6 * i + j * 3 * 2 * (N)+5] = sphereVertices[(i)+(j + 1) * (N + 1)].n;

			sphereTex[6 * i + j * 3 * 2 * (N)+0] = sphereVertices[(i)+(j) * (N + 1)].t;
			sphereTex[6 * i + j * 3 * 2 * (N)+1] = sphereVertices[(i + 1) + (j) * (N + 1)].t;
			sphereTex[6 * i + j * 3 * 2 * (N)+2] = sphereVertices[(i)+(j + 1) * (N + 1)].t;
			sphereTex[6 * i + j * 3 * 2 * (N)+3] = sphereVertices[(i + 1) + (j) * (N + 1)].t;
			sphereTex[6 * i + j * 3 * 2 * (N)+4] = sphereVertices[(i + 1) + (j + 1) * (N + 1)].t;
			sphereTex[6 * i + j * 3 * 2 * (N)+5] = sphereVertices[(i)+(j + 1) * (N + 1)].t;
		}
	eltecg::ogl::ArrayBuffer spherePosVBO;
	eltecg::ogl::ArrayBuffer sphereNormVBO;
	eltecg::ogl::ArrayBuffer sphereTexVBO;
	spherePosVBO.constructImmutable(spherePos);
	sphereNormVBO.constructImmutable(sphereNorm);
	sphereTexVBO.constructImmutable(sphereTex);
	eltecg::ogl::VertexArray sphereVAO;
	sphereVAO.addVBO<glm::vec3>(spherePosVBO);
	sphereVAO.addVBO<glm::vec3>(sphereNormVBO);
	sphereVAO.addVBO<glm::vec2>(sphereTexVBO);
	VaoWrapper sphereVAOWrapper((GLuint)sphereVAO, GL_TRIANGLES, 3 * 2 * N * N, 0u);


	//SKYBOX CREATION
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	eltecg::ogl::ArrayBuffer cubeVBO;
	{
		glm::vec3 a = glm::vec3(-1, -1, -1);	//0
		glm::vec3 b = glm::vec3(1, -1, -1);		//1
		glm::vec3 c = glm::vec3(1, 1, -1);		//2
		glm::vec3 d = glm::vec3(-1, 1, -1);		//3

		glm::vec3 e = glm::vec3(-1, -1, 1);		//4
		glm::vec3 f = glm::vec3(1, -1, 1);		//5
		glm::vec3 g = glm::vec3(1, 1, 1);		//6
		glm::vec3 h = glm::vec3(-1, 1, 1);		//7

		cubeVBO.constructImmutable(
			std::vector<glm::vec3> {
				a, b, c, //Back
				c, d, a,
				e, g, f, //Front
				g, e, h,
				a, d, e, //Left
				e, d, h,
				b, f, c, //Right
				f, g, c,
				b, a, e, //Bottom
				b, e, f,
				d, c, g, //Top
				d, g, h,
			}
		);
	}

	eltecg::ogl::VertexArray cubeVAO;
	cubeVAO.addVBO<glm::vec3>(cubeVBO);

	VaoWrapper cubeVAOWrapper((GLuint)cubeVAO, GL_TRIANGLES, 36, 0u);

	TextureCube<> skyboxTexture("Assets/xpos.png", "Assets/xneg.png", "Assets/ypos.png", "Assets/yneg.png", "Assets/zpos.png", "Assets/zneg.png");
	Texture2D<> earthDiffuseTexture("Assets/earth.jpg", 1, true);
	Texture2D<> earthSpecularTexture("Assets/earth_specular.jpg", 1, true);

	ShaderProgramEditorVF skyboxProgram = "SkyboxShaderProgram";
	skyboxProgram << "Shaders/skybox.vert.glsl"_vert << "Shaders/skybox.frag.glsl"_fs << LinkProgram;
	GL_CHECK; //extra opengl error checking in GPU Debug build configuration

	ShaderProgramEditorVF phongProgram = "PhongShaderProgram";
	phongProgram << "Shaders/phong.vert.glsl"_vert << "Shaders/phong.frag.glsl"_fs << LinkProgram;
	GL_CHECK; //extra opengl error checking in GPU Debug build configuration

	sam.Run([&](float deltaTime)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			cam.Update();

			//RENDER SPHERE
			phongProgram << "MVP" << cam.GetViewProj() << "world" << glm::mat4(1.0f) << "worldIT" << glm::inverse(glm::transpose(glm::mat4(1.0f))) <<
				"eye_pos" << cam.GetEye() <<
				"diffuseTexture" << earthDiffuseTexture << "specularTexture" << earthSpecularTexture << "skyboxTexture" << skyboxTexture;
			phongProgram << sphereVAOWrapper;

			//RENDER SKYBOX
			GLint prevDepthFnc;
			glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);
			glDepthFunc(GL_LEQUAL);
			skyboxProgram << "MVP" << cam.GetViewProj() * glm::translate(cam.GetEye()) << "skyboxTexture" << skyboxTexture;
			skyboxProgram << cubeVAOWrapper;
			glDepthFunc(prevDepthFnc);


			GL_CHECK;
			phongProgram.Render();
			//skyboxProgram.Render();
		}
	);
	return 0;
}

