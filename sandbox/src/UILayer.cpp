#include "UILayer.h"

UILayer::UILayer(const char* name) : Layer(name)
{

	blendFuncAlphaCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::blendFuncCommand, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	enableBlendCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::enableCommand, GL_BLEND));
	disableDepthCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::disableCommand, GL_DEPTH_TEST));
	enableDepthCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::enableCommand, GL_DEPTH_TEST));
	disableBlendCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::disableCommand, GL_BLEND));
	setStandardBlendCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::blendFuncCommand, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	auto& window = Application::getInstance().getAppWindow();

	m_view2D = glm::mat4(1.0f);
	m_projection2D = glm::ortho(0.0f, static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()), 0.0f);

	m_swu["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_view2D)));
	m_swu["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_projection2D)));

	HorizontalContainer top, middle, slider, bottom;

	top.addWidget<Spacer>(100, 10);
	top.addWidget<Label>(300, 100, "Hello World!");

	middle.addWidget<Spacer>(50, 10);
	middle.addWidget<Label>(500, 100, "some text message.");

	slider.addWidget<Slider>(400, 0, 100, "Low", "High");
	slider.addWidget<Spacer>(50, 10);
	slider.addWidget<Slider>(200, 25, 75, "Min", "Max");

	bottom.addWidget<Button>(200, 60, "Button 1", []() {Log::info("clicked on button 1");  });
	bottom.addWidget<Spacer>(300, 10);
	bottom.addWidget<Button>(200, 60, "Button 2", []() {Log::info("clicked on button 2");  });

	m_window.addContainer(top);
	m_window.addContainer(middle);
	m_window.addContainer(slider);
	m_window.addContainer(bottom);

	m_window.show();

	//Quad quads[5] =
	//{
	//	Quad::createCentreHalfExtens({ window->getWidth() / 2 , window->getHeight() - 50 }, {window->getWidth(),50.0f}),
	//	Quad::createCentreHalfExtens({ 100, window->getHeight() - 50 }, {100,50.0f}),
	//	Quad::createCentreHalfExtens({ window->getWidth() - 100, window->getHeight() - 50 }, {100,50.0f}),
	//	Quad::createCentreHalfExtens({ (window->getWidth() / 2) + 350 ,(window->getHeight() / 2) + 270 }, {100.0f,60.0f}),
	//	Quad::createCentreHalfExtens({ 200.0f,100 }, {50.0f,50.0f}),
	//};

	//m_quad.push_back(quads[0]);
}

void UILayer::OnRender()
{

	switch (m_state)
	{
	case UILayerState::Activating:
		m_state = UILayerState::Active;
		break;
	case UILayerState::Deactivating:
		m_state = UILayerState::InActive;
		break;
	}

	if (m_state == UILayerState::Active)
	{


		RendererShared::actionCommand(disableDepthCommand);
		RendererShared::actionCommand(enableBlendCommand);
		RendererShared::actionCommand(setStandardBlendCommand);

		//RendererShared::actionCommand(blendFuncAlphaCommand);

		Renderer2D::begin(m_swu);
		m_window.onRender();

		//Renderer2D::submit(m_quad[0], glm::vec4(.5f, 1.0f, .5f, .5f));

		//Renderer2D::submit("FPS :", glm::vec2(20.0f, 50.0f), glm::vec4(1.0f, .0f, .0f, 1.0f));
		//int fps = 1.0f / m_timestep;
		//std::string fps_s = std::to_string(fps);
		//const char* resultFPS = fps_s.c_str();
		//Renderer2D::submit(resultFPS, glm::vec2(100.0f, 50.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		Renderer2D::end();

		//RendererShared::actionCommand(disableBlendCommand);
		//RendererShared::actionCommand(enableDepthCommand);
	}
}

void UILayer::onKeyPressed(KeyPressedEvent& e)
{
	switch (e.getKeyCode())
	{
	case NG_KEY_G:
		m_state = UILayerState::Activating;
		break;
	case NG_KEY_H:
		m_state = UILayerState::Deactivating;
		break;
	}
	if (m_state == UILayerState::Active) e.handle(true);
}

void UILayer::onMouseMoved(MouseMovedEvent& e)
{
	glm::ivec2 mousePos = e.getMousePos();
	m_window.onMouseMoved(mousePos);
}

void UILayer::onMouseBtnPressed(MouseButtonPressedEvent& e)
{
	glm::ivec2 mousePos = InputPoller::getMousePosition();
	m_window.onMouseBtnPressed(mousePos, e.getButton());
}

void UILayer::onMouseBtnReleased(MouseButtonReleasedEvent& e)
{
	glm::ivec2 mousePos = InputPoller::getMousePosition();
	m_window.onMouseBtnReleased(mousePos, e.getButton());
}
void UILayer::OnUpdate(float timeStep)
{
	m_timestep = timeStep;
}
