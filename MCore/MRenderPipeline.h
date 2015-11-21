/*
*	Render Pipeline
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRef.h"
#include "MRenderQueue.h"

namespace Rad {

	class M_ENTRY RenderPipeline
	{
		DECLARE_REF();
		DECLARE_RTTI_ROOT(RenderPipeline);

	public:
		struct SolidSorter {

			int operator()(RenderObject * obj1, RenderObject * obj2)
			{
				ShaderFX * fx1 = obj1->GetCurrentShaderFX();
				ShaderFX * fx2 = obj2->GetCurrentShaderFX();

				if (fx1 != fx2)
					return fx1 - fx2;

				Texture * tex1 = obj1->GetMaterial()->maps[eMapType::DIFFUSE].c_ptr();
				Texture * tex2 = obj2->GetMaterial()->maps[eMapType::DIFFUSE].c_ptr();

				return tex1 - tex2;
			}
		};

		struct AlphaSorter {

			Float3 mCameraPosition;

			AlphaSorter(Camera * cam)
			{ 
				mCameraPosition = cam->GetWorldPosition();
			}

			int operator()(RenderObject * obj1, RenderObject * obj2)
			{
				if (!obj1->IsSortSpecial() && !obj2->IsSortSpecial())
				{
					Float3 p0, p1;

					obj1->_getWorldPosition(p0);
					obj2->_getWorldPosition(p1);

					float lenSq0 = mCameraPosition.DistanceSq(p0);
					float lenSq1 = mCameraPosition.DistanceSq(p1);

					return lenSq0 < lenSq1 ? -1 : (lenSq0 > lenSq1 ? 1 : 0);
				}
				else if (obj1->IsSortSpecial())
				{
					return obj1->SortSpecial(obj2, mCameraPosition);
				}
				else
				{
					return !obj2->SortSpecial(obj1, mCameraPosition);
				}
			}
		};

	public:
		RenderPipeline();
		virtual ~RenderPipeline();

		void
			Release() { delete this; };

		virtual void 
			DoRender() = 0;

		RenderQueue *
			GetRenderQueue() { return &mRenderQueue; }

	protected:
		RenderQueue mRenderQueue;
	};

	typedef SmartPtr<RenderPipeline> RenderPipelinePtr;

	//
	class M_ENTRY RenderPipelineStandard : public RenderPipeline
	{
		DECLARE_RTTI();

	public:
		RenderPipelineStandard();
		virtual ~RenderPipelineStandard();

		virtual void 
			DoRender();

	protected:
		void 
			DoLighting();
	};

	//
	class M_ENTRY RenderPipelineNodeArray : public RenderPipeline
	{
		DECLARE_RTTI();

	public:
		Array<Node *> NodeArray;

	public:
		RenderPipelineNodeArray(bool childable);
		virtual ~RenderPipelineNodeArray();

		virtual void 
			DoRender();

	protected:
		bool mChildable;
	};

}