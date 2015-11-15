#include "GLShaderFxManager.h"
#include "MResourceManager.h"
#include "MHWBufferManager.h"
#include "MUtil.h"

namespace Rad {

	void _loadState(rml_node * node, GLShaderPass * pass)
	{
		RenderState * state = pass->GetRenderState();

		rml_node * child = node->first_node("FillMode");
		if (child != NULL)
		{
			state->FillMode = util::getFillMode(child->value());
		}

		child = node->first_node("CullMode");
		if (child != NULL)
		{
			state->CullMode = util::getCullMode(child->value());
		}

		child = node->first_node("DepthMode");
		if (child)
		{
			state->DepthMode = util::getDepthMode(child->value());
		}

		child = node->first_node("BlendMode");
		if (child)
		{
			state->BlendMode = util::getBlendMode(child->value());
		}

		child = node->first_node("ColorMode");
		if (child)
		{
			state->ColorMode = util::getColorMode(child->value());
		}
	}

	void _loadUniform(rml_node * node, GLShaderPass * pass)
	{
		GL_Uniform * uniform = new GL_Uniform;
		uniform->Name = node->name();

		const char * str = node->value();
		char temp[128], val_str[128];

		if (*str)
		{
			str = str_substring(temp, sizeof(temp), str);
			uniform->Type = util::getSPType(temp);

			str = str_skipwhite(str);
		}

		val_str[0] = 0;
		while (*str)
		{
			str = str_substring2(temp, sizeof(temp), str, '[', ']');

			const char * str2 = temp;
			char idstr[128];
			str2 = str_substring(idstr, sizeof(idstr), str2);
			str2 = str_skipwhite(str2);

			if (strcmp(idstr, "index") == 0)
			{
				uniform->Index = atoi(str2);
			}
			else if (strcmp(idstr, "count") == 0)
			{
				uniform->Count = atoi(str2);
			}
			else if (strcmp(idstr, "data") == 0)
			{
				strcpy(val_str, str2);
			}
		}

		switch (uniform->Type)
		{
		case eSPType::FLOAT1:
		case eSPType::FLOAT2:
		case eSPType::FLOAT3:
		case eSPType::FLOAT4:
			{
				uniform->data = pool_alloc(sizeof(Float4) * uniform->Count);

				if (val_str[0])
				{
					Float4 val = util::getUniformValue(val_str);
					memcpy(uniform->data, &val, sizeof(Float4));
				}
				else
				{
					memset(uniform->data, 0, sizeof(Float4) * uniform->Count);
				}
			}
			break;

		case eSPType::MATRIX4:
			{
				uniform->data = pool_alloc(sizeof(Mat4) * uniform->Count);

				memset(uniform->data, 0, sizeof(Mat4) * uniform->Count);
			}
			break;
		}

		uniform->handle = glGetUniformLocation(pass->GetGLShaderProgram(), uniform->Name.c_str());
		if (uniform->handle != -1)
		{
			pass->AddUniform(uniform);
		}
		else
		{
			d_log("!: Uniform '%s' can't find.", uniform->Name.c_str());
			safe_pool_free(uniform->data);
			delete uniform;
		}
	}

	void _loadSampler(rml_node * node, GLShaderPass * pass, int index)
	{
		GL_Sampler * sampler = new GL_Sampler;
		sampler->Index = index;

		rml_node * child = node->first_node("Name");
		if (child)
		{
			sampler->Name = child->value();
		}

		child = node->first_node("BindType");
		if (child)
		{
			sampler->BindType = util::getSamplerBindType(child->value());
		}

		child = node->first_node("Filter");
		if (child)
		{
			sampler->Filter = util::getTexFilter(child->value());
		}

		child = node->first_node("Address");
		if (child)
		{
			sampler->Address = util::getTexAddress(child->value());
		}

		child = node->first_node("BorderColor");
		if (child)
		{
			sampler->BorderColor = util::getBorderColor(child->value());
		}

		child = node->first_node("Filename");
		if (child)
		{
			sampler->SamplerTexture = HWBufferManager::Instance()->LoadTexture(child->value(), -1);
		}

		if (sampler->Name != "")
			sampler->handle = glGetUniformLocation(pass->GetGLShaderProgram(), sampler->Name.c_str()); 
		else
			sampler->handle = -1;

		pass->AddSampler(sampler);
	}

	void _loadAttribute(rml_node * node, GLShaderPass * pass)
	{
		GLint handle = glGetAttribLocation(pass->GetGLShaderProgram(), node->name());
		eVertexSemantic semantic = util::getVertexSemantic(node->value());

		if (handle == -1)
		{
			d_log("!: Attribute '%s' can't find.", node->name());
		}
		
		if (semantic != eVertexSemantic::UNKNOWN)
			pass->SetAttribute(semantic, handle);
	}

	void _loadPass(rml_node * root, ShaderFX * fx)
	{
		rml_node * nodeVS = root->first_node("VertexShader");
		rml_node * nodePS = root->first_node("PixelShader");

		if (nodeVS == NULL || nodePS == NULL)
		{
			d_log("?: No Shader Program.");
			return ;
		}

		GLShaderProgram * shader = new GLShaderProgram(nodeVS->value(), nodePS->value(), fx);
		if (!shader->Build())
		{
			delete shader;
			return ;
		}

		GLShaderPass * pShaderPass = new GLShaderPass();
		pShaderPass->SetGLShaderProgram(shader);

		rml_node * node = root->first_node("State");
		if (node != NULL)
		{
			_loadState(node, pShaderPass);
		}

		node = root->first_node("Uniform");
		if (node != NULL)
		{
			rml_node * child = node->first_node();
			while (child != NULL)
			{
				_loadUniform(child, pShaderPass);

				child = child->next_sibling();
			}
		}

		node = root->first_node("Attribute");
		if (node != NULL)
		{
			rml_node * child = node->first_node();
			while (child != NULL)
			{
				_loadAttribute(child, pShaderPass);

				child = child->next_sibling();
			}
		}
		
		int index = 0;
		node = root->first_node("Sampler");
		while (node != NULL)
		{
			_loadSampler(node, pShaderPass, index++);

			node = node->next_sibling("Sampler");
		}

		fx->AddPass(pShaderPass);
	}


	//
	GLShaderFXManager::GLShaderFXManager()
	{
		AddGlobalMacro(
			"#define D_OGL\n" \
			"#define float2 vec2\n" \
			"#define float3 vec3\n" \
			"#define float4 vec4\n" \
			"#define tex2D texture2D\n" \
			"#define saturate(s) clamp(s, 0.0, 1.0)\n");
	}

	GLShaderFXManager::~GLShaderFXManager()
	{
	}

	void GLShaderFXManager::_loadImp(ShaderFX * fx, DataStreamPtr stream)
	{
		d_assert (stream != NULL);

		d_log("-: Loading ShaderFX '%s', source '%s'...", fx->GetName().c_str(), fx->GetSource().c_str());

		rml_doc rdoc;
		rdoc.open(stream);

		rml_node * pass = rdoc.first_node("Pass");
		while (pass != NULL)
		{
			_loadPass(pass, fx);

			pass = pass->next_sibling("Pass");
		}

		if (fx->GetPassCount() == 0)
		{
			d_log("?: ShaderFX '%s', Pass Count = 0.", fx->GetSource().c_str());
		}

		d_log("-: Loading ShaderFX OK...\n");
	}

	void GLShaderFXManager::OnLostDevice()
	{
		ShaderFX * pFX = NULL;
		for (int i = 0; i < mFXMap.Size(); ++i)
		{
			pFX = mFXMap[i].value;
			for (int i = 0; i < pFX->GetPassCount(); ++i)
			{
				GLShaderPass * pShaderPass = (GLShaderPass *)pFX->GetPass(i);
				pShaderPass->OnLostDevice();
			}
		}
	}

	void GLShaderFXManager::OnResetDevice()
	{
		ShaderFX * pFX = NULL;
		for (int i = 0; i < mFXMap.Size(); ++i)
		{
			pFX = mFXMap[i].value;
			for (int i = 0; i < pFX->GetPassCount(); ++i)
			{
				GLShaderPass * pShaderPass = (GLShaderPass *)pFX->GetPass(i);
				pShaderPass->OnResetDevice();
			}
		}
	}

}