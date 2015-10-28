/*
*	Prefab
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MNode.h"
#include "MPrefabManager.h"

namespace Rad {

	class M_ENTRY Prefab : public Node
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(Node);

	public:
		static void 
			SavePrefab(const Array<Node *> & nodes, const String & filename);

	public:
		String mFilename;
		bool mInheritLighting;

	public:
		Prefab();
		virtual ~Prefab();

		virtual IObject *
			Clone();
		virtual void
			OnPropertyChanged(const Property * p);
		virtual void
			Serialize(Serializer & sl);

		void 
			SetFilename(const String & filename);
		const String &
			GetFilename() { return mFilename; }

		void
			SetInheritLighting(bool b);
		bool
			IsInheritLighting() { return mInheritLighting; }

		void
			ResetLighting();

		void 
			SetSource(PrefabSourcePtr prefab);
		PrefabSourcePtr 
			GetSource() { return mSource; }

		Node *
			GetNode(int i) { return mNodes[i]; }
		int
			GetNodeCount() { return mNodes.Size(); }

	protected:
		void
			OnPrefabLoaded();
		void
			OnPrefabUnload();

	protected:
		PrefabSourcePtr mSource;
		int mPrefabVersion;
		Array<Node *> mNodes;
	};

}