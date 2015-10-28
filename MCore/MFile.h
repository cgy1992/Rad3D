/*
*	File
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MString.h"
#include "MFixedString.h"

#include <sys/stat.h>
#ifndef M_PLATFORM_WIN32
#include <dirent.h>
#endif

namespace Rad {

	struct FILE_HANDLE
	{
		FILE * handle;

		FILE_HANDLE() { handle = NULL; }
		~FILE_HANDLE() { if (handle) fclose(handle); }
	};

	M_ENTRY int 
		fread_string(char * str, int size, FILE * fp);
	M_ENTRY int 
		fread_line(char * buff, int size, FILE * fp);
	M_ENTRY int
		ftell_size(FILE * fp);

	//
	class M_ENTRY FileHelper
	{
	public:
		static String 
			GetExternName(const String & file);
		static String 
			GetFileDir(const String & file);
		static int
			GetFileSize(const String & file);
		static String 
			GetBaseName(const String & file);
		static String 
			RemoveExternName(const String & file);
		static String 
			ReplaceExternName(const String & filename, const String & ext);
		static String
			Transliterate(const String & path, const String & file);

		static bool 
			Exist(const String & file);

		static bool
			Move(const String & existFilename, const String & newFilename);
		static int 
			Copy(const String & existFilename, const String & newFilename);
		static int
			Append(const String & existFilename, const String & newFilename);

		static void
			MakeDir(const String & dir);
		static bool 
			NewDir(const String & dir);
		static bool 
			DelDir(const String & dir);
		static bool 
			DelFile(const String & filename);
	};

	//
	class M_ENTRY FileSystem
	{
	public:
		FileSystem();
		~FileSystem();

		bool 
			Open(const String & dir);
		void 
			Close();

		const String & 
			GetOpenDir();

		bool 
			NextFile();
		const char * 
			FileName();
		bool 
			FileIsDir();

	protected:
		String mFileDir;

#ifdef M_PLATFORM_WIN32
		WIN32_FIND_DATA fd;
		HANDLE hFindHandle;
#else
		DIR * dp;
		dirent * dm;
#endif
	};
}
