#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// window resize callback function
void framebuffer_size_callback( GLFWwindow*, int, int );

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main()
{
    // initialize GLFW
    glfwInit( );

    // configure OpenGL's minor and major versions to be 3.3
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );

    // configure GLFW to use only the core-profile
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    // creating a window object to hold windowing data
    GLFWwindow* window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL );
    if ( window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        return -1;
    }

    // make the newly created window the main context of the current thread
    glfwMakeContextCurrent( window );

    // initializing GLAD
    if ( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) )
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // setting the size of the rendering window
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );

    while ( !glfwWindowShouldClose( window ) )
    {
        glfwSwapBuffers( window );
        glfwPollEvents( );
    }
  
    glfwTerminate( );
    return 0;
}

// window resize callback function implementation
void framebuffer_size_callback( GLFWwindow* window, int width, int height )
{
    glViewport( 0, 0, width, height );
}