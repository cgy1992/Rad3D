/*
*	ConfigFile
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRml.h"

namespace Rad {

	class M_ENTRY ConfigFile
	{
	public:
		ConfigFile();
		~ConfigFile();

		bool 
			LoadStream(DataStreamPtr stream);
		bool 
			Load(const char * filename);
		void 
			Save(const char * filename);
		void 
			Clear();

		void 
			SetVariable(const char * name, const char * value);
		const char * 
			GetVariable(const char * name);

		bool 
			GetBool(const char * name, bool _default);
		int 
			GetInt(const char * name, int _default);
		float 
			GetFloat(const char * name, float _default);
		const char * 
			GetString(const char * name, const char * _default);

		rml_node * 
			first_node(const char * name) { return mDocument.first_node(name); }
		rml_node *
			first_node() { return mDocument.first_node(); }

	protected:
		rml_doc mDocument;
	};

}

