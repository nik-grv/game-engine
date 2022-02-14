/** \file Framebuffer.h
*/
#pragma once

#include <unordered_map>
#include <vector>
#include "events/events.h"
#include <memory>
#include "rendering/TextureRend.h"
#include "rendering/renderBuffer.h"

namespace Engine
{


	using Attachment = std::pair<AttachmentType, bool>;

	class FramebufferLayout
	{
	public:
		FramebufferLayout() {};
		//create a framebuffer object
		FramebufferLayout(const std::initializer_list<Attachment>& attachment, uint32_t stride = 0) : m_attachment(attachment) {}
		//add framebuffer attachment
		void addAttachment (AttachmentType type , bool sampled = false)
		{
			m_attachment.push_back(Attachment(type, sampled));
		}

		//iterator return functions
		std::vector<Attachment>::iterator begin() { return m_attachment.begin(); }
		std::vector<Attachment>::iterator end() { return m_attachment.end(); }
		std::vector<Attachment>::const_iterator begin() const { return m_attachment.begin(); }
		std::vector<Attachment>::const_iterator end() const{ return m_attachment.end(); }

	private:
		std::vector<Attachment> m_attachment;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void OnResize(WindowResizeEvent& e) = 0;
		virtual uint32_t getRenderID() = 0; //get render id
		virtual void use() = 0; // bind the FB
		virtual std::shared_ptr<TextureRend> getTexture(uint32_t index) = 0; // get render texture at index

		static Framebuffer* create(glm::ivec2 size, FramebufferLayout layout); //API agnostic
		static Framebuffer* createDefault();

	protected:
		glm::ivec2 m_size; // size of the FB
		FramebufferLayout m_layout; // layout of FB attachment
		std::vector<std::shared_ptr<TextureRend>> m_sampleTargets; // sample targets
		std::vector<std::shared_ptr<RenderBuffer>> m_nonSampledTargets;
	};


}