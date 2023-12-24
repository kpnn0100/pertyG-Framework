#include "Window.h"
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 position;

    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 color;

    void main()
    {
        color = vec4(1.0, 1.0, 1.0, 1.0);
    }
)";
namespace pertyG
{

    Window::Window(int width, int height) : mPropertyManager(PropertyManager(PropertyCount))
    {
        mMainWindow = nullptr;
        create(width, height, "hello");
        mPropertyManager.initValue(WindowWidth, width);
        mPropertyManager.initValue(WindowHeight, height);
    }
    void Window::addTask(std::function<void()> task)
    {
        std::lock_guard<std::mutex> lock(mTaskListLocker);
        mTaskList.push(task);
    }
    void Window::create(int width, int height, const char* title)
    {
        if (mMainWindow == nullptr)
        {
            if (!glfwInit())
            {
                return;
            }

            // Set GLFW window hints (optional)
            // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

            mMainWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
            glfwSetWindowSizeCallback(mMainWindow, Frame::onWindowSizeChanged);
            mBound = Rectangle(Point(0.0, 0.0), (double)width, (double)height);
            //mMainFrame = std::make_shared<Frame>(mMainWindow);
            if (!mMainWindow)
            {
                // Window creation failed
                // You can add error handling or logging here
                return;
            }


        }
    }
    void Window::show()
    {
        if (mMainWindow)
        {
            handleWindow();
        }
    }
    void Window::handleWindow()
    {
        // Make the window's context current
        glfwMakeContextCurrent(mMainWindow);
        glewExperimental = true;
        if (glewInit() != GLEW_OK) {
            fprintf(stderr, "Failed to initialize GLEW\n");
            getchar();
            glfwTerminate();
            return;
        }

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glUseProgram(shaderProgram);

        GLuint vertexArrayID;
        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);

        GLuint vertexBuffer;
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        mMainFrame->fillColor(Color(0, 0, 0, 255));
        // Enter the rendering loop in a separate thread
        while (!glfwWindowShouldClose(mMainWindow))
        {
            //temp

            // Your rendering code here
            {
                std::lock_guard<std::mutex> lock(mTaskListLocker);
                for (; !mTaskList.empty(); mTaskList.pop())
                {
                    mTaskList.front()();
                }
            }


            // Swap front and back buffers
            glfwSwapBuffers(mMainWindow);

            // Poll for and process events
            glfwPollEvents();
        }
        glDeleteVertexArrays(1, &vertexArrayID);
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteProgram(shaderProgram);
    }
    void Window::setSize(int width, int height)
    {
        mBound.setSize(width, height);
    }
    void Window::onFrameInitialized()
    {
        int a = 2;
    }
    void Window::onFrameProcessed()
    {
        if (mMainWindow)
        {
            addTask([this]
                {
                    triggerPaint();
                });
            
        }
    }
    void Window::onFrameRendered()
    {
        mNeedUpdate = !mPropertyManager.isSet();
    }
    void Window::waitToClose()
    {
        if (renderThread.joinable())
        {
            renderThread.join();
        }
    }
    void Window::close()
    {
        if (mMainWindow)
        {
            glfwSetWindowShouldClose(mMainWindow, GL_TRUE);
        }
        if (renderThread.joinable())
        {
            renderThread.join();
        }
    }
    void Window::paint()
    {
        mainFrame.fillColor(Color(0, 0, 0,255));
        mainFrame.drawRectangle(Color(50, 50, 50), mBound.withSizeKeepCenter(200,300), 2.0);
    }
    PropertyManager& Window::getPropertyManager()
    {
        return mPropertyManager;
    }
    Window::~Window()
    {
        if (mMainWindow)
        {
            glfwDestroyWindow(mMainWindow);
        }
    }
}