#pragma once

#include <fstream>
#include <string>
#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <format>

void compileShader(const GLuint& handle);

void linkShaders(const GLuint& program, const GLuint& vertexShader, const GLuint& fragmentShader);

auto read_file(std::string/*_view*/ path) -> std::string; // https://stackoverflow.com/questions/116038/how-do-i-read-an-entire-file-into-a-stdstring-in-c

struct Shader
{
	const GLuint handle, vertexShaderHandle, fragmentShaderHandle;

	Shader(const char* vertexFilename, const char* fragmentFilename);

	~Shader();

	void activate();
};
