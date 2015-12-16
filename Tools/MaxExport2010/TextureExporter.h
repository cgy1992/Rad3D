/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

class TextureExporter : public Singleton<TextureExporter>
{
public:
	TextureExporter();
	~TextureExporter();

	void Clear();

	void Push(const char * texture);

	void Export();

protected:
	Set<FixedString256> mTextures;
};