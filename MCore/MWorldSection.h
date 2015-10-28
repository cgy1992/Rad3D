/*
*	World Section
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MNode.h"
#include "MZone.h"
#include "MResource.h"

namespace Rad {

	class ISectionData;

	class M_ENTRY WorldSection
	{
	public:
		WorldSection();
		virtual ~WorldSection();

		void
			Init(const String & filename, const Int2 & index, const rml_node * defnode);

		const String &
			GetFilename() { return mFilename; }
		const Int2 &
			GetIndex() { return mIndex; }
		const rml_node *
			GetDefNode() { return mDefNode; }
		const Aabb &
			GetBound() { return mBound; }
		Zone *
			GetZone() { return mZone; }
		bool
			IsValid() { return mDefNode != NULL; }

		void
			AddNode(Node * node);
		void
			RemoveNode(Node * node, bool _delete);
		const Array<Node *> &
			GetNodes() { return mNodes; }

		ISectionData *
			GetData(const char * className);
		template <class T> T *
			GetDataT();
		const Array<ISectionData *>
			GetDatas() { return mDatas; }
		
		void
			Update(float elapsedTime);

		void
			Load();
		void
			Unload();
		void
			Serialize(Serializer & sl);

	protected:
		void
			_OnResourceLoaded();
		void 
			_OnResourceUnload();

		cListener0<WorldSection> L_ResourceLoaded;
		cListener0<WorldSection> L_ResourceUnload;

	protected:
		String mFilename;
		const rml_node * mDefNode;
		Int2 mIndex;
		Aabb mBound;
		Zone * mZone;
		Array<Node *> mNodes;
		Array<ISectionData *> mDatas;

		ResourceDataStreamPtr mResource;
	};

	template <class T> T * WorldSection::GetDataT()
	{
		ISectionData * data = GetData(T::RTTI.Name());
		if (data)
		{
			d_assert (KIND_OF(T, data));

			return (T *)data;
		}

		return NULL;
	}

	//
	class M_ENTRY ISectionData : public IObject
	{
		DECLARE_RTTI();

	public:
		ISectionData() {}
		virtual ~ISectionData() {}

		virtual bool
			Init(WorldSection * section, const rml_node * def);

		virtual void
			Update(float elapsedTime) = 0;

		WorldSection *
			GetWorldSection() { return mWorldSection; }

	protected:
		WorldSection * mWorldSection;
	};

}