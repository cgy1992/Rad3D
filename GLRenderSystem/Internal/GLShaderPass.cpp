#include "GLShaderPass.h"
#include "GLUtil.h"

namespace Rad {

	GLShaderPass::GLShaderPass()
		: ShaderPass()
	{
		for (int i = 0; i < mAttributes.Size(); ++i)
			mAttributes[i] = -1;
	}

	GLShaderPass::~GLShaderPass()
	{
		for (int i = 0; i < mUniforms.Size(); ++i)
		{
			safe_pool_free(mUniforms[i]->data);
			delete mUniforms[i];
		}

		for (int i = 0; i < mSamplers.Size(); ++i)
		{
			delete mSamplers[i];
		}

		mShaderProgram = NULL;
	}

	void GLShaderPass::UploadUniform()
	{
		RenderRegister * rr = RenderRegister::Instance();

		for (int i = 0; i < mUniforms.Size(); ++i)
		{
			GL_Uniform * u = (GL_Uniform *)mUniforms[i];

			switch (u->Type)
			{
			case eSPType::FLOAT1:
				glUniform1fv(u->handle, u->Count, (const GLfloat *)u->data);
				break;

			case eSPType::FLOAT2:
				glUniform2fv(u->handle, u->Count, (const GLfloat *)u->data);
				break;

			case eSPType::FLOAT3:
				glUniform3fv(u->handle, u->Count, (const GLfloat *)u->data);
				break;

			case eSPType::FLOAT4:
				glUniform4fv(u->handle, u->Count, (const GLfloat *)u->data);
				break;
			
			case eSPType::MATRIX4:
				glUniformMatrix4fv(u->handle, u->Count, GL_FALSE, (const GLfloat *)u->data);
				break;

			case eSPType::WORLD_MATRIX:
				glUniformMatrix4fv(u->handle, 1, GL_FALSE, rr->GetWorldTM().ToFloatPtr());
				break;

			case eSPType::VIEW_MATRIX:
				glUniformMatrix4fv(u->handle, 1, GL_FALSE, rr->GetViewTM().ToFloatPtr());
				break;

			case eSPType::PROJ_MATRIX:
				glUniformMatrix4fv(u->handle, 1, GL_FALSE, rr->GetProjTM().ToFloatPtr());
				break;

			case eSPType::WORLD_VIEW_MATRIX:
				glUniformMatrix4fv(u->handle, 1, GL_FALSE, rr->GetWorldViewTM().ToFloatPtr());
				break;

			case eSPType::VIEW_PROJ_MATRIX:
				glUniformMatrix4fv(u->handle, 1, GL_FALSE, rr->GetViewProjTM().ToFloatPtr());
				break;

			case eSPType::WORLD_VIEW_PROJ_MATRIX:
				glUniformMatrix4fv(u->handle, 1, GL_FALSE, rr->GetWorldViewProjTM().ToFloatPtr());
				break;

			case eSPType::BONE_MATRIX:
				glUniform4fv(u->handle, rr->GetBoneCount() * 3, (const float *)rr->GetBoneTM());
				break;

			case eSPType::CAMERA_POSITION:
				glUniform4fv(u->handle, 1, rr->GetCameraPosition().ToFloatPtr());
				break;

			case eSPType::CAMERA_DIR_VECTOR:
				glUniform4fv(u->handle, 1, rr->GetCameraDirVector().ToFloatPtr());
				break;

			case eSPType::CAMERA_RIGHT_VECTOR:
				glUniform4fv(u->handle, 1, rr->GetCameraRightVector().ToFloatPtr());
				break;

			case eSPType::CAMERA_UP_VECTOR:
				glUniform4fv(u->handle, 1, rr->GetCameraUpVector().ToFloatPtr());
				break;

			case eSPType::LIGHT_POSITION:
				glUniform4fv(u->handle, 1, rr->GetLightPosition().ToFloatPtr());
				break;

			case eSPType::LIGHT_DIRECTION:
				glUniform4fv(u->handle, 1, rr->GetLightDirection().ToFloatPtr());
				break;

			case eSPType::LIGHT_ATTEN_PARAM:
				glUniform4fv(u->handle, 1, rr->GetLightAttenParam().ToFloatPtr());
				break;

			case eSPType::LIGHT_SPOT_PARAM:
				glUniform4fv(u->handle, 1, rr->GetLightSpotParam().ToFloatPtr());
				break;

			case eSPType::EMISSIVE:
				glUniform4fv(u->handle, 1, rr->GetEmissive().ToFloatPtr());
				break;

			case eSPType::AMBIENT:
				glUniform4fv(u->handle, 1, rr->GetAmbient().ToFloatPtr());
				break;

			case eSPType::DIFFUSE:
				glUniform4fv(u->handle, 1, rr->GetDiffuse().ToFloatPtr());
				break;

			case eSPType::SPECULAR:
				glUniform4fv(u->handle, 1, rr->GetSpecular().ToFloatPtr());
				break;

			case eSPType::OPACITY:
				glUniform4fv(u->handle, 1, rr->GetOpacity().ToFloatPtr());
				break;

			case eSPType::FOG_COLOR:
				glUniform4fv(u->handle, 1, rr->GetFogColor().ToFloatPtr());
				break;

			case eSPType::FOG_PARAM:
				glUniform4fv(u->handle, 1, rr->GetFogParam().ToFloatPtr());
				break;

			case eSPType::SHADER_STATE:
				glUniform4fv(u->handle, 1, rr->GetShaderState().ToFloatPtr());
				break;

			case eSPType::TIME:
				glUniform4fv(u->handle, 1, rr->GetTime().ToFloatPtr());
				break;

			case eSPType::CLIP_PLANE:
				glUniform4fv(u->handle, 1, rr->GetClipPlane().ToFloatPtr());
				break;

			case eSPType::USER_CONST:
				glUniform4fv(u->handle, u->Count, (const float *)rr->GetUserConst(u->Index));
				break;
			}

			d_assert (glGetError() == 0);
		}
	}

	void GLShaderPass::BindSamplerState()
	{
		for (int i = 0; i < mSamplers.Size(); ++i)
		{
			GL_Sampler * s = (GL_Sampler *)mSamplers[i];

			if (s->handle != -1)
				glUniform1i(s->handle, s->Index);

			glActiveTexture(GL_TEXTURE0 + s->Index);

			TexturePtr t = RenderSystem::Instance()->_getCurrentTexture(s->Index);
			if (t == NULL)
				return ;

			eTexFilter Filter = s->Filter;
			eTexAddress Address = s->Address;

			if (Filter == eTexFilter::DEFAULT)
				Filter = HWBufferManager::Instance()->GetDefaultTextureFilter();

			if (t->GetMipmapLevel() > 0)
			{
				// OpenGL ES 2 can't specify mipmap level
#ifdef M_PLATFORM_WIN32
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, t->GetMipmapLevel());
#endif
				switch (Filter)
				{
				case eTexFilter::LINEAR:
				case eTexFilter::ANISOTROPIC:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					break;

				case eTexFilter::POINT:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					break;
				}
			}
			else
			{
				switch (Filter)
				{
				case eTexFilter::LINEAR:
				case eTexFilter::ANISOTROPIC:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					break;

				case eTexFilter::POINT:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					break;
				}
			}

			d_assert (glGetError() == 0);

			switch (Address)
			{
			case eTexAddress::WRAP:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				break;

			case eTexAddress::CLAMP:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				break;

			case eTexAddress::MIRROR:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
				break;

			case eTexAddress::BORDER:
				// OpenGL ES 2.0 not support!.

#ifdef M_PLATFORM_WIN32 
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (GLfloat *)&(s->BorderColor));
#else
				d_assert (0);
#endif
				break;
			}

			d_assert (glGetError() == 0);
		}
	}

	void GLShaderPass::SetAttribute(eVertexSemantic v, GLint handle)
	{
		mAttributes[v] = handle;
	}

	void GLShaderPass::AddUniform(GL_Uniform * pu)
	{
		mUniforms.PushBack(pu);
	}

	void GLShaderPass::AddSampler(GL_Sampler * ps)
	{
		mSamplers.PushBack(ps);
	}

	ShaderPass * GLShaderPass::Clone()
	{
		GLShaderPass * p = new GLShaderPass;

		// State
		p->mRenderState = mRenderState;

		// Uniform
		for (int i = 0; i < mUniforms.Size(); ++i)
		{
			GL_Uniform * u = (GL_Uniform *)mUniforms[i];

			p->AddUniform(u->Clone());
		}

		// Sampler
		for (int i = 0; i < mSamplers.Size(); ++i)
		{
			GL_Sampler * s = (GL_Sampler *)mSamplers[i];

			p->AddSampler(s);
		}

		// Attribute
		for (int i = 0; i < mAttributes.Size(); ++i)
		{
			p->SetAttribute((eVertexSemantic::enum_t)i, mAttributes[i]);
		}

		return p;
	}

	void GLShaderPass::OnLostDevice()
	{
		mShaderProgram->OnLostDevice();
	}

	void GLShaderPass::OnResetDevice()
	{
		mShaderProgram->OnResetDevice();
	}

}
