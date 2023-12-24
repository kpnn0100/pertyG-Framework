#include "GlwfManager.h"

namespace pertyG
{
    GlwfManager::GlwfManager()
    {
        if (!glfwInit()) {

        }
        std::cout << "GLFW done init" << std::endl;
    }

    GlwfManager::~GlwfManager()
    {
        std::cout << "Program exit" << std::endl;
        glfwTerminate();
    }
    GlwfManager instance;
}