#include <windows.h>
#include <string>

//glfw
#include <GLFW/glfw3.h>

//glm
// A.1. glm ライブラリーの一部機能は CPP を定義して制御できるようになっています。
//#define GLM_SWIZZLE
// 1. glm ライブラリーの基本機能が含まれます。
#include <glm/glm.hpp>
// 2. glm ライブラリーの拡張機能のうち既に仕様が安定した機能が glm/gtc に含まれます。
#include <glm/gtc/constants.hpp>
// 3. glm ライブラリーの拡張機能のうち試験的に実装されている機能が glm/gtx に含まれます。
#include <glm/gtx/color_space.hpp>
// 4. glm ライブラリーの拡張機能をひとまとめに取り込みたい場合には glm/ext.hpp を使います。
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

	//glm処理
	auto a = glm::vec2(3, 4);
	auto len = glm::length(a);
	char tmp[128];
	std::snprintf(tmp, 128, "a(x,y):(%f,%f): len:%f\n", a.x, a.y, len);
	OutputDebugStringA(tmp);

	while (!glfwWindowShouldClose(window))
	{
		/*
		** ここで OpenGL による描画を行う
		*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
