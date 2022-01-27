#include "eventTests.h"

//window resize test
TEST(Events, Resize) 
{
	Engine::WindowResizeEvent resizeTest(800, 500);

	int32_t width1 = resizeTest.getWidth();
	int32_t height1 = resizeTest.getHeight();
	auto size = resizeTest.getSize();
	int32_t width2 = size.x;
	int32_t height2 = size.y;

	int32_t category = resizeTest.getCategoryFlags();
	Engine::EventTypes type = resizeTest.getEventType();
	Engine::EventTypes staticType = resizeTest.getStaticType();

	EXPECT_EQ(width1, 800);
	EXPECT_EQ(width2, 800);
	EXPECT_EQ(height1, 500);
	EXPECT_EQ(height2, 500);

	EXPECT_EQ(category, Engine::EventCategory::EventCategoryWindow);
	EXPECT_EQ(category, 1);

	EXPECT_TRUE(resizeTest.isInEventCategory(Engine::EventCategory::EventCategoryWindow));
	EXPECT_EQ(type,staticType);
	EXPECT_EQ(type, Engine::EventTypes::WindowResize);
	
}

//window moved test
TEST(Events, WindowMoved)
{
	Engine::WindowMovedEvent movedEventTest(500, 250);

	int32_t x1 = movedEventTest.getXPosition();
	int32_t y1 = movedEventTest.getYPosition();
	auto pos = movedEventTest.getPos();
	int32_t x2 = pos.x;
	int32_t y2 = pos.y;

	int32_t category = movedEventTest.getCategoryFlags();
	Engine::EventTypes type = movedEventTest.getEventType();
	Engine::EventTypes staticType = movedEventTest.getStaticType();

	EXPECT_EQ(x1, 500);
	EXPECT_EQ(y1, 250);
	EXPECT_EQ(x2, 500);
	EXPECT_EQ(y2, 250);

	EXPECT_EQ(category, Engine::EventCategory::EventCategoryWindow);
	EXPECT_EQ(category, 1);

	EXPECT_TRUE(movedEventTest.isInEventCategory(Engine::EventCategory::EventCategoryWindow));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventTypes::WindowMoved);
}


//window focus test
TEST(Events, WindowFocus)
{
	Engine::WindowFocusEvent windowFocusTest;

	int32_t category = windowFocusTest.getCategoryFlags();
	Engine::EventTypes type = windowFocusTest.getEventType();
	Engine::EventTypes staticType = windowFocusTest.getStaticType();

	EXPECT_EQ(category, Engine::EventCategory::EventCategoryWindow);
	EXPECT_EQ(category, 1);

	EXPECT_TRUE(windowFocusTest.isInEventCategory(Engine::EventCategory::EventCategoryWindow));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventTypes::WindowFocus);
}

//window lost focus test
TEST(Events, WindowLostFocus)
{
	Engine::WindowLostFocusEvent windowLostFocusTest;

	int32_t category = windowLostFocusTest.getCategoryFlags();
	Engine::EventTypes type = windowLostFocusTest.getEventType();
	Engine::EventTypes staticType = windowLostFocusTest.getStaticType();

	EXPECT_EQ(category, Engine::EventCategory::EventCategoryWindow);
	EXPECT_EQ(category, 1);

	EXPECT_TRUE(windowLostFocusTest.isInEventCategory(Engine::EventCategory::EventCategoryWindow));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventTypes::WindowLostFocus);
}

//key press test
TEST(Events, KeyPress)
{
	Engine::KeyPressedEvent keyPressedTest(63, 5);

	int32_t keyCode = keyPressedTest.getKeyCode();
	int32_t repeatCount = keyPressedTest.getRepeatCount();

	int32_t category = keyPressedTest.getCategoryFlags();
	Engine::EventTypes type = keyPressedTest.getEventType();
	Engine::EventTypes staticType = keyPressedTest.getStaticType();


	EXPECT_EQ(keyCode, 63);
	EXPECT_EQ(repeatCount, 5);

	EXPECT_EQ(category, Engine::EventCategory::EventCategoryInput | Engine::EventCategory::EventCategoryKeyboard); 
	EXPECT_EQ(category, 6);

	EXPECT_TRUE(keyPressedTest.isInEventCategory(Engine::EventCategory::EventCategoryKeyboard));
	EXPECT_TRUE(keyPressedTest.isInEventCategory(Engine::EventCategory::EventCategoryInput));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventTypes::KeyPressed);

}


//key release test
TEST(Events, KeyRelease)
{
	Engine::KeyReleasedEvent keyReleasedTest(63);

	int32_t keyCode = keyReleasedTest.getKeyCode();

	int32_t category = keyReleasedTest.getCategoryFlags();
	Engine::EventTypes type = keyReleasedTest.getEventType();
	Engine::EventTypes staticType = keyReleasedTest.getStaticType();


	EXPECT_EQ(keyCode, 63);
	EXPECT_EQ(category, Engine::EventCategory::EventCategoryInput | Engine::EventCategory::EventCategoryKeyboard);
	EXPECT_EQ(category, 6);

	EXPECT_TRUE(keyReleasedTest.isInEventCategory(Engine::EventCategory::EventCategoryKeyboard));
	EXPECT_TRUE(keyReleasedTest.isInEventCategory(Engine::EventCategory::EventCategoryInput));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventTypes::KeyReleased);

}


//mouse button press test
TEST(Events, MouseButtonPress)
{
	Engine::MouseButtonPressedEvent mouseBtnPressTest(9);

	int32_t btnCode = mouseBtnPressTest.getButton();
	int32_t category = mouseBtnPressTest.getCategoryFlags();
	Engine::EventTypes type = mouseBtnPressTest.getEventType();
	Engine::EventTypes staticType = mouseBtnPressTest.getStaticType();


	EXPECT_EQ(btnCode, 9);

	EXPECT_EQ(category, Engine::EventCategory::EventCategoryInput | Engine::EventCategory::EventCategoryMouse); 
	EXPECT_EQ(category, 10);

	EXPECT_TRUE(mouseBtnPressTest.isInEventCategory(Engine::EventCategory::EventCategoryMouse));
	EXPECT_TRUE(mouseBtnPressTest.isInEventCategory(Engine::EventCategory::EventCategoryInput));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventTypes::MouseBtnPressed);
}


//mouse button release test
TEST(Events, MouseButtonRelease)
{
	Engine::MouseButtonReleasedEvent mouseBtnReleaseTest(9);

	int32_t btnCode = mouseBtnReleaseTest.getButton();
	int32_t category = mouseBtnReleaseTest.getCategoryFlags();
	Engine::EventTypes type = mouseBtnReleaseTest.getEventType();
	Engine::EventTypes staticType = mouseBtnReleaseTest.getStaticType();


	EXPECT_EQ(btnCode, 9);

	EXPECT_EQ(category, Engine::EventCategory::EventCategoryInput | Engine::EventCategory::EventCategoryMouse);
	EXPECT_EQ(category, 10);

	EXPECT_TRUE(mouseBtnReleaseTest.isInEventCategory(Engine::EventCategory::EventCategoryMouse));
	EXPECT_TRUE(mouseBtnReleaseTest.isInEventCategory(Engine::EventCategory::EventCategoryInput));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventTypes::MouseBtnReleased);
}


//mouse moved test
TEST(Events, MouseMoved)
{
	Engine::MouseMovedEvent mouseMovedTest(125, 500);

	int32_t x1 = mouseMovedTest.getX();
	int32_t y1 = mouseMovedTest.getY();
	auto pos = mouseMovedTest.getMousePos();
	int32_t x2 = pos.x;
	int32_t y2 = pos.y;

	int32_t category = mouseMovedTest.getCategoryFlags();
	Engine::EventTypes type = mouseMovedTest.getEventType();
	Engine::EventTypes staticType = mouseMovedTest.getStaticType();

	EXPECT_EQ(x1, 125);
	EXPECT_EQ(y1, 500);
	EXPECT_EQ(x2, 125);
	EXPECT_EQ(y2, 500);

	EXPECT_EQ(category, Engine::EventCategory::EventCategoryInput | Engine::EventCategory::EventCategoryMouse);
	EXPECT_EQ(category, 10);

	EXPECT_TRUE(mouseMovedTest.isInEventCategory(Engine::EventCategory::EventCategoryMouse));
	EXPECT_TRUE(mouseMovedTest.isInEventCategory(Engine::EventCategory::EventCategoryInput));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventTypes::MouseMoved);
}