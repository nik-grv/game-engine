#include <gtest/gtest.h>
#include <iostream>

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();

	std::cout << std::endl << "Press enter to continue" << std::endl;
	std::getchar();
	return 0;
}