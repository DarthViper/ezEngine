#include <Core/PCH.h>
#include <Core/Application/Config/ApplicationConfig.h>

EZ_BEGIN_STATIC_REFLECTED_TYPE(ezApplicationConfig, ezNoBase, 1, ezRTTINoAllocator );
EZ_END_STATIC_REFLECTED_TYPE

ezString ezApplicationConfig::s_sSdkRootDir;
ezString ezApplicationConfig::s_sProjectDir;


ezResult ezApplicationConfig::DetectSdkRootDirectory()
{
  ezStringBuilder sdkRoot;
  if (ezFileSystem::FindFolderWithSubPath(ezOSFile::GetApplicationDirectory(), "Data/Base", sdkRoot).Failed())
  {
    ezLog::Error("Could not find SDK root. Application dir is '%s'. Searched for parent with 'Data\\Base' sub-folder.", ezOSFile::GetApplicationDirectory());
    return EZ_FAILURE;
  }

  ezApplicationConfig::SetSdkRootDirectory(sdkRoot);
  return EZ_SUCCESS;
}

void ezApplicationConfig::SetSdkRootDirectory(const char* szSdkDir)
{
  ezStringBuilder s = szSdkDir;
  s.MakeCleanPath();

  s_sSdkRootDir = s;
}

const char* ezApplicationConfig::GetSdkRootDirectory()
{
  EZ_ASSERT_DEV(!s_sSdkRootDir.IsEmpty(), "The project directory has not been set through 'ezApplicationConfig::SetSdkDirectory'.");
  return s_sSdkRootDir.GetData();
}

void ezApplicationConfig::SetProjectDirectory(const char* szProjectDir)
{
  ezStringBuilder s = szProjectDir;
  s.MakeCleanPath();

  s_sProjectDir = s;
}

const char* ezApplicationConfig::GetProjectDirectory()
{
  EZ_ASSERT_DEV(!s_sProjectDir.IsEmpty(), "The project directory has not been set through 'ezApplicationConfig::SetProjectDirectory'.");
  return s_sProjectDir.GetData();
}




EZ_STATICLINK_FILE(Core, Core_Application_Config_Implementation_ApplicationConfig);

