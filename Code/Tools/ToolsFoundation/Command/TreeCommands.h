#pragma once

#include <ToolsFoundation/Basics.h>
#include <ToolsFoundation/Command/Command.h>
#include <ToolsFoundation/Document/Document.h>


class EZ_TOOLSFOUNDATION_DLL ezAddObjectCommand : public ezCommand
{
  EZ_ADD_DYNAMIC_REFLECTION(ezAddObjectCommand);

public:
  ezAddObjectCommand();

public: // Properties
  void SetType(const char* szType);
  const char* GetType() const;

  const ezRTTI* m_pType;
  ezUuid m_Parent;
  ezString m_sParentProperty;
  ezVariant m_Index;
  ezUuid m_NewObjectGuid; ///< This is optional. If not filled out, a new guid is assigned automatically.

private:
  virtual ezStatus Do(bool bRedo) override;
  virtual ezStatus Undo(bool bFireEvents) override;
  virtual void Cleanup(CommandState state) override;

private:
  ezDocumentObject* m_pObject;
};

class EZ_TOOLSFOUNDATION_DLL ezPasteObjectsCommand : public ezCommand
{
  EZ_ADD_DYNAMIC_REFLECTION(ezPasteObjectsCommand);

public:
  ezPasteObjectsCommand();

public: // Properties
  ezUuid m_Parent;
  ezString m_sJsonGraph;

private:
  virtual ezStatus Do(bool bRedo) override;
  virtual ezStatus Undo(bool bFireEvents) override;
  virtual void Cleanup(CommandState state) override;

private:
  struct PastedObject
  {
    ezDocumentObject* m_pObject;
    ezDocumentObject* m_pParent;
    ezString m_sParentProperty;
    ezVariant m_Index;
  };
  
  ezHybridArray<PastedObject, 4> m_PastedObjects;
};

class EZ_TOOLSFOUNDATION_DLL ezInstantiatePrefabCommand : public ezCommand
{
  EZ_ADD_DYNAMIC_REFLECTION(ezInstantiatePrefabCommand);

public:
  ezInstantiatePrefabCommand();

public: // Properties
  ezUuid m_Parent;
  ezUuid m_RemapGuid;
  ezString m_sJsonGraph;
  ezUInt64 m_pCreatedRootObjects; // ezHybridArray<ezUuid, 16>
  bool m_bAllowPickedPosition;

private:
  virtual ezStatus Do(bool bRedo) override;
  virtual ezStatus Undo(bool bFireEvents) override;
  virtual void Cleanup(CommandState state) override;

private:
  struct PastedObject
  {
    ezDocumentObject* m_pObject;
    ezDocumentObject* m_pParent;
    ezString m_sParentProperty;
    ezVariant m_Index;
  };

  ezHybridArray<PastedObject, 4> m_PastedObjects;
};

class EZ_TOOLSFOUNDATION_DLL ezRemoveObjectCommand : public ezCommand
{
  EZ_ADD_DYNAMIC_REFLECTION(ezRemoveObjectCommand);

public:
  ezRemoveObjectCommand();

public: // Properties
  ezUuid m_Object;

private:
  virtual ezStatus Do(bool bRedo) override;
  virtual ezStatus Undo(bool bFireEvents) override;
  virtual void Cleanup(CommandState state) override;

private:
  ezDocumentObject* m_pParent;
  ezString m_sParentProperty;
  ezVariant m_Index;
  ezDocumentObject* m_pObject;
};


class EZ_TOOLSFOUNDATION_DLL ezMoveObjectCommand : public ezCommand
{
  EZ_ADD_DYNAMIC_REFLECTION(ezMoveObjectCommand);

public:
  ezMoveObjectCommand();

public: // Properties
  ezUuid m_Object;
  ezUuid m_NewParent;
  ezString m_sParentProperty;
  ezVariant m_Index;

private:
  virtual ezStatus Do(bool bRedo) override;
  virtual ezStatus Undo(bool bFireEvents) override;
  virtual void Cleanup(CommandState state) override { }

private:
  ezDocumentObject* m_pObject;
  ezDocumentObject* m_pOldParent;
  ezDocumentObject* m_pNewParent;
  ezString m_sOldParentProperty;
  ezVariant m_OldIndex;
};

class EZ_TOOLSFOUNDATION_DLL ezSetObjectPropertyCommand : public ezCommand
{
  EZ_ADD_DYNAMIC_REFLECTION(ezSetObjectPropertyCommand);

public:
  ezSetObjectPropertyCommand();

public: // Properties
  ezUuid m_Object;
  ezVariant m_NewValue;
  ezVariant m_Index;
  ezString m_sPropertyPath;

  const char* GetPropertyPath() const { return m_sPropertyPath; }
  void SetPropertyPath(const char* szPath) { m_sPropertyPath = szPath; }

private:
  virtual ezStatus Do(bool bRedo) override;
  virtual ezStatus Undo(bool bFireEvents) override;
  virtual void Cleanup(CommandState state) override { }

private:
  ezDocumentObject* m_pObject;
  ezVariant m_OldValue;
};

class EZ_TOOLSFOUNDATION_DLL ezInsertObjectPropertyCommand : public ezCommand
{
  EZ_ADD_DYNAMIC_REFLECTION(ezInsertObjectPropertyCommand);

public:
  ezInsertObjectPropertyCommand();

public: // Properties
  ezUuid m_Object;
  ezVariant m_NewValue;
  ezVariant m_Index;
  ezString m_sPropertyPath;

  const char* GetPropertyPath() const { return m_sPropertyPath; }
  void SetPropertyPath(const char* szPath) { m_sPropertyPath = szPath; }

private:
  virtual ezStatus Do(bool bRedo) override;
  virtual ezStatus Undo(bool bFireEvents) override;
  virtual void Cleanup(CommandState state) override { }

private:
  ezDocumentObject* m_pObject;
};

class EZ_TOOLSFOUNDATION_DLL ezRemoveObjectPropertyCommand : public ezCommand
{
  EZ_ADD_DYNAMIC_REFLECTION(ezRemoveObjectPropertyCommand);

public:
  ezRemoveObjectPropertyCommand();

public: // Properties
  ezUuid m_Object;
  ezVariant m_Index;
  ezString m_sPropertyPath;

  const char* GetPropertyPath() const { return m_sPropertyPath; }
  void SetPropertyPath(const char* szPath) { m_sPropertyPath = szPath; }

private:
  virtual ezStatus Do(bool bRedo) override;
  virtual ezStatus Undo(bool bFireEvents) override;
  virtual void Cleanup(CommandState state) override { }

private:
  ezDocumentObject* m_pObject;
  ezVariant m_OldValue;
};

class EZ_TOOLSFOUNDATION_DLL ezMoveObjectPropertyCommand : public ezCommand
{
  EZ_ADD_DYNAMIC_REFLECTION(ezMoveObjectPropertyCommand);

public:
  ezMoveObjectPropertyCommand();

public: // Properties
  ezUuid m_Object;
  ezVariant m_OldIndex;
  ezVariant m_NewIndex;
  ezString m_sPropertyPath;

  const char* GetPropertyPath() const { return m_sPropertyPath; }
  void SetPropertyPath(const char* szPath) { m_sPropertyPath = szPath; }

private:
  virtual ezStatus Do(bool bRedo) override;
  virtual ezStatus Undo(bool bFireEvents) override;
  virtual void Cleanup(CommandState state) override { }

private:
  ezDocumentObject* m_pObject;
};
