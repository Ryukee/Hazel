#include "hzpch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"

#include <GLFW/glfw3.h>

namespace Hazel {
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{

	}

	void Application::Run() {

		HZ_CORE_INFO("run.");
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			HZ_TRACE(e.ToString());
		}

		while (m_Running)
		{
			glClearColor(0.2, 0.5, 0.4, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}

