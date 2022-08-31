project (robotics_v1)

add_executable (robotics_v1
	main.cpp
	utils/Image.cpp)

target_compile_options(robotics_v1 PUBLIC -std=c++20 -Werror -Wall -Wextra -pedantic -pedantic-errors -Wno-unused
		-O3 -g)
    
