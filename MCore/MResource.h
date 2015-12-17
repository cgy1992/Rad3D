/*
*	Resource
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MArray.h"
#include "MEvent.h"
#include "MLinker.h"
#include "MDataStream.h"
#include "MFixedString.h"

namespace Rad
{
	class ResourceDependence;

	class M_ENTRY Resource
	{
		DECLARE_REF_MT();
		DECLARE_ALLOC();

	public:
		enum eLoadState {

			LOADED,
			LOADING,
			UNLOADED
		};

		tEvent0 E_Loaded;
		tEvent0 E_Unload;

	public:
		Resource(const String & name, const String & sourceName);
		virtual ~Resource();

		virtual void
			Release() { delete this; }

		eLoadState 
			GetLoadState() { return mLoadState; }

		const String & 
			GetName() const { return mName; }
		const String & 
			GetSourceName() const { return mSourceName; }

		void 
			SetPriority(float priority) { mPriority = priority; }
		float 
			GetPriority() const { return mPriority; }

		void 
			Load();
		void 
			Unload();
		void 
			Reload();
		void 
			EnsureLoad();
		bool 
			CanLoad() { return mSourceName != ""; }

		virtual void 
			OnLoad();
		virtual void 
			OnUnload();
		virtual void 
			_load(DataStreamPtr stream) = 0;

		void 
			_addDependence(Resource * pSubResource);
		void 
			_removeDependence(ResourceDependence * pDependence);
		void 
			_removeAllDependence();
		ResourceDependence * 
			_getDependences();

	protected:
		String mName;
		String mSourceName;
		float mPriority;

		eLoadState mLoadState;

		ResourceDependence * mDependenceLinker;
	};

	typedef SmartPtr<Resource> ResourcePtr;

	//
	class M_ENTRY ResourceDependence
	{
		DECLARE_POOL_ALLOC();
		DECLARE_LINKER(ResourceDependence);

	public:
		ResourceDependence(Resource * pResource, Resource * pSubResource)
			: mResource(pResource)
			, mSubResource(pSubResource)
		{
			mSubResource->E_Loaded += mLoadListener(this, &ResourceDependence::OnLoaded);
		}

		~ResourceDependence()
		{
			mSubResource->E_Loaded -= mLoadListener(this, &ResourceDependence::OnLoaded);
		}

		Resource * 
			getResource() {  return mResource; }
		Resource * 
			getSubResource() { return mSubResource; }

		void 
			OnLoaded() { mResource->_removeDependence(this); }

	protected:
		Resource * mResource;
		Resource * mSubResource;

		cListener0<ResourceDependence> mLoadListener;
	};

	//
	class M_ENTRY ResourceDataStream : public Resource
	{
	public:
		tEvent1<DataStreamPtr> E_LoadImp;

	public:
		ResourceDataStream(const String & name, const String & sourceName);
		virtual ~ResourceDataStream();

		virtual void 
			_load(DataStreamPtr stream);

		DataStreamPtr
			GetDataStream() { return mDataStream; }

	protected:
		DataStreamPtr mDataStream;
	};

	typedef SmartPtr<ResourceDataStream> ResourceDataStreamPtr;

}