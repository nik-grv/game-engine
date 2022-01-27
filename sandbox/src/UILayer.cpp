#include "../include/UILayer.h"

UILayer::UILayer(const char* name) : Layer(name)
{

	enableBlendCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::enableCommand, GL_BLEND));
	disableDepthCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::disableCommand, GL_DEPTH_TEST));
	blendFuncAlphaCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::blendFuncCommand, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
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

	RendererShared::actionCommand(blendFuncAlphaCommand);

	Renderer2D::begin(m_swu);
	Renderer2D::submit(m_quad[0], glm::vec4(1.f, 1.0f, 1.f, 1.f));
	Renderer2D::submit("FPSSSSSS :", glm::vec2(20.0f, 50.0f), glm::vec4(1.0f, .0f, .0f, 1.0f));

	Renderer2D::end();

	RendererShared::actionCommand(disableBlendCommand);
	RendererShared::actionCommand(enableDepthCommand);

}
