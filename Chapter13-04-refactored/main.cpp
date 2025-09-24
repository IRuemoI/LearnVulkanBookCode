#include "application.h"
#include <iostream>

int main()
{
    try
    {
        auto app = FF::Application::create();
        app->run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
