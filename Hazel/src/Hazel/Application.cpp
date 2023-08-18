#include "hzpch.h"
#include "Core.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"

#include <glad/glad.h>

namespace Hazel {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application() {}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run() {

		HZ_CORE_INFO("run.");

		while (m_Running)
		{
			glClearColor(0.2, 0.5, 0.4, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}
			m_Window->OnUpdate();
		}
		HZ_INFO("end.");
	}

	void Application::OnEvent(Event& e)
	{
		HZ_CORE_TRACE("{0}", e.ToString());
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
		// ?????
		if (e.Handled)
			return;
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		e.Handled = true;
		return true;
	}
}

