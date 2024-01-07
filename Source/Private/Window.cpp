#include "../Public/Window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>
#include <iostream>

namespace Application
{

	Window::Window(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
	{
		InitWindow();
	}

	Window::~Window()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{		
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create surface");
		}

	}

	void Window::InitWindow()
	{
		std::cout << "\n[INFO]: Creating window..." << std::endl;
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		std::cout << "\t[INFO]: Window created successfuly" << std::endl;

	}

}
