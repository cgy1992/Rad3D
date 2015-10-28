#include "MFile.h"
#include "MDebug.h"

#include <dirent.h>
#ifdef M_PLATFORM_WIN32
#include <io.h>
#else
#include<unistd.h>
#include <sys/stat.h>
#endif

namespace Rad {

	int fread_string(char * str, int size, FILE * fp)
	{
		const int TEMP_SIZE = 128;
		char tmpBuf[TEMP_SIZE] = { 0 };
		int count = 0;
		bool flag = false;

		size -= 1;
		while (!flag && count < size && !feof(fp))
		{
			int read_count = fread(tmpBuf, 1, TEMP_SIZE, fp);

			int i = 0;
			while (i < read_count && count < size)
			{
				if (tmpBuf[i] == '\0')
				{
					flag = true;
					i += 1;
					break;
				}

				str[count++] = tmpBuf[i++];
			}

			if (i < read_count)
			{
				fseek(fp, -(read_count - i), SEEK_CUR);
			}
		}

		str[count] = '\0';

		return count;
	}

	int fread_line(char * buff, int size, FILE * fp)
	{
		const int TEMP_SIZE = 128;
		char tmpBuf[TEMP_SIZE] = { 0 };
		int count = 0;
		bool flag = false;

		size -= 1;
		while (!flag && count < size && !feof(fp))
		{
			int read_count = fread(tmpBuf, 1, TEMP_SIZE, fp);

			int i = 0;
			while (i < read_count && count < size)
			{
				if (tmpBuf[i] == '\n')
				{
					flag = true;
					i += 1;
					break;
				}

				buff[count++] = tmpBuf[i++];
			}

			if (i < read_count)
			{
				fseek(fp, -(read_count - i), SEEK_CUR);
			}
		}

		buff[count] = '\0';

		if (count > 0 && buff[count - 1] == '\r')
			buff[count - 1] = '\0';

		return count;
	}

	int ftell_size(FILE * fp)
	{
		int size = 0;
		int cur = ftell(fp);

		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, cur, SEEK_SET);

		return size;
	}

//
#ifdef M_PLATFORM_WIN32

	FileSystem::FileSystem()
		: hFindHandle(INVALID_HANDLE_VALUE)
	{
	}

	FileSystem::~FileSystem()
	{
		Close();
	}

	bool FileSystem::Open(const String & dir)
	{
		d_assert (hFindHandle == INVALID_HANDLE_VALUE);

		mFileDir = dir;

		String filter = dir + "/*.*";
		hFindHandle = FindFirstFile(filter.c_str(), &fd);

		return hFindHandle != INVALID_HANDLE_VALUE;
	}

	void FileSystem::Close()
	{
		if (hFindHandle != INVALID_HANDLE_VALUE)
			FindClose(hFindHandle);
		
		hFindHandle = INVALID_HANDLE_VALUE;
	}

	bool FileSystem::NextFile()
	{
		if (hFindHandle != INVALID_HANDLE_VALUE)
			return FindNextFile(hFindHandle, &fd) ? true : false;

		return false;
	}

	const char * FileSystem::FileName()
	{
		if (hFindHandle != INVALID_HANDLE_VALUE)
			return fd.cFileName;

		return NULL;
	}

	bool FileSystem::FileIsDir()
	{
		if (hFindHandle != INVALID_HANDLE_VALUE)
			return (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		return false;
	}

#else

	FileSystem::FileSystem()
		: dp(NULL)
		, dm(NULL)
	{
	}

	FileSystem::~FileSystem()
	{
		Close();
	}

	bool FileSystem::Open(const String & dir)
	{
		d_assert (dp == NULL);

		mFileDir = dir;

		dp = opendir(dir.c_str());
		dm = NULL;

		return dp != NULL;
	}

	void FileSystem::Close()
	{
		if (dp)
			closedir(dp);

		mFileDir = "";
	}

	
	bool FileSystem::NextFile()
	{
		if (!dp)
			return false;

		dm = readdir(dp);

		return dm != NULL;
	}

	const char * FileSystem::FileName()
	{
		if (!dm)
			return NULL;

		return dm->d_name;
	}

	bool FileSystem::FileIsDir()
	{
		if (!dm)
			return false;

		return dm->d_type == DT_DIR;
	}

#endif

	const String & FileSystem::GetOpenDir()
	{
		return mFileDir;
	}


	//
	//
	String FileHelper::GetExternName(const String & file)
	{
		int len = file.Length();
		const char * str = file.c_str();

		while (len > 0 && str[len - 1] != '.')
			len -= 1;

		if (len < 1 || str[len - 1] != '.')
			return "";

		return &str[len];
	}

	String FileHelper::GetFileDir(const String & file)
	{
		const char * str = file.c_str();
		int len = file.Length();

		while (len > 0 && (str[len - 1] != '\\' && str[len - 1] != '/'))
			--len;

		FixedString256 dir = file.c_str();

		dir[len] = 0;

		if (len > 0)
			dir[len - 1] = 0;

		return dir.c_str();
	}

	int FileHelper::GetFileSize(const String & file)
	{
		struct stat fileinfo;
		
		if (stat(file.c_str(), &fileinfo) == 0)
			return fileinfo.st_size;

		return 0;
	}

	String FileHelper::GetBaseName(const String & file)
	{
		const char * str = file.c_str();
		int len = file.Length();
		int kLen = len;

		while (len > 0 && (str[len - 1] != '\\' && str[len - 1] != '/'))
			--len;

		char baseName[128];

		int i = 0;
		while (len < kLen)
		{
			baseName[i++] = str[len++];
		}

		baseName[i] = 0;

		return baseName;
	}

	String FileHelper::RemoveExternName(const String & file)
	{
		int len = file.Length();
		const char * str = file.c_str();
		int p = len;

		while (p > 0 && str[p - 1] != '.')
			p -= 1;

		if (p < 1 || str[p - 1] != '.')
			return file;

		char txt[128];

		strcpy_s(txt, 128, file.c_str());

		txt[p - 1] = 0;

		return txt;
	}

	String FileHelper::ReplaceExternName(const String & filename, const String & ext)
	{
		String basename = RemoveExternName(filename);

		return basename + "." + ext;
	}

	String FileHelper::Transliterate(const String & _path, const String & _file)
	{
		FixedString256 filename = _path.c_str();
		filename += "/";
		filename += _file.c_str();

		filename.Replace('\\', '/');

		int i = 0;
		while (i < filename.Length())
		{
			if (filename[i] == '/')
			{
				if (filename[i + 1] == '.'&& filename[i + 2] == '.' && filename[i + 3] == '/')
				{
					int k = 0;
					int j = i + 2;
					while (j >= 0)
					{
						if (filename[j] == '/' && k++ == 1)
						{
							strcpy(&filename[j], &filename[i + 3]);
							i = j;

							break;
						}

						--j;
					}

					if (k == 2)
					{
						continue;
					}
				}
				else if (filename[i + 1] == '.' && filename[i + 2] == '/')
				{
					strcpy(&filename[i], &filename[i + 2]);
					continue;
				}
			}
			
			i += 1;
		}

		return filename.c_str();
	}

	bool FileHelper::Exist(const String & file)
	{
		return access(file.c_str(), 0) != -1;
	}

	bool FileHelper::Move(const String & existFilename, const String & newFilename)
	{
		return rename(existFilename.c_str(), newFilename.c_str()) == 0;
	}

	int FileHelper::Copy(const String & existFilename, const String & newFilename)
	{
		int size = 0;
		FILE * fpExist = NULL;
		FILE * fpNew = NULL;

		fpExist = fopen(existFilename.c_str(), "rb");
		if (fpExist)
		{
			MakeDir(GetFileDir(newFilename));

			fpNew = fopen(newFilename.c_str(), "wb");
			if (fpNew)
			{
				size = ftell_size(fpExist);
				if (size > 0)
				{
					char * data = new char[size];

					fread(data, size, 1, fpExist);
					fwrite(data, size, 1, fpNew);

					delete[] data;
				}
			}
		}

		if (fpExist)
			fclose(fpExist);

		if (fpNew)
			fclose(fpNew);

		return size;
	}

	int FileHelper::Append(const String & existFilename, const String & newFilename)
	{
		int size = 0;
		FILE * fpExist = NULL;
		FILE * fpNew = NULL;

		fpExist = fopen(existFilename.c_str(), "rb");
		fpNew = fopen(newFilename.c_str(), "ab");

		if (fpExist)
		{
			if (fpNew == NULL)
			{
				MakeDir(GetFileDir(newFilename));

				fpNew = fopen(newFilename.c_str(), "wb");
			}

			size = ftell_size(fpExist);
			if (size > 0)
			{
				char * data = new char[size];

				fread(data, size, 1, fpExist);
				fwrite(data, size, 1, fpNew);

				delete[] data;
			}
		}

		if (fpExist)
			fclose(fpExist);

		if (fpNew)
			fclose(fpNew);

		return size;
	}

	void FileHelper::MakeDir(const String & filedir)
	{
		String dir = filedir;

		if (dir == "")
			return ;

		if (!FileHelper::Exist(dir))
		{
			String _dir = FileHelper::GetFileDir(dir);

			MakeDir(_dir.c_str());

			FileHelper::NewDir(dir);
		}
	}

	bool FileHelper::NewDir(const String & dir)
	{
#ifdef M_PLATFORM_WIN32

		SECURITY_ATTRIBUTES attribute;
		attribute.nLength = sizeof(attribute);
		attribute.lpSecurityDescriptor = NULL;
		attribute.bInheritHandle = FALSE;

		return CreateDirectory(dir.c_str(), &attribute) == TRUE;

#else

		return mkdir(dir.c_str(), 777) != -1;

#endif
	}

	bool FileHelper::DelDir(const String & dir)
	{
		const char * dirName = dir.c_str();
		DIR* dp = NULL;
		DIR* dpin = NULL;
		char pathname[256];
		struct dirent* dirp;

		dp = opendir(dirName);
		if (dp == NULL)
			return false;

		while ((dirp = readdir(dp)) != NULL)
		{
			if (strcmp(dirp->d_name, "..") == 0 || strcmp(dirp->d_name, ".") == 0)
				continue;

			strcpy(pathname, dirName);
			strcat(pathname, "/");
			strcat(pathname, dirp->d_name);

			dpin = opendir(pathname);
			if (dpin != NULL)
			{
				closedir(dpin);
				dpin = NULL;
				DelDir(pathname);
			}
			else
			{
				remove(pathname);
			}
		}

#ifdef M_PLATFORM_WIN32
		RemoveDirectory(dirName);
#else
		rmdir(dirName);
#endif

		closedir(dp);
		dirp = NULL;

		return TRUE;
	}

	bool FileHelper::DelFile(const String & filename)
	{
		return remove(filename.c_str()) == 0;
	}

}
