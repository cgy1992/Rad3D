#include "MPrefab.h"
#include "MRoot.h"
#include "MMesh.h"
#include "MLight.h"
#include "MResourceManager.h"

namespace Rad {

#define PREFAB_MAGIC (int)MAKE_DWORD('P', 'F', 'B', '0')

	//
	ImplementRTTI(Prefab, Node);

	DF_PROPERTY_BEGIN(Prefab)
		DF_PROPERTY_EX(Prefab, mFilename, "Prefab", "Filename", "PT_Filename prefab(*.prefab)|*.prefab|", PT_String)
		DF_PROPERTY(Prefab, mInheritLighting, "Prefab", "InheritLighting", PT_Bool)
	DF_PROPERTY_END()

	Prefab::Prefab()
	{
		mInheritLighting = true;
	}

	Prefab::~Prefab()
	{
		OnPrefabUnload();
	}

	IObject * Prefab::Clone()
	{
		Prefab * p = new Prefab;

		p->Copy(this);

		return p;
	}

	void Prefab::OnPropertyChanged(const Property * p)
	{
		if (p->name == "mFilename")
		{
			SetFilename(mFilename);
		}
	}

	void Prefab::Serialize(Serializer & sl)
	{
		Node::Serialize(sl);

		if (!mInheritLighting)
		{
			if (sl.IsOut())
			{
				OSerializer & OS = (OSerializer &)sl;

				OSerializerTester tester;
				for (int i = 0; i < mNodes.Size(); ++i)
				{
					mNodes[i]->Serialize(tester);
				}

				OS << mPrefabVersion;
				OS << tester.Size();

				for (int i = 0; i < mNodes.Size(); ++i)
				{
					mNodes[i]->Serialize(OS);
				}
			}
			else
			{
				ISerializer & IS = (ISerializer &)sl;

				int verison = 0;
				int length = 0;

				IS >> verison;
				IS >> length;

				if (mPrefabVersion == verison)
				{
					for (int i = 0; i < mNodes.Size(); ++i)
					{
						mNodes[i]->Serialize(IS);
					}
				}
				else
				{
					IS.Skip(length);
				}
			}
		}
	}

	void Prefab::SetFilename(const String & filename)
	{
		mFilename = filename;

		PrefabSourcePtr source = NULL;
		if (mFilename != "")
		{
			source = PrefabManager::Instance()->LoadPrefab(filename);
		}

		SetSource(source);
	}

	void Prefab::SetSource(PrefabSourcePtr prefab)
	{
		if (mSource != prefab)
		{
			OnPrefabUnload();

			mSource = prefab;

			OnPrefabLoaded();
		}
	}

	void Prefab::ResetLighting()
	{
		if (!Root::InEditor())
			return ;

		for (int i = 0; i < mNodes.Size(); ++i)
		{
			Node * node = mNodes[i];

			if (TYPE_OF(Mesh, node))
			{
				Mesh * pMesh = (Mesh *)node;

				pMesh->ResetLighting();

			}
			else if (TYPE_OF(Light, node))
			{
				Light * pLight = (Light *)node;
				if (pLight->IsStatic())
				{
					pLight->SetPreview(true);
				}
			}
			else if (TYPE_OF(Prefab, node))
			{
				Prefab * pPrefab = (Prefab *)node;
		
				pPrefab->ResetLighting();
			}
		}
	}

	void Prefab::SavePrefab(const Array<Node *> & nodes, const String & filename)
	{
		int objCount = 0;
		Float3 center = Float3(0, 0, 0);
		for (int i = 0; i < nodes.Size(); ++i)
		{
			if (nodes[i]->Serializable())
			{
				center += nodes[i]->GetPosition();
				++objCount;
			}
		}

		if (objCount > 0)
		{
			int verison = 0;
			DataStreamPtr stream = ResourceManager::Instance()->OpenFile(filename);
			if (stream != NULL)
			{
				stream->Read(&verison, sizeof(int)); // magic
				stream->Read(&verison, sizeof(int)); // verison

				// increase verison
				verison += 1;
			}

			FILE * fp = fopen(filename.c_str(), "wb");
			if (!fp)
				return ;

			OSerializerF OS(fp, true);

			OS << PREFAB_MAGIC;
			OS << verison;

			OS << objCount;

			center /= (float)objCount;
			for (int i = 0; i < nodes.Size(); ++i)
			{
				if (nodes[i]->Serializable())
				{
					int typeId = nodes[i]->GetRTTI()->TypeId();
					OS << typeId;

					Float3 position = nodes[i]->GetPosition();

					nodes[i]->Translate(-center);

					nodes[i]->Serialize(OS);

					nodes[i]->SetPosition(position);
				}
			}
		}
	}

	void Prefab::OnPrefabLoaded()
	{
		ISerializerD IS(mSource->GetDataStream());

		int magic;
		IS >> magic;
		IS >> mPrefabVersion;

		d_assert (magic == PREFAB_MAGIC);

		int objCount = 0;
		IS >> objCount;
		for (int i = 0; i < objCount; ++ i)
		{
			int typeId = 0;
			IS >> typeId;

			Node * obj = Root::NEW_OBJECT_T<Node>(typeId);
			d_assert (obj != NULL);

			obj->Serialize(IS);

			if (Root::InEditor())
			{
				obj->ClearFlags(EDITOR_NODE_FLAG);
			}

			Attach(obj);

			mNodes.PushBack(obj);
		}

		Aabb localAABB = Aabb::Zero;
		for (int i = 0; i < mNodes.Size(); ++i)
		{
			Node * n = mNodes[i];

			Mat4 from;
			from.MakeTransform(n->GetPosition(), n->GetRotation(), n->GetScale());

			Aabb bound = n->GetLocalAabb();
			bound.Transform(from);

			localAABB.Merge(bound);
		}

		SetLocalAabb(localAABB);
	}

	void Prefab::OnPrefabUnload()
	{
		for (int i = 0; i < mNodes.Size(); ++i)
		{
			delete mNodes[i];
		}

		mNodes.Clear();
	}

}