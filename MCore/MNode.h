/*
*	Node
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MTypes.h"
#include "MEvent.h"
#include "MShape.h"
#include "MUserData.h"
#include "MComponent.h"

#define EDITOR_NODE_FLAG 0x00000001
#define EDITOR_NODE_PICK 0x00000001

namespace Rad {

	struct eTransform
	{
		enum enum_t {
			LOCAL,
			PARENT,
			WORLD,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eTransform);
	};

	struct eTmFlags
	{
		enum enum_t {
			NONE		= 0x0000,
			TRANSLATE	= 0x0001,
			ROTATE		= 0x0002,
			SCALE		= 0x0004,

			ALL			= 0xFFFF,

			UNKNOWN		= 0x7F000000
		};

		M_ENUMERATION(eTmFlags);
	};


	class RenderQueue;
	class Zone;
	class WorldSection;

	class M_ENTRY Node : public ComponentOwner
	{
		DECLARE_RTTI();
		DECLARE_LINKER(Node);
		DECLARE_PROPERTY(ComponentOwner);
		DECLARE_USERDATA(Node, 8);

		class ZoneLinker
		{
			DECLARE_LINKER(ZoneLinker);

		public:
			ZoneLinker(Node * n) : mNode(n) {}

			Node * 
				GetNode() { return mNode; }
			bool 
				IsEmpty() { return __link_node.prev == NULL && __link_node.next == NULL; }

		protected:
			Node * mNode;
		};

	public:
		tEvent1<Node *> E_Loaded;
		tEvent1<Node *> E_Unload;

	protected:
		int mUID;
		FixedString32 mName;
		Float3 mPosition;
		Quat mRotation;
		Float3 mScale;
		float mOpacity;
		bool mLighting;
		bool mCastShadow;
		bool mReceiveShadow;
		bool mVisible;

	public:
		Node(const FixedString32 & name = "Node", bool _inword = true);
		virtual ~Node();

		virtual IObject *
			Clone();

		void					
			Attach(Node * child);
		void					
			Detach(Node * child);
		virtual void 
			_notifyAttach(Node * parent);
		virtual void 
			_notifyDetach();
		int
			GetChildCount();
		Node *
			GetChild(int index);
		Node *					
			GetParent() { return mParent; }

		void
			_notifyAttachZone(Zone * zone);
		void
			_notifyDetachZone();
		Zone *				
			GetZone() { return mZone; }
		ZoneLinker *		
			GetZoneLinker() { return &mZoneLinker; }
		
		void
			_notifyAttachSection(WorldSection * section);
		void
			_notifyDetachSection();
		WorldSection *
			GetSection() { return mSection; }

		void
			SetUID(int uid);
		int
			GetUID() const { return mUID; }
		void
			SetName(const FixedString32 & name);
		const FixedString32 &
			GetName() const { return mName; }

		void                    
			SetPosition(const Float3 & rk);
		void
			SetPosition(float x, float y, float z);
		const Float3 &
			GetPosition() const { return mPosition; }

		void                    
			SetRotation(const Quat & rk);
		void
			SetRotation(float x, float y, float z, float w);
		const Quat &            
			GetRotation() const { return mRotation; }
		
		void                    
			SetScale(const Float3 & rk);
		void
			SetScale(float x, float y, float z);
		const Float3 &
			GetScale() const { return mScale; }
		
		void 
			SetRotationEx(const Float3 & angles);
		void					
			SetDirection(const Float3 & rk);

		void
			SetOpacity(float opacity);
		float
			GetOpacity() { return mOpacity; }

		void
			SetLighting(bool b);
		bool
			IsLighting() { return mLighting; }

		void
			SetCastShadow(bool b);
		bool
			IsCastShadow() const { return mCastShadow; }

		void
			SetReceiveShadow(bool b);
		bool
			IsReceiveShadow() const { return mReceiveShadow;}

		void                    
			SetVisible(bool visible);
		bool                    
			IsVisible() const;

		void
			SetRenderContextId(int id) { mRenderContexId = id; }
		int
			GetRenderContextId() { return mRenderContexId; }

		void					
			SetFlags(int flags) { mFlags |= flags; }
		void					
			ClearFlags(int flags) { mFlags &= ~flags; }
		int						
			GetFlags() const { return mFlags; }
		void
			SetTmFlags(int type);
		int
			GetTmFlags() const { return mTmFlags; }
		int
			GetTmChangeFlags() const { return mTmChangeFlags; }

		void
			ChangeTm(int flags);

		void                    
			Translate(const Float3 & v, eTransform ts = eTransform::PARENT);
		void                    
			Rotate(const Quat & q, eTransform ts = eTransform::PARENT);
		void                    
			Scale(const Float3 & v);

		void                    
			Pitch(float x, eTransform ts = eTransform::PARENT);
		void                    
			Yaw(float x, eTransform ts = eTransform::PARENT);
		void                    
			Roll(float x, eTransform ts = eTransform::PARENT);

		const Float3 &          
			GetWorldPosition();
		const Quat &            
			GetWorldRotation();
		const Float3 &          
			GetWorldScale();
		const Mat4 &            
			GetWorldTM();
		float
			GetWorldOpacity();

		void					
			SetLocalAabb(const Aabb & aabb);
		const Aabb &			
			GetLocalAabb();
		void
			SetWorldAabb(const Aabb & aabb);
		const Aabb &            
			GetWorldAabb();
		Aabb
			GetBound(int depth = 0);

		void
			SetInheritRotation(bool b);
		bool
			IsInheritRotation() const { return mInheritRotation; }

		void
			SetInheritScale(bool b);
		bool
			IsInheritScale() const { return mInheritScale; }

		virtual void
			_updateTM();
		virtual void
			Update(float elapsedTime);
		virtual void 
			AddRenderQueue(RenderQueue * rq) {}

		virtual void 
			Load(float priority) {}
		virtual void 
			Unload() {}

		void
			SetShape(Shape * shape);
		Shape *
			GetShape() { return mShape; }

	protected:
		Node * mParent;
		Array<Node *> mChildren; 

		Zone * mZone;
		ZoneLinker mZoneLinker;
		WorldSection * mSection;

		int mRenderContexId;

		int mFlags;
		int mTmFlags;
		int mTmChangeFlags;

		Float3 mWorldPosition;
		Quat mWorldRotation;
		Float3 mWorldScale;
		Mat4 mWorldTM;
		float mWorldOpacity;

		Aabb mLocalAABB;
		Aabb mWorldAABB;
		bool mLocalAABBValid;

		bool mInheritRotation;
		bool mInheritScale;
		bool mInWorld;

		Shape * mShape;
	};

}
