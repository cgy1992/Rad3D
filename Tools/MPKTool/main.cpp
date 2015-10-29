#pragma once

#include "mpk.h"
#include "mpk2.h"

Set<FixedString32> gUncompressed;
Array<MPKFileInfo> gFileInfo; 
Array<FixedString256> gFilenames;
FixedString256 gOutputPath;

dword getsize(const char * text)
{
	FixedString32 str = text;
	if (str.Length() > 0)
	{
		char ch = str[str.Length() - 1];
		str[str.Length() - 1] = 0;

		switch (ch)
		{
		case 'K':
		case 'k':
			return (dword)str.ToInt() * 1024;
			break;

		case 'M':
		case 'm':
			return (dword)str.ToInt() * 1024 * 1024;
			break;

		case 'G':
		case 'g':
			return (dword)str.ToInt() * 1024 * 1024 * 1024;
		}
	}

	return 0;
}

int main()
{
	Root * gRoot = new Root();

	rml_doc * xdoc = new rml_doc;
	if (xdoc->open_file("MPKTool.ini"))
	{
		rml_node * nd_outputPath = xdoc->first_node("OutputPath");

		if (nd_outputPath)
		{
			gOutputPath = nd_outputPath->value();
		}

		if (gOutputPath == "")
		{
			std::cout << "OutputPath error" << std::endl;
			goto __end;
		}

		rml_node * nd_uncomp = xdoc->first_node("Uncompress");
		while (nd_uncomp != NULL)
		{
			FixedString32 ext = nd_uncomp->value();
			ext.ToLower();

			gUncompressed.Insert(ext);

			nd_uncomp = nd_uncomp->next_sibling("Uncompress");
		}

		rml_node * nd_mpk = xdoc->first_node("MPK");
		while (nd_mpk != NULL)
		{
			FixedString256 filename = nd_mpk->value();

			pk_begin();

			rml_node * node = nd_mpk->first_node("Size");
			if (node != NULL)
			{
				PK_MAXSIZE = getsize(node->value());
				d_assert (PK_MAXSIZE > 0);
			}

			node = nd_mpk->first_node("Input");
			while (node != NULL)
			{
				pk_convert(node->value(), filename, "");

				node = node->next_sibling("Input");
			}

			pk_save(filename);

			nd_mpk = nd_mpk->next_sibling("MPK");
		}

		nd_mpk = xdoc->first_node("MPK2");
		while (nd_mpk != NULL)
		{
			FixedString256 filename = nd_mpk->value();

			pk2_begin();

			rml_node * node = nd_mpk->first_node("Size");
			if (node != NULL)
			{
				PK2_MAXSIZE = getsize(node->value());
				d_assert (PK2_MAXSIZE > 0);
			}

			node = nd_mpk->first_node("Input");
			while (node != NULL)
			{
				pk2_prepare(node->value(), filename, "");

				node = node->next_sibling("Input");
			}

			pk2_create(filename);

			node = nd_mpk->first_node("Input");
			while (node != NULL)
			{
				pk2_convert(node->value(), filename, "");

				node = node->next_sibling("Input");
			}

			pk2_save(filename);

			nd_mpk = nd_mpk->next_sibling("MPK2");
		}

		goto __end;
	}
	else
	{
		std::cout << "MPKTool.ini open failed" << std::endl;
	}

__end:
	delete xdoc;

	delete gRoot;

	std::cin.get();

	return 0;
}