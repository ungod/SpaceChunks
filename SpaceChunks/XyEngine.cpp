#include "XyEngine.h"

XyEngine::XyEngine()
{
	srand(time(NULL));
	seed = rand() % 100;
}

void XyEngine::SetFunctions(void(*initFunc)(), void(*renderFunc)(), void(*inputFunc)(SDL_Event event))
{
	this->m_InitFunc = initFunc;
	this->m_RenderFunc = renderFunc;
	this->m_InputFunc = inputFunc;
	this->m_FunctionSet = true;
}

int XyEngine::CreateWindow(int width, int height, char* title, float frameRate)
{
	double StartTime = Time::GetTime();
	printf("[XYENGINE] XyEngine is Loading... \n");
	
	if (this->m_FunctionSet == false)
		return ReturnWithError("Functions Not Set!");

	this->m_FrameRate = frameRate;
	this->m_frameTime = 1.0 / this->m_FrameRate;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return ReturnWithError("SDL Failed To Init!");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

	m_Window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

	if (m_Window == NULL)
	{
		SDL_Quit();
		return ReturnWithError("SDL Failed To Create the WIndow!");
	}

	glcontext = SDL_GL_CreateContext(m_Window);

	if (glewInit() != GLEW_OK)
		return ReturnWithError("GLEW Failed to Init!");

	if (TTF_Init() != 0)
		return ReturnWithError("TTF Failed to Init!");

	if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG))
		return ReturnWithError("IMG Failed to Init!");

	m_running = true;

	LoadShaderProgram(m_posShader, "shaders/basicShader");

	

	double lastTime = Time::GetTime();
	double unprocessedTime = 0;
	double frameCounter = 0;
	frames = 0;

	glEnable(GL_TEXTURE_2D);

	m_RenderTexture = CreateTexture(windowWidth, windowHeight);
	//m_RenderTextureDepth = CreateTexture(windowWidth, windowHeight, true);

	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderTexture, 0);
	
	int i = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (i != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FrameBuffer Failed! Error: " << i << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	double EndTime = Time::GetTime() - StartTime;
	std::cout << "XyEngine Took " << EndTime << " Seconds to Load!" << std::endl;

	m_InitFunc();

	SDL_Event event;

	while (m_running)
	{
		bool render = false;

		double startTime = Time::GetTime();
		double passedTime = startTime - lastTime;
		lastTime = startTime;

		unprocessedTime += passedTime;
		frameCounter += passedTime;

		if (frameCounter >= 1.0)
		{
			fpsRate = frames;
			frames = 0;
			frameCounter = 0;
		}

		while (unprocessedTime > m_frameTime)
		{
			render = true;

			while (SDL_PollEvent(&event))
			{
				m_InputFunc(event);

				switch (event.type)
				{

				case SDL_QUIT:
					m_running = false;
					break;

				case SDL_MOUSEBUTTONDOWN:
					mousein = true;
					SDL_ShowCursor(SDL_DISABLE);
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						mousein = false;
						SDL_ShowCursor(SDL_ENABLE);
						break;
					}
				}
			}

			unprocessedTime -= m_frameTime;
		}

		if (render)
		{
			if (mousein)
				SDL_WarpMouseInWindow(m_Window, (int)MidX, (int)MidY);

			
			BindShader(m_posShader);
			m_pipeline.UpdateMatrices(m_posShader);

			//glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

			m_RenderFunc();
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
			
			//UnbindShader();
			SDL_GL_SwapWindow(m_Window);
			frames++;
		}
		else
		{
			SDL_Delay(1);
		}
	}


	return EXIT_SUCCESS;
}

int XyEngine::ReturnWithError(std::string err)
{
	std::cout << " " << std::endl;
	std::cout << "Error: " << err << std::endl;
	return EXIT_FAILURE;
}

void XyEngine::LoadShaderProgram(GLuint &shader, const std::string& fileName)
{
	shader = glCreateProgram();
	m_shaders[0] = ShaderUtil::CreateShader(ShaderUtil::LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = ShaderUtil::CreateShader(ShaderUtil::LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
		glAttachShader(shader, m_shaders[i]);

	glBindAttribLocation(shader, 0, "position");

	glLinkProgram(shader);
	ShaderUtil::CheckShaderError(shader, GL_LINK_STATUS, true, "Error: Shader Program Failed To Link: ");

	glValidateProgram(shader);
	ShaderUtil::CheckShaderError(shader, GL_LINK_STATUS, true, "Error: Shader Program Failed To Validate: ");
}

XyEngine::~XyEngine()
{
	if (m_Window != NULL)
	{
		printf("[CORE] XyEngine is shutting down... \n");
		TTF_Quit();
		IMG_Quit();
		SDL_GL_DeleteContext(glcontext);
		SDL_DestroyWindow(m_Window);
		SDL_Quit();
	}
}