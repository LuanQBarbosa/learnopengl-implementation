#include <iostream>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "learnopengl-implementation/shader.h"

void processInput( GLFWwindow* );
void framebuffer_size_callback( GLFWwindow*, int, int );

// settings
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
float mixValue = 0.2f;

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

    // creating a shader object
    Shader ourShader( "/home/ryuugami/Projects/C++/learnopengl-implementation/src/shader.vs", 
                      "/home/ryuugami/Projects/C++/learnopengl-implementation/src/shader.fs" );

    // triangle vertices in normalized device coordinates
    float vertices[] = {
        // positions            // colors           // texture coords
         0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left 
        -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
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
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );
    // teaching OpenGL how it should interpret the Vertex Data (fourth)
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0 );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)) );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)) );
    glEnableVertexAttribArray( 2 );
    // unbiding the current Vertex Buffer Object
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // creating and biding multiple textures
    unsigned int texture1, texture2;
    glGenTextures( 1, &texture1 );
    glGenTextures( 1, &texture2 );

    // activating the first texture unit to bind to it
    glBindTexture( GL_TEXTURE_2D, texture1 );
    // setting texture wrap
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    // setting texture scaling
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    // setting mipmap filtering method
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // setting texture border color
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor );

    // loading image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load( true );
    unsigned char *data = stbi_load( "/home/ryuugami/Projects/C++/learnopengl-implementation/textures/container.jpg", 
                                    &width, &height, &nrChannels, 0 );

    if ( data )
    {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
        glGenerateMipmap( GL_TEXTURE_2D );
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free( data );

    // activating the second texture unit to bind to it
    glBindTexture( GL_TEXTURE_2D, texture2 );
    // setting texture wrap
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
    // setting texture scaling
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    // setting mipmap filtering method
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // setting texture border color
    glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor );

    // loading image
    data = stbi_load( "/home/ryuugami/Projects/C++/learnopengl-implementation/textures/awesomeface.png", 
                        &width, &height, &nrChannels, 0 );

    if ( data )
    {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
        glGenerateMipmap( GL_TEXTURE_2D );
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free( data );

    // telling to which texture unit each shader sampler belongs to
    ourShader.use( );
    glUniform1i( glGetUniformLocation( ourShader.ID, "texture1" ), 0 );
    ourShader.setInt( "texture2", 1 );

    // initializing render loop
    while ( !glfwWindowShouldClose( window ) )
    {
        // input processing
        processInput( window );

        // rendering commands
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );        

        // activating the Shader Program
        ourShader.use( );
        ourShader.setFloat( "mixValue", mixValue );

        // activating and binding each texture unit
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, texture1 );
        glActiveTexture( GL_TEXTURE1 );
        glBindTexture( GL_TEXTURE_2D, texture2 );

        // rotating our container each game loop
        float time = (float)glfwGetTime( );
        glm::mat4 transform = glm::mat4( 1.0f );
        transform = glm::translate( transform, glm::vec3( 0.5f, -0.5f, 0.0f ) );
        transform = glm::rotate( transform, time, glm::vec3( 0.0f, 0.0f, 1.0f ) );
        
        unsigned int transformLoc = glGetUniformLocation( ourShader.ID, "transform" );
        glUniformMatrix4fv( transformLoc, 1, GL_FALSE, glm::value_ptr( transform ) );

        // rendering triangle             
        glBindVertexArray( VAO );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
        // glDrawArrays( GL_TRIANGLES, 0, 3 );

        // translating and scaling a second container
        transform = glm::mat4( 1.0f );
        transform = glm::translate( transform, glm::vec3( -0.5f, 0.5f, 0.0f ) );
        transform = glm::scale( transform, glm::vec3( sin( time ), sin( time ), 0.0f ) );

        glUniformMatrix4fv( transformLoc, 1, GL_FALSE, &transform[0][0] );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
        
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
    else if ( glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS )
    {
        mixValue += 0.0001f;
        if ( mixValue >= 1.0f )
            mixValue = 1.0f;
    }
    else if ( glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS )
    {
        mixValue -= 0.0001f;
        if ( mixValue <= 0.0f )
            mixValue = 0.0f;
    }
}