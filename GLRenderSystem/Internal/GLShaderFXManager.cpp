#include "GLShaderFxManager.h"
#include "MResourceManager.h"
#include "MHWBufferManager.h"

namespace Rad {

	inline Float4 _getFloat4(const char * str)
	{
		float r = 0, g = 0, b = 0, a = 0;

		str = str_getfloat(r, str);
		str = str_getfloat(g, str);
		str = str_getfloat(b, str);
		str = str_getfloat(a, str);

		return Float4(r, g, b, a);
	}

	inline eFillMode _getFillMode(const char * str)
	{
		const EnumDesc * ed = eFillMode::Instance()->GetEnum(str);
		if (ed != NULL)
		{
			return (eFillMode::enum_t)ed->val;
		}

		d_log("?: Unknown FillMode '%s'.", str);

		return eFillMode::SOLID;
	}

	inline eCullMode _getCullMode(const char * str)
	{
		const EnumDesc * ed = eCullMode::Instance()->GetEnum(str);
		if (ed != NULL)
		{
			return (eCullMode::enum_t)ed->val;
		}

		d_log("?: Unknown CullMode '%s'.", str);

		return eCullMode::BACK;
	}

	inline eDepthMode _getDepthMode(const char * str)
	{
		const EnumDesc * ed = eDepthMode::Instance()->GetEnum(str);
		if (ed != NULL)
		{
			return (eDepthMode::enum_t)ed->val;
		}

		d_log("?: Unknown DepthMode '%s'.", str);

		return eDepthMode::LESS_EQUAL;
	}

	inline eBlendMode _getBlendMode(const char * str)
	{
		const EnumDesc * ed = eBlendMode::Instance()->GetEnum(str);
		if (ed != NULL)
		{
			return (eBlendMode::enum_t)ed->val;
		}

		d_log("?: Unknown BlendMode '%s'.", str);

		return eBlendMode::OPACITY;
	}

	inline eColorMode _getColorMode(const char * str)
	{
		const EnumDesc * ed = eColorMode::Instance()->GetEnum(str);
		if (ed != NULL)
		{
			return (eColorMode::enum_t)ed->val;
		}

		d_log("?: Unknown ColorMode '%s'.", str);

		return eColorMode::ALL;
	}

	inline eSPType _getSPType(const char * str)
	{
		const EnumDesc * ed = eSPType::Instance()->GetEnum(str);
		if (ed != NULL)
		{
			return (eSPType::enum_t)ed->val;
		}

		d_log("?: Unknown SPType '%s'.", str);

		return eSPType::FLOAT4;
	}

	inline eSamplerBindType _getSamplerBindType(const char * str)
	{
		const EnumDesc * ed = eSamplerBindType::Instance()->GetEnum(str);
		if (ed != NULL)
		{
			return (eSamplerBindType::enum_t)ed->val;
		}

		d_log("?: Unknown SamplerBindType '%s'.", str);

		return eSamplerBindType::NONE;
	}

	inline eTexFilter _getTexFilter(const char * str)
	{
		const EnumDesc * ed = eTexFilter::Instance()->GetEnum(str);
		if (ed != NULL)
		{
			return (eTexFilter::enum_t)ed->val;
		}

		d_log("?: Unknown TexAddress '%s'.", str);

		return eTexFilter::DEFAULT;
	}

	inline eTexAddress _getTexAddress(const char * str)
	{
		const EnumDesc * ed = eTexAddress::Instance()->GetEnum(str);
		if (ed != NULL)
		{
			return (eTexAddress::enum_t)ed->val;
		}

		d_log("?: Unknown TexAddress '%s'.", str);

		return eTexAddress::WRAP;
	}

	inline eVertexSemantic _getVertexSemantic(const char * str)
	{
		const EnumDesc * ed = eVertexSemantic::Instance()->GetEnum(str);
		if (ed != NULL)
		{
			return (eVertexSemantic::enum_t)ed->val;
		}

		d_log("?: Unknown VertexSemantic '%s'.", str);

		return eVertexSemantic::POSITION;
	}

	void _loadState(rml_node * node, GLShaderPass * pass)
	{
		RenderState * state = pass->GetRenderState();

		rml_node * child = node->first_node("FillMode");
		if (child != NULL)
		{
			state->FillMode = _getFillMode(child->value());
		}

		child = node->first_node("CullMode");
		if (child != NULL)
		{
			state->CullMode = _getCullMode(child->value());
		}

		child = node->first_node("DepthMode");
		if (child)
		{
			state->DepthMode = _getDepthMode(child->value());
		}

		child = node->first_node("BlendMode");
		if (child)
		{
			state->BlendMode = _getBlendMode(child->value());
		}

		child = node->first_node("ColorMode");
		if (child)
		{
			state->ColorMode = _getColorMode(child->value());
		}
	}

	void _loadUniform(rml_node * node, GLShaderPass * pass, ShaderFX * fx)
	{
		GL_Uniform * uniform = new GL_Uniform;
		uniform->Name = node->name();

		const char * str = node->value();
		char temp[128], val_str[128];

		if (*str)
		{
			str = str_substring(temp, sizeof(temp), str);
			uniform->Type = _getSPType(temp);

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
				if (*str2 != '#')
				{
					uniform->Count = atoi(str2);
				}
				else
				{
					const char * m = strstr(fx->GetMacros().c_str(), str2 + 1);
					if (m != NULL)
					{
						m = str_substring(idstr, sizeof(idstr), m);
						m = str_substring(idstr, sizeof(idstr), m);

						uniform->Count = atoi(idstr);
						d_assert (uniform->Count > 0);
					}
					else
					{
						d_log("!: Uniform '%s' count macro '%s' invalid.", uniform->Name.c_str(), str2);
						uniform->Count = 1;
					}
				}
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
					Float4 val = _getFloat4(val_str);
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

		child = node->first_node("Index");
		if (child != NULL)
		{
			sampler->Index = atoi(child->value());
		}

		child = node->first_node("BindType");
		if (child)
		{
			sampler->BindType = _getSamplerBindType(child->value());
		}

		child = node->first_node("BindId");
		if (child)
		{
			sampler->BindId = atoi(child->value());
		}

		child = node->first_node("Filter");
		if (child)
		{
			sampler->Filter = _getTexFilter(child->value());
		}

		child = node->first_node("Address");
		if (child)
		{
			sampler->Address = _getTexAddress(child->value());
		}

		child = node->first_node("BorderColor");
		if (child)
		{
			sampler->BorderColor = _getFloat4(child->value());
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
		eVertexSemantic semantic = _getVertexSemantic(node->value());

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
				_loadUniform(child, pShaderPass, fx);

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

	void _loadImp(ShaderFX * fx, DataStreamPtr stream)
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

	//
	struct GLSL_VERSION
	{
		int ver;
		const char * str;
	};

	GLShaderFXManager::GLShaderFXManager()
	{
		const GLSL_VERSION glv[] = {
			{M_OPEN_GL_2_0, "#version 110 \n"},
			{M_OPEN_GL_2_1, "#version 120 \n"},
			{M_OPEN_GL_3_0, "#version 130 \n"},
			{M_OPEN_GL_3_1, "#version 140 \n"},
			{M_OPEN_GL_3_2, "#version 150 \n"},
			{M_OPEN_GL_3_3, "#version 330 \n"},
			{M_OPEN_GL_4_0, "#version 400 \n"},
			{M_OPEN_GL_4_1, "#version 410 \n"},
			{M_OPEN_GL_4_2, "#version 420 \n"},
			{M_OPEN_GL_4_3, "#version 430 \n"},

			{M_OPEN_GLES_2_0, "#version 100 \n"},
			{M_OPEN_GLES_3_0, "#version 300 \n"}
		};

		AddGlobalMacro(
			"#define D_OGL\n" \
			"#define float2 vec2\n" \
			"#define float3 vec3\n" \
			"#define float4 vec4\n" \
			"#define tex2D texture2D\n");

		AddGlobalMacro(
			"float3 transform3(float3 n, mat4 m) \n" \
			"{ \n" \
			"	float3 rn; \n" \
			"	rn.x = n.x * m[0].x + n.y * m[1].x + n.z * m[2].x; \n" \
			"	rn.y = n.x * m[0].y + n.y * m[1].y + n.z * m[2].y; \n" \
			"	rn.z = n.x * m[0].z + n.y * m[1].z + n.z * m[2].z; \n" \
			"	return rn; \n" \
			"} \n");

		AddGlobalMacro(
			"#define transform4(v, m) (m * v) \n");
	}

	GLShaderFXManager::~GLShaderFXManager()
	{
		for (int i = 0; i < mFXMap.Size(); ++i)
		{
			delete mFXMap[i].value;
		}

		mFXMap.Clear();
	}

	ShaderFX * GLShaderFXManager::_find(Hash2 hash, const String & name)
	{
		int i = mFXMap.Find(hash);

		d_assert (i == -1 || name == mFXMap[i].value->GetName());

		return i != -1 ? mFXMap[i].value : NULL;
	}

	ShaderFX * GLShaderFXManager::Load(const String & name, const String & source, const String & macros)
	{
		Hash2 hash(name.c_str());

		ShaderFX * pShaderFX = _find(hash, name);
		if (pShaderFX != NULL)
		{
			return pShaderFX;
		}

		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source);
		if (stream == NULL)
		{
			d_log("?: ShaderFX '%s' Open Failed.", source.c_str());
			return NULL;
		}

		pShaderFX = new ShaderFX(name, stream->GetSource(), macros);

		_loadImp(pShaderFX, stream);

		mFXMap.Insert(hash, pShaderFX);

		return pShaderFX;
	}

	ShaderFX * GLShaderFXManager::Load(const String & name, DataStreamPtr stream, const String & macros)
	{
		Hash2 hash(name.c_str());

		ShaderFX * pShaderFX = _find(hash, name);
		if (pShaderFX != NULL)
		{
			return pShaderFX;
		}

		pShaderFX = new ShaderFX(name, stream->GetSource(), macros);

		_loadImp(pShaderFX, stream);

		mFXMap.Insert(hash, pShaderFX);

		return pShaderFX;
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

	void GLShaderFXManager::Remove(ShaderFX * fx)
	{
		Hash2 hash(fx->GetName().c_str());
		int i = mFXMap.Find(hash);

		d_assert (i != -1);

		delete mFXMap[i].value;
		mFXMap.Erase(i);
	}

	void GLShaderFXManager::Reload(ShaderFX * fx)
	{
		if (fx->GetSource() != "")
		{
			fx->Clear();

			DataStreamPtr stream = ResourceManager::Instance()->OpenResource(fx->GetSource());
			if (stream == NULL)
			{
				d_log("?: ShaderFX '%s' Open Failed.", fx->GetSource().c_str());
			}

			_loadImp(fx, stream);
		}
	}

	void GLShaderFXManager::ReloadAll()
	{
		for (int i = 0; i < mFXMap.Size(); ++i)
		{
			Reload(mFXMap[i].value);
		}

		E_ReloadAll();
	}

}