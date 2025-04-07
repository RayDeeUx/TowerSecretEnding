// original file by @acaruso-xx (github)
// adapted for the tower by raydeeux with permission

#pragma once

#include <Geode/Enums.hpp>
#include <cocos2d.h>

#include <queue>
#include <string>

//! @brief Delegate for @ref AssetDownloader.
class AssetDownloaderDelegate {
public:

	//! @brief Callback for when assets finish downloading.
	virtual void assetDownloadFinished() = 0;

	//! @brief Callback for when assets fail downloading.
	virtual void assetDownloadFailed() = 0;
};

class AssetDownloader : public cocos2d::CCNode,
						public MusicDownloadDelegate {
public:
	AssetDownloader() : m_delegate(nullptr), m_failed(false) {}
	~AssetDownloader() override;

	static AssetDownloader* create(GJGameLevel* level);
	bool init(GJGameLevel* level);

	//! @brief Start downloading assets.
	void download() {
		downloadNextAsset();
	}

	void setDelegate(AssetDownloaderDelegate* delegate) {
		m_delegate = delegate;
	}

	void loadSongInfoFinished(SongInfoObject* info) override;
	void loadSongInfoFailed(int id, GJSongError err) override;

	void downloadSongFinished(int id) override;
	void downloadSongFailed(int id, GJSongError err) override;

	void downloadSFXFinished(int id) override;
	void downloadSFXFailed(int id, GJSongError error) override;

	void musicActionFinished(GJMusicAction action) override;
	void musicActionFailed(GJMusicAction action) override;

private:
	//! @brief Asset type.
	//! @details Ideally, this would be replaced with GJAssetType.
	enum class AssetType {
		Song,
		SFX,
	};

	//! @brief A song or sound effect asset.
	struct Asset {
		Asset(AssetType type_, int id_) : type(type_), id(id_) {}

		AssetType type;
		int id;
	};

	//! @brief Parse asset strings and add them to the asset list.
	//! @param songID Song ID. Fallback for if songStr is empty.
	//! @param songStr Stringly-typed array of song IDs.
	//! @param sfxStr Stringly-typed array of sound effect IDs.
	void parse(
		const int songID,
		const std::string& songStr,
		const std::string& sfxStr
	);

	//! @brief Calls either finish or fail callback.
	void finish();

	//! @brief Downloads next asset. Called recursively.
	void downloadNextAsset();

	//! @brief Download a song asset.
	//! @param id Song ID.
	void downloadSong(int id);

	//! @brief Download a sound effect asset.
	//! @param id Sound effect ID.
	void downloadSFX(int id);

	AssetDownloaderDelegate* m_delegate;
	MusicDownloadManager* m_mgr; //!< This is used a lot.

	std::queue<Asset> m_assets;
	bool m_failed; //!< True if one or more asset fails to download.
};