#include <windows.h>
#include <string>

//glfw
#include <GLFW/glfw3.h>

//glm
// A.1. glm ���C�u�����[�̈ꕔ�@�\�� CPP ���`���Đ���ł���悤�ɂȂ��Ă��܂��B
//#define GLM_SWIZZLE
// 1. glm ���C�u�����[�̊�{�@�\���܂܂�܂��B
#include <glm/glm.hpp>
// 2. glm ���C�u�����[�̊g���@�\�̂������Ɏd�l�����肵���@�\�� glm/gtc �Ɋ܂܂�܂��B
#include <glm/gtc/constants.hpp>
// 3. glm ���C�u�����[�̊g���@�\�̂��������I�Ɏ�������Ă���@�\�� glm/gtx �Ɋ܂܂�܂��B
#include <glm/gtx/color_space.hpp>
// 4. glm ���C�u�����[�̊g���@�\���ЂƂ܂Ƃ߂Ɏ�荞�݂����ꍇ�ɂ� glm/ext.hpp ���g���܂��B
#include <glm/ext.hpp>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//glm����
	auto a = glm::vec2(3, 4);
	auto len = glm::length(a);
	char tmp[128];
	std::snprintf(tmp, 128, "a(x,y):(%f,%f): len:%f\n", a.x, a.y, len);
	OutputDebugStringA(tmp);

	while (!glfwWindowShouldClose(window))
	{
		/*
		** ������ OpenGL �ɂ��`����s��
		*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
