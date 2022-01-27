#include "OpenGLTests.h"

//vertex array test
TEST(OpenGL, VetexArrayConstructor) 
{
	Engine::OpenGLVertexArray vao;
	uint32_t renderId = vao.getRenderID();

	EXPECT_EQ(renderId, 1);
}	

//window properties test
TEST(OpenGL, WindowProperties)
{
	Engine::WindowProperties props("test title",1920,1080,false);
	
	const char* title = props.title;
	int32_t w = props.width;
	int32_t h = props.height;
	bool fullscreen = props.isFullScreen;

	EXPECT_EQ(title,"test title");
	EXPECT_EQ(w,1920);
	EXPECT_EQ(h,1080);
	EXPECT_EQ(fullscreen,false);
}

//vertex buffer test
TEST(OpenGL, VertexArrayBuffer) 
{
	Engine::OpenGLVertexArray vao;
	uint32_t bufferSize = vao.getVertexBuffers().size();
	EXPECT_EQ(bufferSize, 0);
}

//vertex draw count
TEST(OpenGL, VertexArrayDrawCount)
{
	Engine::OpenGLVertexArray vao;
	uint32_t drawCount = vao.getDrawCount();
	EXPECT_EQ(drawCount, 0);
}

