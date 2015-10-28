#include "GLShaderProgram.h"
#include "MShaderFXManager.h"
#include "MLog.h"

namespace Rad {

	GLShaderProgram::GLShaderProgram(const char * vscode, const char * pscode, ShaderFX * fx)
		: mProgram(0)
		, mVertexShader(vscode)
		, mPixelShader(pscode)
		, mFX(fx)
	{
	}

	GLShaderProgram::~GLShaderProgram()
	{
		if (mProgram != 0)
			glDeleteProgram(mProgram);
	}

	bool GLShaderProgram::Build()
	{
		d_assert (mProgram == 0);

		String buffer;
		GLuint vertexShader = 0, pixelShader = 0;
		GLint success;

		// vertex shader
		buffer = ShaderFXManager::Instance()->GetGlobalMacroString();
		buffer += mFX->GetMacros();
		buffer += mVertexShader;

		const char * strVS = buffer.c_str();
		int lengthVS = buffer.Length();

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, (const char **)&strVS, &lengthVS);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char InfoLog[1024];
			glGetShaderInfoLog(vertexShader, sizeof(InfoLog), NULL, InfoLog);
			d_log("?: Compile vertex shader, %s.", InfoLog);

			return false;
		}

		// pixel shader
		buffer = ShaderFXManager::Instance()->GetGlobalMacroString();
		buffer += mFX->GetMacros();
		buffer += mPixelShader;

		const char * strPS = buffer.c_str();
		int lengthPS = buffer.Length();

		pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(pixelShader, 1, (const char **)&strPS, &lengthPS);
		glCompileShader(pixelShader);
		glGetShaderiv(pixelShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char InfoLog[1024];
			glGetShaderInfoLog(pixelShader, sizeof(InfoLog), NULL, InfoLog);
			d_log("?: Compile pixel shader, %s.", InfoLog);

			return false;
		}

		d_assert (vertexShader != NULL && pixelShader != NULL);

		// create program
		mProgram = glCreateProgram();

		glAttachShader(mProgram, vertexShader);
		glAttachShader(mProgram, pixelShader);

		glLinkProgram(mProgram);

		glGetProgramiv(mProgram, GL_LINK_STATUS, &success); 
		if (!success)
		{
			char ErrorLog[1024];
			glGetProgramInfoLog(mProgram, sizeof(ErrorLog), NULL, ErrorLog);

			d_log("?: linking shader,'%s'.", ErrorLog);

			glDeleteProgram(mProgram);
		}

		if (vertexShader)
			glDeleteShader(vertexShader);

		if (pixelShader)
			glDeleteShader(pixelShader);

		d_assert (glGetError() == 0);

		return mProgram != 0;
	}

	void GLShaderProgram::OnLostDevice()
	{
		if (mProgram != 0)
		{
			glDeleteProgram(mProgram);
			mProgram = 0;
		}
	}

	void GLShaderProgram::OnResetDevice()
	{
		Build();
	}

}