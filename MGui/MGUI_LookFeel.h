/*
*	MGUI LockFeel
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Types.h"

namespace Rad { namespace MGUI {

	class Widget;

	class MGUI_ENTRY LookFeel
	{
		DECLARE_POOL_ALLOC();

	public:
		LookFeel(const FixedString32 & _name = "", const char * _internal = NULL);
		virtual ~LookFeel();

		void 
			SetName(const FixedString32 & name) { mName = name; }
		const FixedString32 & 
			GetName() const { return mName; }

		void 
			SetSkin(const String & filename);
		String
			GetSkin() const { return mSkin; }

		void 
			SetUVRect(int _state, const Rect & _rect);
		const Rect & 
			GetUVRect(int _state) const { return mUVRect[_state]; }

		void
			SetTextColor(int _state, const Float4 & color);
		const Float4 &
			GetTextColor(int _state) const { return mTextColor[_state]; }

		void 
			SetClientRect(const ClientRect & _rect) { mClientRect = _rect; }
		const ClientRect & 
			GetClientRect() const { return mClientRect; }

		Rect 
			GetUVClientRect(int _state) const;

		void 
			AddChild(LookFeel * _lookfeel) { mChildren.PushBack(_lookfeel); }
		void 
			RemoveChild(int index) { delete mChildren[index]; mChildren.Erase(index); }
		int 
			GetChildCount() { return mChildren.Size(); }
		LookFeel * 
			GetChild(int _index) { return mChildren[_index]; }
		LookFeel * 
			GetChild(const FixedString32 & _name);
		const LookFeel * 
			GetChild(const FixedString32 & _name) const;

		LookFeel *
			Clone(const FixedString32 & _name);

		const char * 
			_getInternal() { return mInternal; }

	protected:
		const char * mInternal;
		FixedString32 mName;
		String mSkin;

		Field<Rect, eWidgetState::MAX> mUVRect;
		Field<Float4, eWidgetState::MAX> mTextColor;
		ClientRect mClientRect;

		Array<LookFeel *> mChildren;
	};

	//
	class MGUI_ENTRY LookFeelGroup
	{
	public:
		LookFeelGroup(const FixedString32 & typeName);
		~LookFeelGroup();

		void
			Clear();
		void
			Append(LookFeel * _lookfeel);
		void
			Remove(LookFeel * _lookfeel);
		LookFeel *
			Find(const FixedString32 & name);
		LookFeel *
			Find(int crc32, const FixedString32 & name);
		void
			GetLookFeelMap(Map<FixedString32, LookFeel *> & lookfeels);

		const FixedString32 &
			TypeName() { return mTypeName; }
		int
			Size() { return mLookFeels.Size(); }
		LookFeel *
			At(int i) { return mLookFeels[i].value; }
		void
			_Remove(int i);

	protected:
		FixedString32 mTypeName;
		Map<int, LookFeel *> mLookFeels;
	};

	//
	class MGUI_ENTRY LookFeelManager : public Singleton<LookFeelManager>
	{
	public:
		enum {
			K_Version = 1,
		};

	public:
		LookFeelManager();
		~LookFeelManager();

		void 
			Load(const char * source);
		void 
			LoadFromStream(DataStreamPtr stream, const char * source);
		void 
			Unload(const char * source);
		void 
			UnloadAll();

		bool 
			Save(const String & filename);

		void 
			Append(const FixedString32 & _type, LookFeel * _lookfeel);
		void 
			Remove(const FixedString32 & _type, LookFeel * _lookfeel);
		LookFeel * 
			Find(const FixedString32 & _type, const FixedString32 & _name);

		LookFeelGroup *
			GetGroup(const FixedString32 & _type);
		LookFeelGroup *
			GetGroup(int i);
		int
			GetGroupSize();
		void
			GetGroupMap(Map<FixedString32, LookFeelGroup *> & groups);

		const Array<const char *> & 
			_getFilenames() { return mFilenames; }

		void 
			SetDefaultLookfeel(LookFeel * lookfeel) { mDefaultLookfeel = lookfeel; }
		LookFeel * 
			GetDefaultLookFeel() { return mDefaultLookfeel; }

	protected:
		Map<int, LookFeelGroup *> mGroups;

		Array<const char *> mFilenames;
		LookFeel * mDefaultLookfeel;
	};

}}
