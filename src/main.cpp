#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void processInput( GLFWwindow* );
void framebuffer_size_callback( GLFWwindow*, int, int );

// settings
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// simple Vertex Shader source code
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout ( location = 0 ) in vec3 aPos;\n"
                                 "void main( )\n"
                                 "{\n"
                                 "      gl_Position = vec4( aPos.x, aPos.y, aPos.z, 1.0f );\n"
                                 "}\n\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main( )\n"
                                   "{\n"
                                   "      FragColor = vec4( 1.0f, 0.5f, 0.2f, 1.0f );\n"
                                   "}\n\0";

int main( )
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
    if ( window == NULL )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        return -1;
    }

    // make the newly created window the main context of the current thread
    glfwMakeContextCurrent( window );
    // passing to GLFW the window resize callback function
    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );

    // initializing GLAD
    if ( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) )
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // creating a shader object to reference a vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader( GL_VERTEX_SHADER );
    // attaching the shader source code with the created shader object and compiling
    glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
    glCompileShader( vertexShader );

    // creating a shader object to reference a fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    // attaching the shader source code with the created shader object and compiling
    glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
    glCompileShader( fragmentShader );

    // creating the Shader Program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram( );
    // attaching previously compiled shaders to the Shader Program and linking them
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );
    glLinkProgram( shaderProgram );    

    // deleting created and now unnecessary shaders
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );   

    // triangle vertices in normalized device coordinates
    float vertices[] = {
         0.5f,  0.5f, 0.0f, // top right
         0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3,    // first triangle
        1, 2, 3     // second triangle
    };

    // generating a Vertex Array Object to store the states that were set
    unsigned int VAO;
    glGenVertexArrays( 1, &VAO );
    // generating a Vertex Buffer Object to be able to send the vertices data do the GPU
    unsigned int VBO;
    glGenBuffers( 1, &VBO );
    // generating a Element Buffer Object to store the vertex indices to be part of a specified triangle
    unsigned int EBO;
    glGenBuffers( 1, &EBO );

    // biding the Vertex Array Object (first)
    glBindVertexArray( VAO );
    // binding the newly generated Vertex Buffer Object with the GL_ARRAY_BUFFER of OpenGL (second)
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    // copying the previourly defined vertex data into the buffer's memory (third)
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    // binding the newly generated Element Buffer Object with the GL_ELEMENT_ARRAY_BUFFER of OpenGL
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    // copying the previously defined index data into the buffer's memory
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW );
    // teaching OpenGL how it should interpret the Vertex Data (fourth)
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0 );
    glEnableVertexAttribArray( 0 );
    // unbiding the current Vertex Buffer Object
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    // unbiding the current Vertex Array Object
    glBindVertexArray( 0 );

    // initializing render loop
    while ( !glfwWindowShouldClose( window ) )
    {
        // input processing
        processInput( window );

        // rendering commands
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT ); 

        // activating the Shader Program, biding the Vertex Array Object and (finally) drawing the triangle
        glUseProgram( shaderProgram );
        glBindVertexArray( VAO );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glDrawArrays( GL_TRIANGLES, 0, 3 );
        
        // check call events and swap buffer
        glfwSwapBuffers( window );
        glfwPollEvents( );
    }

    // deallocating all the used resources
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers( 1, &EBO );
  
    // clean up GLFW's allocated resources
    glfwTerminate( );
    return 0;
}

// window resize callback function implementation
void framebuffer_size_callback( GLFWwindow* window, int width, int height )
{
    // setting the size of the rendering window
    glViewport( 0, 0, width, height );
}

// processes the given inputs
void processInput( GLFWwindow* window )
{
    if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
    {
        glfwSetWindowShouldClose( window, true );
    }
    else if ( glfwGetKey( window, GLFW_KEY_1 ) == GLFW_PRESS )
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }
    else if ( glfwGetKey( window, GLFW_KEY_2 ) == GLFW_PRESS )
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    } 
    else if ( glfwGetKey( window, GLFW_KEY_3 ) == GLFW_PRESS )
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
    }
}