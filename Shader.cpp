#include "Shader.h"

void compileShader(const GLuint& handle)
{
	glCompileShader(handle);

	GLint isCompiled = GL_FALSE;

	glGetShaderiv(handle, GL_COMPILE_STATUS, &isCompiled);
	if (!isCompiled)
	{
		GLint maxLength = 0;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(handle, maxLength, &maxLength, &errorLog[0]);

		throw std::runtime_error{ std::format("Error during compilation: {}", std::string{ errorLog.begin(), errorLog.end()}) };
	}
}

void linkShader(const GLuint& program, const GLuint& vertexShader, const GLuint& fragmentShader)
{
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	GLint success = GL_FALSE;

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);

		throw std::runtime_error{ std::format("Error while linking shader: {}", std::string{errorLog.begin(), errorLog.end()}) };
	}
}

auto read_file(std::string_view path) -> std::string {
	constexpr auto read_size = std::size_t(4096);
	auto stream = std::ifstream(path.data());
	stream.exceptions(std::ios_base::badbit);

	if (not stream) {
		throw std::ios_base::failure("file does not exist");
	}

	auto out = std::string();
	auto buf = std::string(read_size, '\0');
	while (stream.read(&buf[0], read_size)) {
		out.append(buf, 0, stream.gcount());
	}
	out.append(buf, 0, stream.gcount());
	return out;
}

Shader::Shader(const char* vertexFilename, const char* fragmentFilename) : handle(glCreateProgram()), vertexShaderHandle(glCreateShader(GL_VERTEX_SHADER)), fragmentShaderHandle(glCreateShader(GL_FRAGMENT_SHADER))
{
	std::string vertexShaderSource = read_file(vertexFilename);
	std::string fragmentShaderSource = read_file(fragmentFilename);
	const char* szVertexShaderSource = vertexShaderSource.c_str();
	const char* szFragmentShaderSource = fragmentShaderSource.c_str();
	glShaderSource(vertexShaderHandle, 1, &szVertexShaderSource, nullptr);
	glShaderSource(fragmentShaderHandle, 1, &szFragmentShaderSource, nullptr);
	compileShader(vertexShaderHandle);
	compileShader(fragmentShaderHandle);
	linkShader(handle, vertexShaderHandle, fragmentShaderHandle);
}

Shader::~Shader()
{
	glDeleteProgram(handle);
	glDeleteShader(vertexShaderHandle);
	glDeleteShader(fragmentShaderHandle);
	glDetachShader(handle, vertexShaderHandle);
	glDetachShader(handle, fragmentShaderHandle);
}

void Shader::activate()
{
	glUseProgram(handle);
}
