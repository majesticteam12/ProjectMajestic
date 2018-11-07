// OpenGl.cpp: definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include <iostream>
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <string>
#include "window.h"

#define LOG(x) std::cout<<x;
#define ASSERT(x) if(!(x)) __debugbreak();

struct ShaderProgramSource {

	std::string VertexShader;
	std::string FragmentShader;

};

unsigned int CompileShader(const std::string& source, unsigned int type) {

	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {

		int lenght;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
		char* message = (char*)alloca(lenght * sizeof(char));
		glGetShaderInfoLog(id, lenght, &lenght, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "shader") << "shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;

	}


	return id;

}

unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;

}

ShaderProgramSource ParseShader(const std::string& filePath) {

	std::ifstream stream(filePath);

	enum class ShaderType {

		NONE = -1, VERTEX = 0, FRAGMENT = 1

	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {

		if (line.find("#shader") != std::string::npos) {

			if (line.find("vertex") != std::string::npos) {

				type = ShaderType::VERTEX;

			}
			else if (line.find("fragment") != std::string::npos) {

				type = ShaderType::FRAGMENT;
				
			}

		}
		else {

			ss[(int)type] << line << '\n';
		}


	}

	return { ss[0].str(), ss[1].str() };
}

int main()
{
	window Window("Majestic", 1280, 780);
	glClearColor(0.5f, 1.0f, 1.0f, 0.0f);


	if (glewInit() != GLEW_OK) {
		LOG("GLEW failed!");
	}

	float positions[6] = { -0.5f, -0.5f, 0.0f,0.5f, 0.5f, -0.5f };

	//Triangle buffer
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	//Shader
	ShaderProgramSource source = ParseShader("Basic.shader");
	unsigned int shader = CreateShader(source.VertexShader, source.FragmentShader);
	glUseProgram(shader);

	int location = glGetUniformLocation(shader, "u_color");
	ASSERT(location != -1);
	glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f);

	float coolDown = 0.0f;
	float increment = 0.05f;
	while (!Window.Closed())
	{
		Window.Clear();
		glUniform4f(location, coolDown, 0.3f, 0.8f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		if (coolDown > 1.0f)
			increment = -0.05f;
		else if (coolDown < 0.0)
			increment = 0.05f;
		
		coolDown += increment;

		Window.update();

	}

	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}
