#pragma once

#include <filesystem>
#include "include/independent/rendering/TextureRend.h"
#include "rendering/subTexture.h"

namespace Engine {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;
		std::shared_ptr<TextureRend> m_DirectoryIcon;
		std::shared_ptr<TextureRend> m_FileIcon;
		std::shared_ptr<TextureRend> Icon;
	};

}
