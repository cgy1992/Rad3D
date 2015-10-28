#include "MVertexDeclaration.h"

namespace Rad {

	VertexDeclaration::VertexDeclaration()
	{
	}

	VertexDeclaration::~VertexDeclaration()
	{
	}

	void VertexDeclaration::Clear()
	{
		mElements.Clear();
	}

	void VertexDeclaration::AddElement(eVertexSemantic semantic, eVertexType type)
	{
		mElements.PushBack(VertexElement(semantic, type));
	}

	int VertexDeclaration::GetElementCount() const
	{
		return mElements.Size();
	}

	const VertexElement & VertexDeclaration::GetElement(int index) const
	{
		return mElements[index];
	}

	bool VertexDeclaration::HasElement(eVertexSemantic semantic, eVertexType type) const
	{
		for (int i = 0; i < mElements.Size(); ++i)
		{
			if (mElements[i].Semantic == semantic && mElements[i].Type == type)
				return true;
		}

		return false;
	}

	int VertexDeclaration::GetElementOffset(eVertexSemantic semantic) const
	{
		int offset = 0;

		for (int i = 0; i < mElements.Size(); ++i)
		{
			if (mElements[i].Semantic == semantic)
				return offset;

			offset += VertexElement::GetTypeSize(mElements[i].Type);
		}

		return -1;
	}

	int VertexDeclaration::GetVertexSize() const
	{
		int size = 0;

		for (int i = 0; i < mElements.Size(); ++i)
		{
			size += VertexElement::GetTypeSize(mElements[i].Type);
		}

		return size;
	}

	VertexDeclaration & VertexDeclaration::operator =(const VertexDeclaration & rk)
	{
		Clear();

		for (int i = 0; i < rk.mElements.Size(); ++i)
		{
			mElements.PushBack(rk.mElements[i]);
		}

		return *this;
	}

}