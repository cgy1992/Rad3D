/*
*	Light
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MNode.h"
#include "MTypes.h"

namespace Rad {

	class M_ENTRY Light : public Node
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(Node);

	protected:
		int mType;
		bool mStatic;
		bool mPreview;

		Float3 mAmbient;
		Float3 mDiffuse;
		Float3 mSpecular;

		float mAttenStart;
		float mAttenEnd;
		float mAttenFallOff;

		float mSpotInner;
		float mSpotOuter;
		float mSpotFallOff;

	public:
		Light(eLightType type = eLightType::POINT);
		virtual ~Light();

		virtual IObject *
			Clone();

		virtual void
			OnPropertyChanged(const Property * p);

		void			
			SetType(eLightType type);
		eLightType
			GetType() const { return (eLightType::enum_t)mType; }

		void 
			SetStatic(bool b);
		bool 
			IsStatic() const { return mStatic; }

		void 
			SetPreview(bool b);
		bool 
			IsPreview() const { return mPreview; }

		void
			SetAmbient(const Float3 & ambient);
		const Float3 &  
			GetAmbient() const { return mAmbient; }

		void
			SetDiffuse(const Float3 & diffuse);
		const Float3 &
			GetDiffuse() const { return mDiffuse; }

		void
			SetSpecular(const Float3 & specular);
		const Float3 &  
			GetSpecular() const { return mSpecular; }

		void            
			SetAttenParam(float start, float end, float fallOff);
		float			
			GetAttenStart() const { return mAttenStart; }
		float           
			GetAttenEnd() const { return mAttenEnd; }
		float           
			GetAttenFallOff() const { return mAttenFallOff; }
		const Float4 &		
			GetAttenParam() const { return mAttenParam;}

		void            
			SetSpotParam(float inner, float outer, float fallOff);
		float          
			GetSpotInner() const { return mSpotInner; }
		float           
			GetSpotOuter() const { return mSpotOuter; }
		float			
			GetSpotFallOff() const { return mSpotFallOff; }
		const Float4 &		
			GetSpotParam() const { return mSpotParam; }

		bool
			IsLighting() const { return IsVisible() && (!IsStatic() || IsPreview()); }

	protected:
		Float4 mAttenParam;
		Float4 mSpotParam;
	};

}