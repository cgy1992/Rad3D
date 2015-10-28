/*
*	VertexDeclaration
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MTypes.h"

namespace Rad {

	struct M_ENTRY VertexElement
	{
		eVertexSemantic Semantic;
		eVertexType Type;

		VertexElement() {}
		VertexElement(eVertexSemantic vs, eVertexType vt) : Semantic(vs), Type(vt) {}

		static int GetTypeSize(eVertexType type)
		{
#if 1
			d_assert(type <= eVertexType::FLOAT4);

			static const int S_SIZE[] = {
				0, 
				1, 2, 3, 4,
				1, 2, 3, 4,
				2, 4, 6, 8,
				2, 4, 6, 8,
				4, 8, 12, 16
			};

			return S_SIZE[type];

#else

			switch (type)
			{
			case eVertexType::BYTE1:
			case eVertexType::UBYTE1:
				return 1;

			case eVertexType::BYTE2:
			case eVertexType::UBYTE2:
				return 2;

			case eVertexType::BYTE3:
			case eVertexType::UBYTE3:
				return 3;

			case eVertexType::BYTE4:
			case eVertexType::UBYTE4:
				return 4;

			case eVertexType::SHORT1:
			case eVertexType::USHORT1:
				return 2;

			case eVertexType::SHORT2:
			case eVertexType::USHORT2:
				return 4;

			case eVertexType::SHORT3:
			case eVertexType::USHORT3:
				return 6;

			case eVertexType::SHORT4:
			case eVertexType::USHORT4:
				return 8;

			case eVertexType::FLOAT1:
				return 4;

			case eVertexType::FLOAT2:
				return 8;

			case eVertexType::FLOAT3:
				return 12;

			case eVertexType::FLOAT4:
				return 16;
			}

			d_assert (0);
			return 0;
#endif
		}
	};

	class M_ENTRY VertexDeclaration
	{
	public:
		VertexDeclaration();
		~VertexDeclaration();

		void 
			Clear();

		int 
			GetVertexSize() const;

		void 
			AddElement(eVertexSemantic semantic, eVertexType type);
		int 
			GetElementCount() const;
		const VertexElement & 
			GetElement(int index) const;
		bool 
			HasElement(eVertexSemantic semantic, eVertexType type) const;
		int 
			GetElementOffset(eVertexSemantic semantic) const;

		VertexDeclaration & 
			operator =(const VertexDeclaration & rk);
		
	protected:
		FixedArray<VertexElement, MAX_HW_VELEMENT> mElements;
	};

}
