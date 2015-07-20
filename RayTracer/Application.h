/*!
 * @file Application.h
 *
 * @author Frank Hart
 * @date 31/01/2014
 *
 * class declaration: Application
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

struct GLFWwindow;

/*!
 * @ingroup
 *
 * @class Application
 *
 * @brief [brief description]
 *
 * [detailed description]
 *
 * @author Frank Hart
 * @date
 */
class Application
{
private:
  static Application * s_instance;

  Application(const Application&);
  Application& operator= (const Application&);

  Application() : m_window(nullptr){}
  ~Application() {}

public:

  static Application * GetInstance();

	void Run();
	void Render(double currentTime);
	void OnResize(int w, int h);
	void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);
	void OnMouseButton(int button, int action);
  void OnMouseMove(GLFWwindow*, double x, double y);
	void OnMouseWheel(int pos);
	void GetMousePosition(int& x, int& y);

public:
	struct APPINFO
	{
		char title[128];
		int windowWidth;
		int windowHeight;
		int majorVersion;
		int minorVersion;
		int samples;
		union
		{
			struct
			{
				unsigned int    fullscreen : 1;
				unsigned int    vsync : 1;
				unsigned int    cursor : 1;
				unsigned int    stereo : 1;
				unsigned int    debug : 1;
			};
			unsigned int        all;
		} flags;
	};

private:

	APPINFO		    m_info;
	GLFWwindow*   m_window;
	GLuint        m_renderingProgram;
	GLuint        m_vao;
	GLuint        m_posBuffer;
	GLuint        m_indexBuffer;

  double m_mouseX;
  double m_mouseY; 
  double m_prevX; 
  double m_prevY;

	//static		Application* app;

	void init();
	void StartUp();
	void ShutDown();
	GLuint CompileShaders();
	GLuint LoadShaderFromFile(std::string path, GLenum shaderType);
};

#endif