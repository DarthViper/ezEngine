#include <PhysXCookingPCH.h>

#include <Core/Graphics/ConvexHull.h>
#include <Foundation/Configuration/Singleton.h>
#include <Foundation/Configuration/Startup.h>
#include <Foundation/IO/ChunkStream.h>
#include <Foundation/Math/BoundingBoxSphere.h>
#include <Foundation/Time/Stopwatch.h>
#include <Foundation/Utilities/Progress.h>
#include <PhysXCooking/PhysXCooking.h>
#include <PxPhysicsAPI.h>

#include <vhacd/public/VHACD.h>
using namespace VHACD;

// clang-format off
EZ_BEGIN_SUBSYSTEM_DECLARATION(PhysX, PhysXCooking)

  BEGIN_SUBSYSTEM_DEPENDENCIES
    "Foundation",
    "Core",
    "PhysXPlugin"
  END_SUBSYSTEM_DEPENDENCIES

  ON_CORESYSTEMS_STARTUP
  {
    ezPhysXCooking::Startup();
  }

  ON_CORESYSTEMS_SHUTDOWN
  {
    ezPhysXCooking::Shutdown();
  }

  ON_HIGHLEVELSYSTEMS_STARTUP
  {
  }

  ON_HIGHLEVELSYSTEMS_SHUTDOWN
  {
  }

EZ_END_SUBSYSTEM_DECLARATION;
// clang-format on

PxCooking* ezPhysXCooking::s_pCooking = nullptr;
ezPhysXInterface* ezPhysXCooking::s_pPhysX = nullptr;

void ezPhysXCooking::Startup()
{
  s_pPhysX = ezSingletonRegistry::GetRequiredSingletonInstance<ezPhysXInterface>();

  PxCookingParams params = PxCookingParams(s_pPhysX->GetPhysXAPI()->getTolerancesScale());

  s_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, s_pPhysX->GetPhysXAPI()->getFoundation(), params);
  EZ_ASSERT_DEV(s_pCooking != nullptr, "Initializing PhysX cooking API failed");
}

void ezPhysXCooking::Shutdown()
{
  if (s_pCooking)
  {
    s_pCooking->release();
    s_pCooking = nullptr;
  }
}

class ezPxOutStream : public PxOutputStream
{
public:
  ezPxOutStream(ezStreamWriter* pStream)
    : m_pStream(pStream)
  {
  }

  virtual PxU32 write(const void* src, PxU32 count) override
  {
    if (m_pStream->WriteBytes(src, count).Succeeded())
      return count;

    return 0;
  }

  ezStreamWriter* m_pStream;
};

class ezPxAllocator : public PxAllocatorCallback
{
public:
  virtual void* allocate(size_t size, const char* typeName, const char* filename, int line) override
  {
    if (size == 0)
      return nullptr;

    return new unsigned char[size];
  }

  virtual void deallocate(void* ptr) override
  {
    if (ptr != nullptr)
    {
      delete[] static_cast<unsigned char*>(ptr);
    }
  }
};

ezResult ezPhysXCooking::CookTriangleMesh(const ezPhysXCookingMesh& mesh, ezStreamWriter& OutputStream)
{
  // ezPhysXCookingMesh mesh;
  // if (ComputeConvexHull(mesh0, mesh).Failed())
  //{
  //  ezLog::Error("Convex Hull computation failed.");
  //  return EZ_FAILURE;
  //}

  ezProgressRange range("Cooking Triangle Mesh", 2, false);

  PxTriangleMeshDesc desc;
  desc.setToDefault();
  desc.materialIndices.data = mesh.m_PolygonSurfaceID.GetData();
  desc.materialIndices.stride = sizeof(ezUInt16);

  range.BeginNextStep("Preprocessing Mesh");

  ezDynamicArray<ezUInt32> TriangleIndices;
  CreateMeshDesc(mesh, desc, TriangleIndices);

  ezPxOutStream PassThroughStream(&OutputStream);

  range.BeginNextStep("PhysX Cooking");

  if (!s_pCooking->cookTriangleMesh(desc, PassThroughStream))
    return EZ_FAILURE;

  return EZ_SUCCESS;
}

ezResult ezPhysXCooking::CookConvexMesh(const ezPhysXCookingMesh& mesh0, ezStreamWriter& OutputStream)
{
  ezProgressRange range("Cooking Convex Mesh", 2, false);

  range.BeginNextStep("Computing Convex Hull");

  ezPhysXCookingMesh mesh;
  EZ_SUCCEED_OR_RETURN(ComputeConvexHull(mesh0, mesh));

  range.BeginNextStep("Cooking Convex Hull");

  EZ_SUCCEED_OR_RETURN(CookSingleConvexPxMesh(mesh, OutputStream));

  return EZ_SUCCESS;
}

ezResult ezPhysXCooking::CookSingleConvexPxMesh(const ezPhysXCookingMesh& mesh, ezStreamWriter& OutputStream)
{
  ezProgressRange range("Cooking Convex PhysX Mesh", 3, false);

  if (mesh.m_PolygonIndices.IsEmpty() || mesh.m_Vertices.IsEmpty())
  {
    ezLog::Error("Convex mesh data is empty.");
    return EZ_FAILURE;
  }

  if (mesh.m_VerticesInPolygon.GetCount() > 255)
  {
    ezLog::Error("Cannot cook convex meshes with more than 255 polygons. This mesh has {0}.", mesh.m_VerticesInPolygon.GetCount());
    return EZ_FAILURE;
  }

  PxSimpleTriangleMesh desc;
  desc.setToDefault();

  range.BeginNextStep("Preparing Mesh Data");

  ezDynamicArray<ezUInt32> TriangleIndices;
  CreateMeshDesc(mesh, desc, TriangleIndices);

  if (desc.triangles.count > 255)
  {
    ezLog::Error("Cannot cook convex meshes with more than 255 triangles. This mesh has {0}.", desc.triangles.count);
    return EZ_FAILURE;
  }

  range.BeginNextStep("Computing Hull Polygons");

  ezPxAllocator allocator;

  PxU32 uiNumVertices = desc.points.count, uiNumIndices = desc.triangles.count * 3, uiNumPolygons = 0;
  PxVec3* pVertices = nullptr;
  PxU32* pIndices = nullptr;
  PxHullPolygon* pPolygons = nullptr;
  if (!s_pCooking->computeHullPolygons(desc, allocator, uiNumVertices, pVertices, uiNumIndices, pIndices, uiNumPolygons, pPolygons))
  {
    ezLog::Error("Convex Hull computation failed");
    allocator.deallocate(pVertices);
    allocator.deallocate(pIndices);
    allocator.deallocate(pPolygons);
    return EZ_FAILURE;
  }

  PxConvexMeshDesc convex;
  convex.points.count = uiNumVertices;
  convex.points.data = pVertices;
  convex.points.stride = sizeof(PxVec3);

  convex.indices.count = uiNumIndices;
  convex.indices.data = pIndices;
  convex.indices.stride = sizeof(PxU32);

  convex.polygons.count = uiNumPolygons;
  convex.polygons.data = pPolygons;
  convex.polygons.stride = sizeof(PxHullPolygon);

  convex.vertexLimit = 256;

  range.BeginNextStep("PhysX Cooking");

  ezPxOutStream PassThroughStream(&OutputStream);
  if (!s_pCooking->cookConvexMesh(convex, PassThroughStream))
  {
    ezLog::Warning("Convex mesh cooking failed. Trying again with inflated mesh.");

    convex.flags.set(PxConvexFlag::eCOMPUTE_CONVEX);

    if (!s_pCooking->cookConvexMesh(convex, PassThroughStream))
    {
      allocator.deallocate(pVertices);
      allocator.deallocate(pIndices);
      allocator.deallocate(pPolygons);

      ezLog::Error("Convex mesh cooking failed with inflated mesh as well.");
      return EZ_FAILURE;
    }
  }

  allocator.deallocate(pVertices);
  allocator.deallocate(pIndices);
  allocator.deallocate(pPolygons);
  return EZ_SUCCESS;
}

ezResult ezPhysXCooking::ComputeConvexHull(const ezPhysXCookingMesh& mesh, ezPhysXCookingMesh& outMesh)
{
  ezStopwatch timer;

  outMesh.m_bFlipNormals = mesh.m_bFlipNormals;


  ezConvexHullGenerator gen;
  if (gen.Build(mesh.m_Vertices).Failed())
  {
    ezLog::Error("Computing the convex hull failed.");
    return EZ_FAILURE;
  }

  ezDynamicArray<ezConvexHullGenerator::Face> faces;
  gen.Retrieve(outMesh.m_Vertices, faces);

  if (faces.GetCount() >= 255)
  {
    ezConvexHullGenerator gen2;
    gen2.SetSimplificationMinTriangleAngle(ezAngle::Degree(30));
    gen2.SetSimplificationFlatVertexNormalThreshold(ezAngle::Degree(10));
    gen2.SetSimplificationMinTriangleEdgeLength(0.08f);

    if (gen2.Build(outMesh.m_Vertices).Failed())
    {
      ezLog::Error("Computing the convex hull failed (second try).");
      return EZ_FAILURE;
    }

    gen2.Retrieve(outMesh.m_Vertices, faces);
  }


  for (const auto& face : faces)
  {
    outMesh.m_VerticesInPolygon.ExpandAndGetRef() = 3;
    outMesh.m_PolygonSurfaceID.ExpandAndGetRef() = 0;

    for (int vert = 0; vert < 3; ++vert)
      outMesh.m_PolygonIndices.ExpandAndGetRef() = face.m_uiVertexIdx[vert];
  }

  ezLog::Dev("Computed the convex hull in {0} milliseconds", ezArgF(timer.GetRunningTotal().GetMilliseconds(), 1));
  return EZ_SUCCESS;
}

void ezPhysXCooking::CreateMeshDesc(const ezPhysXCookingMesh& mesh, PxSimpleTriangleMesh& desc, ezDynamicArray<ezUInt32>& TriangleIndices)
{
  desc.setToDefault();

  desc.points.count = mesh.m_Vertices.GetCount();
  desc.points.stride = sizeof(ezVec3);
  desc.points.data = mesh.m_Vertices.GetData();

  ezUInt32 uiTriangles = 0;
  for (auto numIndices : mesh.m_VerticesInPolygon)
    uiTriangles += numIndices - 2;

  TriangleIndices.SetCountUninitialized(uiTriangles * 3);

  ezUInt32 uiFirstIndex = 0;
  ezUInt32 uiFirstTriangleIdx = 0;
  for (auto numIndices : mesh.m_VerticesInPolygon)
  {
    for (ezUInt32 t = 2; t < numIndices; ++t)
    {
      TriangleIndices[uiFirstTriangleIdx + 0] = mesh.m_PolygonIndices[uiFirstIndex];
      TriangleIndices[uiFirstTriangleIdx + 1] = mesh.m_PolygonIndices[uiFirstIndex + t - 1];
      TriangleIndices[uiFirstTriangleIdx + 2] = mesh.m_PolygonIndices[uiFirstIndex + t];

      uiFirstTriangleIdx += 3;
    }

    uiFirstIndex += numIndices;
  }

  desc.triangles.count = uiTriangles;
  desc.triangles.stride = sizeof(ezUInt32) * 3;
  desc.triangles.data = TriangleIndices.GetData();

  desc.flags.set(mesh.m_bFlipNormals ? PxMeshFlag::eFLIPNORMALS : (PxMeshFlag::Enum)0);

  EZ_ASSERT_DEV(desc.isValid(), "PhysX PxTriangleMeshDesc is invalid");
}

ezStatus ezPhysXCooking::WriteResourceToStream(ezChunkStreamWriter& stream, const ezPhysXCookingMesh& mesh, const ezArrayPtr<ezString>& surfaces, MeshType meshType, ezUInt32 uiMaxConvexPieces)
{
  ezResult resCooking = EZ_FAILURE;

  {
    stream.BeginChunk("Surfaces", 1);

    stream << surfaces.GetCount();

    for (const auto& slot : surfaces)
    {
      stream << slot;
    }

    stream.EndChunk();
  }

  {
    stream.BeginChunk("Details", 1);

    ezBoundingBoxSphere aabb;
    aabb.SetFromPoints(mesh.m_Vertices.GetData(), mesh.m_Vertices.GetCount());

    stream << aabb;

    stream.EndChunk();
  }

  if (meshType == MeshType::Triangle)
  {
    stream.BeginChunk("TriangleMesh", 1);

    ezStopwatch timer;
    resCooking = ezPhysXCooking::CookTriangleMesh(mesh, stream);
    ezLog::Dev("Triangle Mesh Cooking time: {0}s", ezArgF(timer.GetRunningTotal().GetSeconds(), 2));

    stream.EndChunk();
  }
  else
  {
    if (meshType == MeshType::ConvexDecomposition)
    {
#ifdef BUILDSYSTEM_ENABLE_VHACD_SUPPORT
      stream.BeginChunk("ConvexDecompositionMesh", 1);

      ezStopwatch timer;
      resCooking = ezPhysXCooking::CookDecomposedConvexMesh(mesh, stream, uiMaxConvexPieces);
      ezLog::Dev("Decomposed Convex Mesh Cooking time: {0}s", ezArgF(timer.GetRunningTotal().GetSeconds(), 2));

      stream.EndChunk();
#else
      meshType = MeshType::ConvexHull;
      ezLog::SeriousWarning("Convex decomposition support is not available. Computing convex hull instead.");
#endif
    }

    if (meshType == MeshType::ConvexHull)
    {
      stream.BeginChunk("ConvexMesh", 1);

      ezStopwatch timer;
      resCooking = ezPhysXCooking::CookConvexMesh(mesh, stream);
      ezLog::Dev("Convex Mesh Cooking time: {0}s", ezArgF(timer.GetRunningTotal().GetSeconds(), 2));

      stream.EndChunk();
    }
  }

  if (resCooking.Failed())
    return ezStatus("Cooking the collision mesh failed.");


  return ezStatus(EZ_SUCCESS);
}

#ifdef BUILDSYSTEM_ENABLE_VHACD_SUPPORT
ezResult ezPhysXCooking::CookDecomposedConvexMesh(const ezPhysXCookingMesh& mesh, ezStreamWriter& OutputStream, ezUInt32 uiMaxConvexPieces)
{
  EZ_LOG_BLOCK("Decomposing Mesh");

  IVHACD* pConDec = CreateVHACD();
  IVHACD::Parameters params;
  params.Init();
  params.m_maxConvexHulls = ezMath::Max(1u, uiMaxConvexPieces);

  if (uiMaxConvexPieces <= 2)
  {
    params.m_resolution = 10 * 10 * 10;
  }
  else if (uiMaxConvexPieces <= 5)
  {
    params.m_resolution = 20 * 20 * 20;
  }
  else if (uiMaxConvexPieces <= 10)
  {
    params.m_resolution = 40 * 40 * 40;
  }
  else if (uiMaxConvexPieces <= 25)
  {
    params.m_resolution = 60 * 60 * 60;
  }
  else if (uiMaxConvexPieces <= 50)
  {
    params.m_resolution = 80 * 80 * 80;
  }
  else
  {
    params.m_resolution = 100 * 100 * 100;
  }

  if (!pConDec->Compute(mesh.m_Vertices.GetData()->GetData(), mesh.m_Vertices.GetCount(), mesh.m_PolygonIndices.GetData(), mesh.m_VerticesInPolygon.GetCount(), params))
  {
    ezLog::Error("Failed to compute convex decomposition");
    return EZ_FAILURE;
  }

  ezUInt16 uiNumParts = 0;

  for (ezUInt32 i = 0; i < pConDec->GetNConvexHulls(); ++i)
  {
    IVHACD::ConvexHull ch;
    pConDec->GetConvexHull(i, ch);

    if (ch.m_nTriangles == 0)
      continue;

    ++uiNumParts;
  }

  ezLog::Dev("Convex mesh parts: {}", uiNumParts);

  OutputStream << uiNumParts;

  for (ezUInt32 i = 0; i < pConDec->GetNConvexHulls(); ++i)
  {
    IVHACD::ConvexHull ch;
    pConDec->GetConvexHull(i, ch);

    if (ch.m_nTriangles == 0)
      continue;

    ezPhysXCookingMesh chm;

    chm.m_Vertices.SetCount(ch.m_nPoints);

    for (ezUInt32 v = 0; v < ch.m_nPoints; ++v)
    {
      chm.m_Vertices[v].Set((float)ch.m_points[v * 3 + 0], (float)ch.m_points[v * 3 + 1], (float)ch.m_points[v * 3 + 2]);
    }

    chm.m_VerticesInPolygon.SetCount(ch.m_nTriangles);
    chm.m_PolygonSurfaceID.SetCount(ch.m_nTriangles);
    chm.m_PolygonIndices.SetCount(ch.m_nTriangles * 3);

    for (ezUInt32 t = 0; t < ch.m_nTriangles; ++t)
    {
      chm.m_VerticesInPolygon[t] = 3;
      chm.m_PolygonSurfaceID[t] = 0;

      chm.m_PolygonIndices[t * 3 + 0] = ch.m_triangles[t * 3 + 0];
      chm.m_PolygonIndices[t * 3 + 1] = ch.m_triangles[t * 3 + 1];
      chm.m_PolygonIndices[t * 3 + 2] = ch.m_triangles[t * 3 + 2];
    }

    EZ_SUCCEED_OR_RETURN(CookSingleConvexPxMesh(chm, OutputStream));
  }

  return EZ_SUCCESS;
}

#endif

EZ_STATICLINK_FILE(PhysXCooking, PhysXCooking_PhysXCooking);
