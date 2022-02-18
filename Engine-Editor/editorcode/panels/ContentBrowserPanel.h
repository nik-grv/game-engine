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
		//Ref<TextureRend> m_DirectoryIcon;
		//Ref<Texture2D> m_FileIcon;
	};

}
