#include "MPKCommon.h"
#include "MMath.h"
#include "fastlz.c"

namespace Rad {

	DataStreamPtr MPK_ReadFile(const String & filename)
	{
		if (FileHelper::Exist(filename))
		{
			return new FileStream(filename);
		}

		return NULL;
	}

	FixedString8 MPK_Filename(int id)
	{
		const int MAX_FILE_COUNT = 16 * 16 * 16 * 16;

		d_assert (id < MAX_FILE_COUNT);

		static char IdStr[] = {
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			'A', 'B', 'C', 'D', 'E', 'F'
		};

		FixedString8 uName = "MPK0000";

		int i = 0;
		while (i < 4)
		{
			int index = id & 0x0F;

			d_assert (index < 16);

			id = id >> 4;

			uName[6 - i] = IdStr[index];
			i++;
		}

		return uName;
	}

	void MPK_Encrypt(byte * data, int len, const byte * pwd)
	{
		byte temp[8];
		memcpy(temp, pwd, 8);

		for (int i = 0, j = 0; i < len; ++i)
		{
			data[i] ^= temp[j];
			temp[j] += 13;

			j = (j + 1) & 7;
		}
	}

	int MPK_Compress(byte * data, int len)
	{
		int elen = len > 16 ? len / 8 : 2;
		static_memory buff(M_STATIC_MEMORY_I_CHANEL, len + elen);

		int encodes = fastlz_compress(data, len, buff.data());
		if (encodes >= 0 && encodes < len)
		{
			memcpy(data, buff.data(), encodes);

			return encodes;
		}

		return -1;
	}

	int MPK_Decompress(byte * uncomp, int unc_len, const byte * comp, int c_len)
	{
		return fastlz_decompress((const char *)comp, c_len, uncomp, unc_len);
	}

	int MPK_Append(MPKFileInfo & fi, const String & existFilename, const String & newFilename, const byte * pwd)
	{
		FILE * fpExist = NULL;
		FILE * fpNew = NULL;

		fpExist = fopen(existFilename.c_str(), "rb");
		fpNew = fopen(newFilename.c_str(), "ab");

		fi.size = 0;
		fi.unc_size = 0;

		if (fpExist)
		{
			if (fpNew == NULL)
			{
				FileHelper::MakeDir(FileHelper::GetFileDir(newFilename));

				fpNew = fopen(newFilename.c_str(), "wb");
			}

			fi.size = ftell_size(fpExist);
			if (fi.size > 0)
			{
				byte * data = new byte[fi.size];
				fread(data, fi.size, 1, fpExist);

				if (fi.flag & MPK_FLAG_COMPRESSED)
				{
					int encodes = MPK_Compress(data, fi.size);
					if (encodes != -1)
					{
						fi.unc_size = fi.size;
						fi.size = encodes;
					}
					else
					{
						fi.flag &= ~(MPK_FLAG_COMPRESSED);
					}
				}

				MPK_Encrypt(data, fi.size, pwd);
				fwrite(data, fi.size, 1, fpNew);

				delete[] data;
			}
		}

		if (fpExist)
			fclose(fpExist);

		if (fpNew)
			fclose(fpNew);

		return fi.size;
	}
}