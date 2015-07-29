/*!
 * @file Application.cpp
 *
 * @author Frank Hart
 * @date 30/01/2014
 *
 * [description]
 */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "Application.h"
#include <string>
#include <cstring>
#include "Matrix44.h"
#include "Vector4.h"
#include <math.h>

typedef Dg::Vector4<float> vec4;
typedef Dg::Matrix44<float> mat4;

Application * Application::s_instance(nullptr);

static void OnKeyCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
{
  Application::GetInstance()->OnKey(a_window, a_key, a_scancode, a_action, a_mods);
}

static void OnMouseMoveCallback(GLFWwindow* a_window, double a_x, double a_y)
{
  Application::GetInstance()->OnMouseMove(a_window, a_x, a_y);
}

Application * Application::GetInstance()
{
  if (s_instance == nullptr)
  {
    s_instance = new Application();
  }
  return s_instance;
}

/*
	Pre Condition: -None
	Post Condition:
		-Returns the ID of a compiled shader of the specified
		 type from the specified file
		-Reports error to console if file could not be found or compiled
	Side Effects:
		-None
*/
GLuint Application::LoadShaderFromFile(std::string a_path, GLenum a_shaderType)
{
	//Open file
	GLuint shaderID = 0;
	std::string shaderString;
	std::ifstream sourceFile( a_path.c_str() );

	//Source file loaded
	if (sourceFile)
	{
		//Get shader source
		shaderString.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());

		//Create shader ID
		shaderID = glCreateShader( a_shaderType );

		//Set shader source
		const GLchar* shaderSource = shaderString.c_str();
		glShaderSource( shaderID, 1, (const GLchar**)&shaderSource, NULL );

		//Compile shader source
		glCompileShader( shaderID );

		//Check shader for errors
		GLint shaderCompiled = GL_FALSE;
		glGetShaderiv( shaderID, GL_COMPILE_STATUS, &shaderCompiled );
		if( shaderCompiled != GL_TRUE )
		{
			printf( "Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource );
			glDeleteShader( shaderID );
			shaderID = 0;
		}
	}
	else
	{
		printf("Unable to open file %s\n", a_path.c_str());
	}

	return shaderID;
}


GLuint Application::QuadFullScreenVao()
{
  glGenVertexArrays(1, &m_vao);
  GLuint vbo(0);
  glGenBuffers(1, &vbo);
  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  char screenVertices[12] = {-1, -1,
                              1, -1,
                              1,  1, 
                              1,  1, 
                             -1,  1, 
                             -1, -1 };

  glBufferData(GL_ARRAY_BUFFER,
               sizeof(screenVertices),
               screenVertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_BYTE, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
  return m_vao;
}


GLuint Application::CreateQuadProgram()
{
  GLuint quadProgram = glCreateProgram();
  GLuint vshader = LoadShaderFromFile("quad_vs.glsl", GL_VERTEX_SHADER);
  GLuint fshader = LoadShaderFromFile("quad_fs.glsl", GL_FRAGMENT_SHADER);
  glAttachShader(quadProgram, vshader);
  glAttachShader(quadProgram, fshader);
  glBindAttribLocation(quadProgram, 0, "vertex");
  glBindFragDataLocation(quadProgram, 0, "color");
  glLinkProgram(quadProgram);
  GLint linked(0);
  glGetProgramiv(quadProgram, GL_LINK_STATUS, &linked);
  if (linked == 0)
  {
    GLchar buf[2048] = {};
    GLsizei length;
    glGetProgramInfoLog(quadProgram, 2048, &length, buf);
    printf(buf);
  }
  return quadProgram;
}


void Application::InitQuadProgram()
{
  glUseProgram(m_quadProgram);
  GLint texUniform = glGetUniformLocation(m_quadProgram, "tex");
  glUniform1i(texUniform, 0);
  glUseProgram(0);
}


GLuint Application::CreateComputeProgram()
{
  GLuint computeProgram = glCreateProgram();
  GLuint cshader = LoadShaderFromFile("raytracer_cs.glsl", GL_COMPUTE_SHADER);
  glAttachShader(computeProgram, cshader);
  glLinkProgram(computeProgram);
  GLint linked(0);
  glGetProgramiv(computeProgram, GL_LINK_STATUS, &linked);
  if (linked == 0)
  {
    GLchar buf[2048] = {};
    GLsizei length;
    glGetProgramInfoLog(computeProgram, 2048, &length, buf);
    printf(buf);
  }
  return computeProgram;
}


void Application::InitComputeProgram()
{
  glUseProgram(m_computeProgram);
  GLint workGroupSize[3] = {};
  glGetProgramiv(m_computeProgram, GL_COMPUTE_WORK_GROUP_SIZE, workGroupSize);
  m_workGroupSizeX = workGroupSize[0];
  m_workGroupSizeY = workGroupSize[1];
  m_eyeUniform = glGetUniformLocation(m_computeProgram, "eye");
  m_ray00Uniform = glGetUniformLocation(m_computeProgram, "ray00");
  m_ray10Uniform = glGetUniformLocation(m_computeProgram, "ray10");
  m_ray01Uniform = glGetUniformLocation(m_computeProgram, "ray01");
  m_ray11Uniform = glGetUniformLocation(m_computeProgram, "ray11");
  glUseProgram(0);
}


void Application::Init()
{
  //Init defaults
	strcpy_s(m_info.title, "Raytracer example");
	m_info.windowWidth = 800;
	m_info.windowHeight = 600;
	m_info.majorVersion = 4;
	m_info.minorVersion = 4;
	m_info.samples = 0;
	m_info.flags.all = 0;
	m_info.flags.cursor = 1;
	m_info.flags.fullscreen = 0;

  m_mouseX = 0.0;
  m_mouseY = 0.0;
  m_prevX = 0.0;
  m_prevY = 0.0;
  
  m_window = nullptr;

  //Init GLFW
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return;
  }

  //Init Window
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_info.majorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_info.minorVersion);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, m_info.samples);
  glfwWindowHint(GLFW_STEREO, m_info.flags.stereo ? GL_TRUE : GL_FALSE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  if (m_info.flags.fullscreen)
  {
    if (m_info.windowWidth == 0 || m_info.windowHeight == 0)
    {
      const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

      m_info.windowWidth = mode->width;
      m_info.windowHeight = mode->height;
    }
    m_window = glfwCreateWindow(m_info.windowWidth,
      m_info.windowHeight,
      m_info.title,
      glfwGetPrimaryMonitor(),
      NULL);
    glfwSwapInterval((int)m_info.flags.vsync);
  }
  else
  {
    m_window = glfwCreateWindow(m_info.windowWidth,
      m_info.windowHeight,
      m_info.title,
      NULL,
      NULL);
  }

  if (!m_window)
  {
    fprintf(stderr, "Failed to open m_window\n");
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1);

  //Set up the camera
  m_camera.SetScreen((m_info.windowWidth / m_info.windowHeight), 1.0f);

  //Set up the mouse
  glfwSetCursorPos(m_window, 0.0, 0.0);
  m_mouseX = 0.0;
  m_mouseY = 0.0;
  glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  //Set up keys
  glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);


  glfwSetKeyCallback(m_window, ::OnKeyCallback);
  glfwSetCursorPosCallback(m_window, ::OnMouseMoveCallback);
  m_info.flags.stereo = (glfwGetWindowAttrib(m_window, GLFW_STEREO) ? 1 : 0);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  // get version m_info
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // version as a string
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", version);

  // Create all needed GL resources
  m_tex = CreateFramebufferTexture();
  m_vao = QuadFullScreenVao();
  m_computeProgram = CreateComputeProgram();
  InitComputeProgram();
  m_quadProgram = CreateQuadProgram();
  InitQuadProgram();
}


GLuint Application::CreateFramebufferTexture()
{
  GLuint tex(0);
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  GLchar * black(nullptr);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_info.windowWidth, m_info.windowHeight, 0, GL_RGBA, GL_FLOAT, black);
  glBindTexture(GL_TEXTURE_2D, 0);
  return tex;
}


void Application::ShutDown()
{
}


void Application::OnKey(GLFWwindow* m_window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, GL_TRUE);
}


void Application::OnMouseMove(GLFWwindow* m_window, double x, double y)
{
  double dx = x - m_mouseX;
  double dy = y - m_mouseY;

  m_camera.UpdateYPR(-dx/100.0, -dy/100.0, 0.0);

  m_mouseX = x;
  m_mouseY = y;
}


void Application::DoInput()
{
  if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)         m_w = true;
  else if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_RELEASE)  m_w = false;

  if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)         m_s = true;
  else if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_RELEASE)  m_s = false;

  if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)         m_a = true;
  else if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_RELEASE)  m_a = false;

  if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)         m_d = true;
  else if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_RELEASE)  m_d = false;

  if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS)         m_r = true;
  else if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_RELEASE)  m_r = false;

  if (glfwGetKey(m_window, GLFW_KEY_F) == GLFW_PRESS)         m_f = true;
  else if (glfwGetKey(m_window, GLFW_KEY_F) == GLFW_RELEASE)  m_f = false;


  if (m_w) m_camera.MoveForward(0.1);
  if (m_s) m_camera.MoveForward(-0.1);
  if (m_a) m_camera.MoveLeft(0.1);
  if (m_d) m_camera.MoveLeft(-0.1);
  if (m_r) m_camera.MoveWorldUp(0.1);
  if (m_f) m_camera.MoveWorldUp(-0.1);
  
}


void Application::Trace()
{
  glUseProgram(m_computeProgram);

  vec4 ray00, ray01, ray10, ray11, eye;
  m_camera.GetCornerRays(ray00, ray01, ray10, ray11, eye);
  glUniform3f(m_eyeUniform, eye[0], eye[1], eye[2]);
  glUniform3f(m_ray00Uniform, ray00[0], ray00[1], ray00[2]);
  glUniform3f(m_ray01Uniform, ray01[0], ray01[1], ray01[2]);
  glUniform3f(m_ray10Uniform, ray10[0], ray10[1], ray10[2]);
  glUniform3f(m_ray11Uniform, ray11[0], ray11[1], ray11[2]);

  // Bind level 0 of framebuffer texture as writable image in the shader.
  glBindImageTexture(0, m_tex, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);

  // Compute appropriate invocation dimension.
  int worksizeX = Dg::NextPower2(m_info.windowWidth);
  int worksizeY = Dg::NextPower2(m_info.windowHeight);

  /* Invoke the compute shader. */
  glDispatchCompute(worksizeX / m_workGroupSizeX, worksizeY / m_workGroupSizeY, 1);

  /* Reset image binding. */
  glBindImageTexture(0, 0, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  glUseProgram(0);

  /*
  * Draw the rendered image on the screen using textured full-screen
  * quad.
  */
  glUseProgram(m_quadProgram);
  glBindVertexArray(m_vao);
  glBindTexture(GL_TEXTURE_2D, m_tex);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}


void Application::Run()
{
  //Init all systems and data
  Init();

  //Run the app
  while (glfwWindowShouldClose(m_window) == GL_FALSE) 
  {
    glfwPollEvents();
    glViewport(0, 0, m_info.windowWidth, m_info.windowHeight);

    DoInput();

    Trace();

    glfwSwapBuffers(m_window);
  }

  //Shut down and clean up.
	ShutDown();

	glfwDestroyWindow(m_window);
	glfwTerminate();
}
