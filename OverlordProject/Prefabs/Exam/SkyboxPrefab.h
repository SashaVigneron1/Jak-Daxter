#pragma once

class SkyboxPrefab :public GameObject
{
public:
	SkyboxPrefab(const std::wstring& assetFile);
	~SkyboxPrefab() override;

protected:

private:
	std::wstring m_AssetFile;
};