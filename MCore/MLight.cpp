#include "MLight.h"
#include "MWorld.h"

namespace Rad {

	DF_PROPERTY_BEGIN(Light)
		DF_PROPERTY_ENUM(Light, mType, "", "Type", eLightType)
		DF_PROPERTY(Light, mStatic, "", "Static", PT_Bool)
		DF_PROPERTY(Light, mPreview, "", "Preview", PT_Bool)
		DF_PROPERTY_EX(Light, mDiffuse, "Transform", "Diffuse", "PT_Color", PT_Float3)
		DF_PROPERTY_EX(Light, mSpecular, "Transform", "Specular", "PT_Color",PT_Float3)
		DF_PROPERTY(Light, mAttenStart, "Attenuation", "Start", PT_Float)
		DF_PROPERTY(Light, mAttenEnd, "Attenuation", "End", PT_Float)
		DF_PROPERTY(Light, mAttenFallOff, "Attenuation", "FallOff", PT_Float)
		DF_PROPERTY(Light, mSpotInner, "SpotParam", "Inner", PT_Float)
		DF_PROPERTY(Light, mSpotOuter, "SpotParam", "Outer", PT_Float)
		DF_PROPERTY(Light, mSpotFallOff, "SpotParam", "FallOff", PT_Float)
	DF_PROPERTY_END();

	ImplementRTTI(Light, Node);

	Light::Light(eLightType type)
		: Node("Light")
		, mType(type)
		, mStatic(false)
		, mPreview(true)
		, mAmbient(0.5f, 0.5f, 0.5f)
		, mDiffuse(1, 1, 1)
		, mSpecular(0, 0, 0)
		, mAttenStart(0)
		, mAttenEnd(100)
		, mAttenFallOff(1)
		, mSpotInner(0)
		, mSpotOuter(45)
		, mSpotFallOff(1)
	{
		mAttenParam = Float4(0, 100, 0.01f, 1);
		mSpotParam = Float4(0.7071f, 1.0f, 1 / (1 - 0.7071f), 1);
	}

	Light::~Light()
	{
	}

	IObject * Light::Clone()
	{
		Light * light = new Light;

		light->Copy(this);

		return light;
	}

	void Light::OnPropertyChanged(const Property * p)
	{
		if (p->name == "mAttenStart" || p->name == "mAttenEnd" || p->name == "mAttenFallOff")
		{
			SetAttenParam(mAttenStart, mAttenEnd, mAttenFallOff);
		}
		else if (p->name == "mSpotInner" || p->name == "mSpotOuter" || p->name == "mSpotFallOff")
		{
			SetSpotParam(mSpotInner, mSpotOuter, mSpotFallOff);
		}

		IObject::OnPropertyChanged(p);
	}

	void Light::SetType(eLightType type)
	{
		mType = type;
	}

	void Light::SetStatic(bool b)
	{
		mStatic = b;
	}

	void Light::SetPreview(bool b)
	{
		mPreview = b;
	}

	void Light::SetAmbient(const Float3 & ambient)
	{
		mAmbient = ambient;
	}

	void Light::SetDiffuse(const Float3 & diffuse)
	{
		mDiffuse = diffuse;
	}

	void Light::SetSpecular(const Float3 & specular)
	{
		mSpecular = specular;
	}

	void Light::SetAttenParam(float start, float end, float falloff)
	{
		if (end > start)
		{
			mAttenStart = start;
			mAttenEnd = end;
			mAttenFallOff = falloff;

			mAttenParam.x = start;
			mAttenParam.y = end;
			mAttenParam.z = 1 / (end - start);
			mAttenParam.w = falloff;
		}
	}

	void Light::SetSpotParam(float inner, float outer, float falloff)
	{
		if (outer > inner)
		{
			mSpotInner = inner;
			mSpotOuter = outer;
			mSpotFallOff = falloff;

			float cosInner = Math::Cos(Math::DegreeToRadian(inner));
			float cosOuter = Math::Cos(Math::DegreeToRadian(outer));
			mSpotParam.x = cosOuter;
			mSpotParam.y = cosInner;
			mSpotParam.z = 1 / (cosInner - cosOuter);
			mSpotParam.w = mSpotFallOff;
		}
	}

}

