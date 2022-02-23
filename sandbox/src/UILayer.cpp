#include "../include/UILayer.h"

UILayer::UILayer(const char* name) : Layer(name)
{

	blendFuncAlphaCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::blendFuncCommand, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	enableBlendCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::enableCommand, GL_BLEND));
	disableDepthCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::disableCommand, GL_DEPTH_TEST));
	enableDepthCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::enableCommand, GL_DEPTH_TEST));
	disableBlendCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::disableCommand, GL_BLEND));

	auto& window = Application::getInstance().getAppWindow();

	m_view2D = glm::mat4(1.0f);
	m_projection2D = glm::ortho(0.0f, static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()), 0.0f);

	m_swu["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_view2D)));
	m_swu["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_projection2D)));

	Quad quads[5] =
	{
		Quad::createCentreHalfExtens({ window->getWidth() / 2 , window->getHeight() - 50 }, {window->getWidth(),50.0f}),
		Quad::createCentreHalfExtens({ 100, window->getHeight() - 50 }, {100,50.0f}),
		Quad::createCentreHalfExtens({ window->getWidth() - 100, window->getHeight() - 50 }, {100,50.0f}),
		Quad::createCentreHalfExtens({ (window->getWidth() / 2) + 350 ,(window->getHeight() / 2) + 270 }, {100.0f,60.0f}),
		Quad::createCentreHalfExtens({ 200.0f,100 }, {50.0f,50.0f}),
	};

	m_quad.push_back(quads[0]);
}

void UILayer::OnRender()
{

	RendererShared::actionCommand(disableDepthCommand);
	RendererShared::actionCommand(enableBlendCommand);

	//RendererShared::actionCommand(blendFuncAlphaCommand);

	Renderer2D::begin(m_swu);
	Renderer2D::submit(m_quad[0], glm::vec4(.5f, 1.0f, .5f, .5f));

	Renderer2D::submit("FPS :", glm::vec2(20.0f, 50.0f), glm::vec4(1.0f, .0f, .0f, 1.0f));
	int fps = 1.0f / m_timestep;
	std::string fps_s = std::to_string(fps);
	const char* resultFPS = fps_s.c_str();
	Renderer2D::submit(resultFPS, glm::vec2(100.0f, 50.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	Renderer2D::end();

	RendererShared::actionCommand(disableBlendCommand);
	RendererShared::actionCommand(enableDepthCommand);

}

void UILayer::OnUpdate(float timeStep)
{
	m_timestep = timeStep;
}
