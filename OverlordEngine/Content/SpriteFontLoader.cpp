#include "stdafx.h"
#include "SpriteFontLoader.h"

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	//See BMFont Documentation for Binary Layout
	if (pReader->Read<char>() != 'B' || pReader->Read<char>() != 'M' || pReader->Read<char>() != 'F')
	{ 
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font");
		return nullptr;
	}
	
	//Parse the version (version 3 required)
	const int currentVersion = 3;
	if ((int)pReader->Read<char>() < currentVersion)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported");
		return nullptr;
	}

	//Valid .fnt file >> Start Parsing!
	//use this SpriteFontDesc to store all relevant information (used to initialize a SpriteFont object)
	SpriteFontDesc fontDesc{};

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	int blockId = (int)pReader->Read<char>();
	int blockSize = pReader->Read<int>();
	if (blockId != 1)
	{
		Logger::LogError(L"BlockId is invalid, expected 1, but received: {}", blockId);
	}
	//Retrieve the FontSize [fontDesc.fontSize]
	fontDesc.fontSize = pReader->Read<short>();
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	pReader->MoveBufferPosition(12);
	//Retrieve the FontName [fontDesc.fontName]

	char nextChar = pReader->Read<char>();
	while (nextChar != '\0')
	{
		fontDesc.fontName += nextChar;
		nextChar = pReader->Read<char>();
	}

	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = (int)pReader->Read<char>();
	blockSize = pReader->Read<int>();
	if (blockId != 2)
	{
		Logger::LogError(L"BlockId is invalid, expected 2, but received: {}", blockId);
	}
	//Retrieve Texture Width & Height [fontDesc.textureWidth/textureHeight]
	pReader->MoveBufferPosition(4);
	fontDesc.textureWidth = pReader->Read<short>();
	fontDesc.textureHeight = pReader->Read<short>();
	//Retrieve PageCount
	int pageCount = pReader->Read<short>();
	// Only 1 page is supported
	if (pageCount > 1)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only one texture per font is allowed!");
		return nullptr;
	}
	// Move to next block
	pReader->MoveBufferPosition(5);

	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = (int)pReader->Read<char>();
	blockSize = pReader->Read<int>();
	if (blockId != 3)
	{
		Logger::LogError(L"BlockId is invalid, expected 3, but received: {}", blockId);
	}
	//Retrieve the PageName (BinaryReader::ReadNullString)
	std::wstring pageName = L"";
	nextChar = pReader->Read<char>();
	while (nextChar != '\0')
	{
		pageName += nextChar;
		nextChar = pReader->Read<char>();
	}
	//Construct the full path to the page texture file
	//	>> page texture should be stored next to the .fnt file, pageName contains the name of the texture file
	//	>> full texture path = asset parent_path of .fnt file (see loadInfo.assetFullPath > get parent_path) + pageName (filesystem::path::append)
	//	>> Load the texture (ContentManager::Load<TextureData>) & Store [fontDesc.pTexture]
	std::wstring filePath = loadInfo.assetFullPath.parent_path().append(pageName);
	fontDesc.pTexture = ContentManager::Load<TextureData>(filePath);

	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = (int)pReader->Read<char>();
	blockSize = pReader->Read<int>();
	if (blockId != 4)
	{
		Logger::LogError(L"BlockId is invalid, expected 4, but received: {}", blockId);
	}
	//Retrieve Character Count (see documentation)
	const int charInfoSize = 20;
	int charCount = blockSize / charInfoSize;

	//> Create instance of FontMetric (struct)
	for (int i = 0; i < charCount; ++i)
	{
		FontMetric metric{};
		wchar_t charId = (wchar_t)pReader->Read<int>();

		// Retreive Information
		metric.character = charId;
		float posX = (float)pReader->Read<short>();
		float posY = (float)pReader->Read<short>();
		metric.width = pReader->Read<unsigned short>();
		metric.height = pReader->Read<unsigned short>();
		metric.offsetX = pReader->Read<short>();
		metric.offsetY = pReader->Read<short>();
		metric.advanceX = pReader->Read<short>();
		metric.page = pReader->Read<unsigned char>();
		int channel = (int)pReader->Read<char>();

		// The texture channel where the character image is found (1 = blue, 2 = green, 4 = red, 8 = alpha, 15 = all channels).
		switch(channel)
		{
		case 1:	metric.channel = 2;	break; // Blue
		case 2:	metric.channel = 1;	break; // Green
		case 4:	metric.channel = 0;	break; // Red
		case 8:	metric.channel = 3;	break; // Alpha
		default:
			Logger::LogError(L"SpriteFontLoader::LoadContent > correct channel not found for char {}.", charId);
			break;
		}

		//	> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
		metric.texCoord = XMFLOAT2(posX / (float)fontDesc.textureWidth, posY / (float)fontDesc.textureHeight);

		//> Insert new FontMetric to the metrics [font.metrics] map
		//	> key = (wchar_t) charId
		//	> value = new FontMetric
		//(loop restarts till all metrics are parsed)
		fontDesc.metrics[charId] = metric;
	}

	//Done!
	delete pReader;
	return new SpriteFont(fontDesc);
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
