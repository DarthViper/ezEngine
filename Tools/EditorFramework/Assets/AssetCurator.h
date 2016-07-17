#pragma once

#include <EditorFramework/Plugin.h>
#include <EditorFramework/Assets/AssetDocument.h>
#include <ToolsFoundation/Project/ToolsProject.h>
#include <ToolsFoundation/Document/DocumentManager.h>
#include <Foundation/Containers/HashTable.h>
#include <Foundation/Time/Timestamp.h>
#include <Foundation/Threading/Mutex.h>
#include <Foundation/Threading/TaskSystem.h>
#include <Core/Application/Config/FileSystemConfig.h>
#include <Foundation/Configuration/Singleton.h>

class ezUpdateTask;
class ezTask;
class ezAssetDocumentManager;
struct ezFileStats;

struct ezAssetInfo
{
  enum class ExistanceState
  {
    FileAdded,
    FileUnchanged,
    FileRemoved
  };

  enum class TransformState
  {
    Unknown,
    NeedsTransform,
    NeedsThumbnail,
    UpToDate,
  };

  ezAssetInfo() : m_pManager(nullptr)
  {
    m_ExistanceState = ExistanceState::FileAdded;
    m_TransformState = TransformState::Unknown;
    m_LastAssetDependencyHash = 0;
  }

  ExistanceState m_ExistanceState;
  TransformState m_TransformState;
  ezAssetDocumentManager* m_pManager;
  ezString m_sAbsolutePath;
  ezString m_sDataDirRelativePath;
  ezAssetDocumentInfo m_Info;
  ezTime m_LastAccess;
  ezUInt64 m_LastAssetDependencyHash; ///< For debugging only.
};

struct ezAssetCuratorEvent
{
  enum class Type
  {
    AssetAdded,
    AssetRemoved,
    AssetUpdated,
    AssetListReset,
    ActivePlatformChanged,
  };

  ezUuid m_AssetGuid;
  const ezAssetInfo* m_pInfo;
  Type m_Type;
};

class EZ_EDITORFRAMEWORK_DLL ezAssetCurator
{
  EZ_DECLARE_SINGLETON(ezAssetCurator);

public:
  ezAssetCurator();
  ~ezAssetCurator();

  void CheckFileSystem();

  /// \brief Transforms all assets and writes the lookup tables. If the given platform is empty, the active platform is used.
  void TransformAllAssets(const char* szPlatform = nullptr);
  ezStatus TransformAsset(const ezUuid& assetGuid, const char* szPlatform = nullptr);
  ezStatus CreateThumbnail(const ezUuid& assetGuid);

  /// \brief Writes the asset lookup table for the given platform, or the currently active platform if nullptr is passed.
  ezResult WriteAssetTables(const char* szPlatform = nullptr);

  void MainThreadTick();

  void Initialize(const ezApplicationFileSystemConfig& cfg);
  void Deinitialize();

  const ezAssetInfo* FindAssetInfo(const char* szRelativePath) const;

  const ezAssetInfo* GetAssetInfo(const ezUuid& assetGuid) const;
  const ezHashTable<ezUuid, ezAssetInfo*>& GetKnownAssets() const;

  void UpdateAssetLastAccessTime(const ezUuid& assetGuid);

  /// \brief Computes the combined hash for the asset and its dependencies. Returns 0 if anything went wrong.
  ezUInt64 GetAssetDependencyHash(ezUuid assetGuid);

  /// \brief Computes the combined hash for the asset and its references. Returns 0 if anything went wrong.
  ezUInt64 GetAssetReferenceHash(ezUuid assetGuid);

  /// \brief Iterates over all known data directories and returns the absolute path to the directory in which this asset is located
  ezString FindDataDirectoryForAsset(const char* szAbsoluteAssetPath) const;

  // TODO: Background file-system watcher and main thread update tick to add background info to main data
  // TODO: Hash changed in different thread

  const char* GetActivePlatform() const { return m_sActivePlatform; }
  void SetActivePlatform(const char* szPlatform);

  /// \brief Allows to tell the system of a new or changed file, that might be of interest to the Curator.
  void NotifyOfFileChange(const char* szAbsolutePath);

  /// \brief Allows to tell the system to re-evaluate an assets status.
  void NotifyOfAssetChange(const ezUuid& assetGuid);

  /// \brief The curator gathers all folders in which assets have been found. This list can only grow over the lifetime of the application.
  const ezSet<ezString>& GetAllAssetFolders() const { return m_AssetFolders; }

  ezAssetInfo::TransformState IsAssetUpToDate(const ezUuid& assetGuid, const char* szPlatform, const ezDocumentTypeDescriptor* pTypeDescriptor, ezUInt64& out_AssetHash, ezUInt64& out_ThumbHash);


  /// \brief Returns the number of assets in the system and how many are in what transform state
  void GetAssetTransformStats(ezUInt32& out_uiNumAssets, ezUInt32& out_uiNumUnknown, ezUInt32& out_uiNumNeedTransform, ezUInt32& out_uiNumNeedThumb);

public:

  ezEvent<const ezAssetCuratorEvent&> m_Events;


private:

  /// \brief Information about a single file on disk. The file might be an asset or any other file (needed for dependencies).
  struct FileStatus
  {
    enum class Status
    {
      Unknown,    ///< Since the file has been tagged as 'Unknown' it has not been encountered again on disk (yet)
      FileLocked, ///< The file is probably an asset, but we could not read it
      Valid       ///< The file exists on disk
    };

    FileStatus()
    {
      m_uiHash = 0;
      m_Status = Status::Unknown;
    }

    ezTimestamp m_Timestamp;
    ezUInt64 m_uiHash;
    ezUuid m_AssetGuid; ///< If the file is linked to an asset, the GUID is valid, otherwise not.
    Status m_Status;
  };

  ezStatus ProcessAsset(ezAssetInfo* pAssetInfo, const char* szPlatform);
  void DocumentManagerEventHandler(const ezDocumentManager::Event& r);
  static void BuildFileExtensionSet(ezSet<ezString>& AllExtensions);
  void IterateDataDirectory(const char* szDataDir, const ezSet<ezString>& validExtensions);
  void HandleSingleFile(const ezString& sAbsolutePath);
  void HandleSingleFile(const ezString& sAbsolutePath, const ezSet<ezString>& validExtensions, const ezFileStats& FileStat);

  void SetAllAssetStatusUnknown();
  void RemoveStaleFileInfos();

  ezResult EnsureAssetInfoUpdated(const ezUuid& assetGuid);
  ezResult EnsureAssetInfoUpdated(const char* szAbsFilePath);
  ezUInt64 GetAssetHash(ezUuid assetGuid, bool bReferences);

  static ezResult UpdateAssetInfo(const char* szAbsFilePath, ezAssetCurator::FileStatus& stat, ezAssetInfo& assetInfo, const ezFileStats* pFileStat);
  void UpdateTrackedFiles(const ezUuid& assetGuid, const ezSet<ezString>& files, ezMap<ezString, ezHybridArray<ezUuid, 1> >& inverseTracker, bool bAdd);
  void TrackDependencies(ezAssetInfo* pAssetInfo);
  void UntrackDependencies(ezAssetInfo* pAssetInfo);
  void UpdateAssetTransformState(const ezUuid& assetGuid, ezAssetInfo::TransformState state);

private:
  void RestartUpdateTask();
  void ShutdownUpdateTask();

  bool m_bRunUpdateTask;
  ezSet<ezUuid> m_TransformStateUnknown;
  ezSet<ezUuid> m_TransformStateNeedsTransform;
  ezSet<ezUuid> m_TransformStateNeedsThumbnail;
  ezSet<ezUuid> m_TransformStateChanged;

  ezHashTable<ezUuid, ezAssetInfo*> m_KnownAssets;
  ezMap<ezString, FileStatus, ezCompareString_NoCase<ezString> > m_ReferencedFiles;
  ezMap<ezString, ezHybridArray<ezUuid, 1> > m_InverseDependency;
  ezMap<ezString, ezHybridArray<ezUuid, 1> > m_InverseReferences;
  ezApplicationFileSystemConfig m_FileSystemConfig;
  ezString m_sActivePlatform;
  ezSet<ezString> m_ValidAssetExtensions;
  ezSet<ezString> m_AssetFolders;

private:
  friend class ezUpdateTask;

  /// \brief Computes the hash of the given file. Optionally passes the data stream through into another stream writer.
  static ezUInt64 HashFile(ezStreamReader& InputStream, ezStreamWriter* pPassThroughStream);

  /// \brief Opens the asset JSON file and reads the "Header" into the given ezAssetDocumentInfo.
  static void ReadAssetDocumentInfo(ezAssetDocumentInfo* pInfo, ezStreamReader& stream);


  bool GetNextAssetToUpdate(ezUuid& guid, ezStringBuilder& out_sAbsPath);
  void OnUpdateTaskFinished(ezTask* pTask);

  /// \brief Writes the asset lookup table for the given platform, or the currently active platform if nullptr is passed.
  ezResult WriteAssetTable(const char* szDataDirectory, const char* szPlatform = nullptr);

  void RunNextUpdateTask();

  mutable ezMutex m_CuratorMutex;
  ezUpdateTask* m_pUpdateTask;

};

class ezUpdateTask : public ezTask
{
public:
  ezUpdateTask();


private:
  ezStringBuilder m_sAssetPath;

  virtual void Execute() override;
};