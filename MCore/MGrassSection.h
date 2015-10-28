/*
*	GrassSection
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGrass.h"
#include "MGrassRenderer.h"
#include "MWorldSection.h"

namespace Rad {

	class M_ENTRY GrassSection : public ISectionData
	{
		DECLARE_RTTI();

	public:
		GrassSection();
		virtual ~GrassSection();

		virtual bool
			Init(WorldSection * section, const rml_node * def);

		virtual void
			Update(float elapsedTime) {}

		virtual void
			Serialize(Serializer & sl);

		const GrassTemplate * 
			GetTemplate(int id);
		int 
			GetTemplateCount();

		void 
			AddInstance(const GrassInstance & gi);
		void 
			RemoveInstance(int id, const RectF & rect);
		int 
			GetInstanceCount();
		void 
			_getInstance(Array<const GrassInstance *> & garray, float x, float z, float radius);
		void 
			_getInstanceEx(Array<const GrassInstance *> & garray, const Float3 & eye, const Float3 & dir, float radius);
		
		TexturePtr
			_getTexture() { return mTexture; }

		GrassBlock * 
			_getBlock(int i);
		GrassBlock *
			_getBlock(int i, int j);
		const Int2 &
			_getBlockCount() { return mBlockCount; }

	protected:
		Int2 mBlockCount;
		GrassBlock * mBlockArray;

		TexturePtr mTexture;
		Array<GrassTemplate> mTempArray;

		GrassRenderer * mRenderer;
	};
}