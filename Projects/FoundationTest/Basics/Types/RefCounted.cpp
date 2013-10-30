#include <PCH.h>
#include <Foundation/Basics/Types/RefCounted.h>

class RefCountedTestClass : public ezRefCounted
{
};

EZ_CREATE_SIMPLE_TEST(Basics, RefCounted)
{
  EZ_TEST_BLOCK(ezTestBlock::Enabled, "Ref Counting")
  {
    RefCountedTestClass Instance;

    EZ_TEST(Instance.GetRefCount() == 0);
    EZ_TEST(!Instance.IsReferenced());
    
    Instance.AddRef();

    EZ_TEST(Instance.GetRefCount() == 1);
    EZ_TEST(Instance.IsReferenced());

    /// Test scoped ref pointer
    {
      ezScopedRefPointer<RefCountedTestClass> ScopeTester(&Instance);

      EZ_TEST(Instance.GetRefCount() == 2);
      EZ_TEST(Instance.IsReferenced());
    }

    /// Test assignment of scoped ref pointer
    {
      ezScopedRefPointer<RefCountedTestClass> ScopeTester;

      ScopeTester = &Instance;

      EZ_TEST(Instance.GetRefCount() == 2);
      EZ_TEST(Instance.IsReferenced());

      ezScopedRefPointer<RefCountedTestClass> ScopeTester2;

      ScopeTester2 = ScopeTester;

      EZ_TEST(Instance.GetRefCount() == 3);
      EZ_TEST(Instance.IsReferenced());

      ezScopedRefPointer<RefCountedTestClass> ScopeTester3(ScopeTester);

      EZ_TEST(Instance.GetRefCount() == 4);
      EZ_TEST(Instance.IsReferenced());
    }

    /// Test copy constructor for ezRefCounted
    {
      RefCountedTestClass inst2(Instance);
      RefCountedTestClass inst3;
      inst3 = Instance;

      EZ_TEST(Instance.GetRefCount() == 1);
      EZ_TEST(Instance.IsReferenced());

      EZ_TEST(inst2.GetRefCount() == 0);
      EZ_TEST(!inst2.IsReferenced());

      EZ_TEST(inst3.GetRefCount() == 0);
      EZ_TEST(!inst3.IsReferenced());
    }

    EZ_TEST(Instance.GetRefCount() == 1);
    EZ_TEST(Instance.IsReferenced());

    Instance.ReleaseRef();

    EZ_TEST(Instance.GetRefCount() == 0);
    EZ_TEST(!Instance.IsReferenced());
  }
}

