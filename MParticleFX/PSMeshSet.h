/*
*	PS_MeshSet
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"
#include "PSMesh.h"
#include "PSShader.h"

namespace Rad {

	struct PS_MeshType : public IEnum
	{
		DECLARE_ENUM(PS_MeshType);

		enum enum_t {
			FILE_NAME,
			SPHERE,
			CLINDER,
			UP_CONE,
			DN_CONE,
		};
	};

	class FX_ENTRY PS_MeshSet : public PS_Set
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Set);

	protected:
		FixedString32 mShaderClass;
		bool mShaderEnable;
		int mBlendMode;
		int mType;
		Float3 mCenter;
		float mRadius;
		float mHeight;
		int mRings;
		int mSegments;
		String mFilename;
		String mTexture;

	public:
		PS_MeshSet();
		virtual ~PS_MeshSet();

		virtual void 
			OnPropertyChanged(const Property * p);

		virtual void 
			AllocQuota(int quota);
		virtual void 
			AddRenderQueue(RenderQueue * rq);

		virtual void
			Update(float elapsedTime);

		void 
			SetShaderClass(const FixedString32 & name);
		const FixedString32 & 
			GetShaderClass() const { return mShaderClass; }

		void
			SetShaderEnable(bool enable) { mShaderEnable = enable; }
		bool
			GetShaderEnable() const { return mShaderEnable; }

		void 
			SetBlendMode(int blendMode) { mBlendMode = blendMode; }
		int  
			GetBlendMode() { return mBlendMode; }

		void
			SetType(int type) { mType = type; }
		int
			GetType() const { return mType; }

		void
			SetCenter(const Float3 & center) { mCenter = center; }
		const Float3 &
			GetCenter() const { return mCenter; }

		void
			SetRadius(float radius) { mRadius = radius; }
		float
			GetRadius() const { return mRadius; }

		void
			SetHeight(float height) { mHeight = height; }
		float
			GetHeight() const { return mHeight; }

		void
			SetRings(int rings) { mRings = rings; }
		int
			GetRings() const { return mRings; }

		void
			SetSegments(int segments) { mSegments = segments; }
		int
			GetSegments() const { return mSegments; }

		void 
			SetFilename(const String & name);
		const String &
			GetFilename() const { return mFilename; }
		MeshSourcePtr
			_getMeshSource() { return i_mesh; }

		void
			SetTexture(const String & name);
		const String &
			GetTexture() const { return mTexture; }
		TexturePtr
			_getTexture() { return i_texture; }

		Mesh *
			_createMesh();

	protected:
		virtual Particle *	
			_quest_particle();
		virtual void		
			_free_particle(Particle * p);
		virtual void 
			_clear_particle();

		Mesh *
			_createSphere();
		Mesh *
			_createClinder();
		Mesh *
			_createCone(bool up);

	protected:
		TexturePtr i_texture;
		MeshSourcePtr i_mesh;
	};
}