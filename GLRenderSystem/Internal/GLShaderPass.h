/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "GLInclude.h"
#include "GLShaderProgram.h"

namespace Rad {

	struct GL_Uniform : public FX_Uniform
	{
		GLint handle;

		GL_Uniform() : handle(-1) {}
		~GL_Uniform() {}

		GL_Uniform * Clone()
		{
			GL_Uniform * u = new GL_Uniform;

			u->handle = handle;

			u->Name = Name;
			u->Type = Type;
			u->Index = Index;
			u->Count = Count;

			switch (u->Type)
			{
			case eSPType::FLOAT4:
				{
					u->data = pool_alloc(sizeof(Float4) * u->Count);
					memcpy(u->data, data, sizeof(Float4) * u->Count);
				}
				break;

			case eSPType::MATRIX4:
				{
					u->data = pool_alloc(sizeof(Mat4) * u->Count);
					memcpy(u->data, data, sizeof(Mat4) * u->Count);
				}
				break;

			default:
				{
					u->data = NULL;
				}
				break;
			}

			return u;
		}
	};

	struct GL_Sampler : public FX_Sampler
	{
		GLint handle;

		GL_Sampler() : handle(-1) {}
		~GL_Sampler() {}

		GL_Sampler * Clone()
		{
			GL_Sampler * s = new GL_Sampler;

			s->handle = handle;

			s->Index = Index;
			s->Name = Name;
			s->BindType = BindType;
			s->Filter = Filter; 
			s->Address = Address;
			s->BorderColor = BorderColor;
			s->SamplerTexture = SamplerTexture;

			return s;
		}
	};

	

	class GLShaderPass : public ShaderPass
	{
		friend class GLShaderFXManager;

	public:
		GLShaderPass();
		virtual ~GLShaderPass();

		void 
			UploadUniform();
		void 
			BindSamplerState();

		void 
			AddUniform(GL_Uniform * pu);
		void 
			AddSampler(GL_Sampler * ps);

		void 
			SetGLShaderProgram(GLShaderProgramPtr p) { mShaderProgram = p; }
		GLuint 
			GetGLShaderProgram() { return mShaderProgram->GetProgram(); }

		void 
			SetAttribute(eVertexSemantic v, GLint handle);
		GLint 
			GetAttribute(eVertexSemantic v) { return mAttributes[v]; }

		virtual ShaderPass * 
			Clone();

		void 
			OnLostDevice();
		void 
			OnResetDevice();

	protected:
		GLShaderProgramPtr mShaderProgram;
		Field<GLint, eVertexSemantic::MAX> mAttributes;
	};

}
