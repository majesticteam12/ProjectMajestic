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
	int  endProgram = 1;
	do {
		window Window("Majestic", 1280, 780);
		glClearColor(0.5f, 1.0f, 1.0f, 0.0f);

		if (glewInit() != GLEW_OK) {
			LOG("GLEW failed!");
		}

		int numbOfvertices;
		float positions[CAPACITY];
		int polytype, choice1, choice2;
		bool loopEffect = false, loopEffect2 = false;
		LOG("Do you want to render a prefabricate polygon(1) or to insert vertices by your own(2)?");
		INSERT(choice1);
		if (choice1 == 1) {
			LOG("Which polygon do you want to render?(triangle(1), rectangle(2), pentagon(3), hexagon(4), heptagon(5), octagon(6))");
			INSERT(polytype);
			switch (polytype) {
			case 1:
				positions[0] = -0.5f;
				positions[1] = -0.5f;
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
				positions[0] = -0.5f;
				positions[1] = -0.8f;
				positions[2] = -0.8f;
				positions[3] = 0.1f;
				positions[4] = 0.0f;
				positions[5] = 0.8f;
				positions[6] = 0.8f;
				positions[7] = 0.1f;
				positions[8] = 0.5f;
				positions[9] = -0.8f;
				numbOfvertices = 5;
				break;
			case 4:
				positions[0] = -0.3f;
				positions[1] = -0.6f;
				positions[2] = -0.8f;
				positions[3] = 0.0f;
				positions[4] = -0.3f;
				positions[5] = 0.6f;
				positions[6] = 0.3f;
				positions[7] = 0.6f;
				positions[8] = 0.8f;
				positions[9] = 0.0f;
				positions[10] = 0.3f;
				positions[11] = -0.6; 
				numbOfvertices = 6;
				break;
			case 5:
				positions[0] = -0.3f;
				positions[1] = -0.8f;
				positions[2] = -0.6f;
				positions[3] = -0.3f;
				positions[4] = -0.5f;
				positions[5] = 0.4f;
				positions[6] = 0.0f;
				positions[7] = 0.9f;
				positions[8] = 0.5f;
				positions[9] = 0.4f;
				positions[10] = 0.6f;
				positions[11] = -0.3f;
				positions[12] = 0.3f;
				positions[13] = -0.8f;
				numbOfvertices = 7;
				break;
			case 6:
				positions[0] = -0.3f;
				positions[1] = -0.8f;
				positions[2] = -0.6f;
				positions[3] = -0.4f;
				positions[4] = -0.6f;
				positions[5] = 0.4f;
				positions[6] = -0.3f;
				positions[7] = 0.8f;
				positions[8] = 0.3f;
				positions[9] = 0.8f;
				positions[10] = 0.6f;
				positions[11] = 0.4f;
				positions[12] = 0.6f;
				positions[13] = -0.4f;
				positions[14] = 0.3f;
				positions[15] = -0.8f;
				numbOfvertices = 8;
				break;
			default:
				//LOG("ERROR");
				break;
			}
		}
		else if (choice1 == 2) {
			LOG("How many vertices do you want to insert?(MAX 10)")
			INSERT(numbOfvertices);
			int n = 0;
			for (int i = 0; i < numbOfvertices; i++) {
				getchar();
				std::cout << "Vertex " << i + 1 << std::endl,
					std::cout << "X:";
				INSERT(positions[n]);
				LOG(";");
				std::cout << "Y:";
				INSERT(positions[n + 1]);
				n += 2;
			}

		}

		float r, g, b;
		int choice3;
		LOG("Do you want to select a prefabricate shader(1) or to insert values by your own?(2)")
			INSERT(choice3);
		if (choice3 == 1) {
			LOG("Select a shader red(1), green(2), blue(3), flashlight(4), multicolor(5)");
			INSERT(choice2);
			switch (choice2) {
			case 1:
				r = 1.0f;
				g = 0;
				b = 0;
				break;
			case 2:
				r = 0;
				g = 1;
				b = 0;
				break;
			case 3:
				r = 0;
				g = 0;
				b = 1;
				break;
			case 4:
				r = 0.8f;
				g = 0.3f;
				b = 0.8f;
				loopEffect = true;
				break;
			case 5:
				r = 0.8f;
				g = 0.3f;
				b = 0.8f;
				loopEffect2 = true;
				break;
			default:
				break;

			}
		}
		else if (choice3 == 2) {
			LOG("Insert color values(max 1.0) per color");
			LOG("Red:");
			INSERT(r);
			std::cout << std::endl;
			LOG("Green:");
			INSERT(g);
			std::cout << std::endl;
			LOG("Blue:");
			INSERT(b);
			std::cout << std::endl;

		}




		unsigned int indices[] = { //Indices

			0, 1, 2,
			0, 2, 3,
			0, 3, 4,
			0, 4, 5,
			0, 5, 6,
			0, 6, 7,
			0, 7, 8,
			0, 8, 9


		};

		VertexBuffer vb(positions, numbOfvertices * 2 * sizeof(float));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

		//Index Buffer 
		IndexBuffer ib(indices, 24);


		//Shader
		ShaderProgramSource source = ParseShader("Basic.shader");
		unsigned int shader = CreateShader(source.VertexShader, source.FragmentShader);
		glUseProgram(shader);

		int location = glGetUniformLocation(shader, "u_color");
		ASSERT(location != -1);
		glUniform4f(location, r, g, b, 1.0f);

		float coolDown = 0.0f;
		float increment = 0.05f;
		LOG("Close the window to procede.");
		while (!Window.Closed())
		{
			Window.Clear();
			if (loopEffect) {
				glUniform4f(location, coolDown, coolDown, coolDown, 1.0f);

				if (coolDown > 1.0f)
					increment = -0.05f;
				else if (coolDown < 0.0)
					increment = 0.05f;

				coolDown += increment;
			}
			else if (loopEffect2) {
				glUniform4f(location, coolDown, coolDown, 0.3f, 1.0f);

				if (coolDown > 1.0f)
					increment = -0.05f;
				else if (coolDown < 0.0)
					increment = 0.05f;

				coolDown += increment;
			}

			glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, nullptr);
			ib.Bind();


			Window.update();

		}

		glDeleteProgram(shader);
		glfwTerminate();

		LOG("Do you want to render another object? Yes(1), No(2)");
		INSERT(endProgram);
	}while (endProgram == 1);
	
	return 0;
}
