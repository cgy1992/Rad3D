/*
*	Mesh
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MNode.h"
#include "MRenderObject.h"
#include "MMeshSource.h"
#include "MSkeleton.h"
#include "MAnimationController.h"

namespace Rad {

	class Mesh;

	class M_ENTRY SubMesh : public RenderObject
	{
	public:
		SubMesh(Mesh * parent, MeshBuffer * sm);
		virtual ~SubMesh();

		Mesh * 
			GetParent() { return mParent; }
		MeshBuffer *
			GetMeshBuffer() { return mMeshBuffer; }

		void
			SetMeshShader(MeshShaderPtr shader);
		MeshShaderPtr
			GetMeshShader() { return mMeshShader; }

		VertexBufferPtr
			QueryVertexAnimationBuffer();

		virtual void 
			_getWorldPosition(Float3 & pos);
		virtual void 
			_getWorldBound(Aabb & bound);
		virtual void 
			_getWorldTM(Mat4 & form);
		virtual int 
			_getBoneTM(Mat4 * boneTM);
		virtual Node *
			_getNode();

	protected:
		Mesh * mParent;
		MeshBuffer * mMeshBuffer;
		MeshShaderPtr mMeshShader;
		VertexBufferPtr mVertexAnimtionBuffer;
	};

	//
	class M_ENTRY Mesh : public Node
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(Node);

	protected:
		String mFilename;
		bool mGrouped;
		int mSLMode;
		bool mSLCastShadow;
		bool mSLReceiveShadow;
		int mSLMapWidth;
		int mSLMapHeight;
		String mSLMapFile;

	public:
		Mesh();
		virtual ~Mesh();

		virtual void 
			OnPropertyChanged(const Property * p);

		virtual IObject *
			Clone();

		void 
			SetFilename(const String & filename);
		const String &
			GetFilename() { return mFilename; }

		void
			SetGrouped(bool grouped);
		bool
			GetGrouped() const { return mGrouped; }

		void
			SetSLMode(int mode);
		int
			GetSLMode() { return mSLMode; }

		void
			SetSLCastShadow(bool cast);
		bool
			GetSLCastShadow() { return mSLCastShadow; }

		void
			SetSLReceiveShadow(bool receive);
		bool
			GetSLReceiveShadow() { return mSLReceiveShadow; }

		void
			SetSLMapSize(int w, int h);
		int
			GetSLMapWidth() { return mSLMapWidth; }
		int
			GetSLMapHeight() { return mSLMapHeight; }

		void
			SetSLMapFile(const String & filename);
		const String &
			GetSLMapFile() { return mSLMapFile; }

		void 
			SetSource(MeshSourcePtr mesh);
		MeshSourcePtr       
			GetSource() { return mSource; }

		void				
			SetShaderFX(ShaderFX * fx);

		virtual void
			Update(float elapsedTime);
		virtual void        
			AddRenderQueue(RenderQueue * rq);

		int					
			GetVertexCount();
		int					
			GetPrimCount();
		int                 
			GetSubMeshCount();
		SubMesh *			
			GetSubMesh(int index);
		SubMesh *			
			NewSubMesh();
		void				
			DeleteSubMesh(int index);

		void				
			SetSkeleton(SkeletonPtr skel, bool shared);
		SkeletonPtr 
			GetSkeleton();
		bool				
			IsSkeletonShared();

		void				
			LoadAnimation(const FixedString32 & name, const String & source);
		AnimationController *
			PlayAnimation(const FixedString32 & name, const BlendInfo & bi = BlendInfo::Default, int order = 0);
		void
			AddAnimationController(AnimationController * controller);
		void
			RemoveAnimationController(AnimationController * controller);
		AnimationController *
			GetAnimationController(const FixedString32 & name);
		AnimationController *
			GetAnimationController(int i);
		int
			GetAnimationControllerCount();
		bool
			IsAnimationPlaying(const FixedString32 & name);
		void
			StopAnimation();
		bool
			IsAnimationStoped();
		void
			PauseAnimation(bool pause);
		bool
			IsAnimationPaused();

		void				
			SetLightingColor(const Rgba32 * colorBuffer, int count);
		void				
			GetLightingColor(Array<Rgba32> & colorBuffer);
		bool				
			HasLightingColor();
		void				
			SetLightingMap(TexturePtr texture);
		TexturePtr			
			GetLightingMap();
		void
			ResetLighting();
		bool				
			HasLightingMapUV();

		virtual void 
			Load(float priority);
		virtual void 
			Unload();

	protected:
		void 
			OnMeshLoaded();
		void 
			OnMeshUnload();

		cListener0<Mesh> L_MeshLoaded;
		cListener0<Mesh> L_MeshUnload;

	protected:
		MeshSourcePtr mSource;

		Array<SubMesh *> mMeshes;

		SkeletonPtr mSkeleton;
		bool mSkeletonShared;

		Array<AnimationController*> mAnimationControllers;
		bool mAnimtionPaused;

		Array<Rgba32> mLightingColor;
	};

}
