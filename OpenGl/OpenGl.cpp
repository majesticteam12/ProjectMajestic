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
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#define LOG(x) std::cout<<x<<std::endl;
#define INSERT(x) std::cin >> x;

#define CAPACITY 1000

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

	int numbOfvertices;
	float positions[CAPACITY];
	int polytype, choice1, choice2;
	LOG("Do you want to render a prefabricate polygon(1) or you want to insert vertices(2) by your own?");
	INSERT(choice1);
	if (choice1==1) {
		LOG("Which polygon do you want to render?(triangle(1), rectangle(2), pentagon(3))");
		INSERT(polytype);
		switch (polytype) {
		case 1:
			positions[0] = -0.5f;
			positions[1] = -0.5;
			positions[2] = 0.0f;
			positions[3] = 0.5f;
			positions[4] = 0.5f;
			positions[5] = -0.5;
			numbOfvertices = 3;
			break;
		case 2:
			positions[0] = -0.5f;
			positions[1] = -0.5;
			positions[2] = 0.5f;
			positions[3] = -0.5f;
			positions[4] = 0.5f;
			positions[5] = 0.5;
			positions[6] = -0.5f;
			positions[7] = 0.5f;
			numbOfvertices = 4;
			break;
		case 3:
			positions[0] = 0.0f;
			positions[1] = 0.5f;
			positions[2] = 0.5f;
			positions[3] = 0.25f;
			positions[4] = 0.25f;
			positions[5] = -0.5f;
			positions[6] = -0.25f;
			positions[7] = -0.5f;
			positions[8] = -0.5f;
			positions[9] = 0.25f;
			numbOfvertices = 5;
		default:
			LOG("ERROR");
			break;
		}
	}
	else if (choice1 == 2) {
		LOG("How many vertices do you want to insert?")
		INSERT(numbOfvertices);
		for (int i = 0; i < numbOfvertices; i++) {
			int n = 0;
			std::cout << "Vertice " << i + 1 << std::endl,
			std::cout << "X:";
			INSERT(positions[n]);
			LOG(";");
			std::cout << "Y:";
			INSERT(positions[n + 1]);
			n += 2;		
			getchar();
		}

	}

	float r, g, b, y;
	LOG("Select a shader (red(1), green(2), blue(3)");
	INSERT(choice2);
	switch (choice2){
	case 1:
		r = 1.0f;






	}
	
	


	unsigned int indices[] = { //Indices

		0, 1, 2,
		2, 3 ,0

	};

	VertexBuffer vb(positions, numbOfvertices * 2 * sizeof(float));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	//Index Buffer 
	IndexBuffer ib(indices, 6);
	

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
		glUniform4f(location, coolDown, 0.8f, 0.5f, 1.0f);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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
