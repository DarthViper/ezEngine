#include <PhysXPluginPCH.h>

#include <Core/Messages/EventMessage.h>
#include <Core/WorldSerializer/WorldReader.h>
#include <Core/WorldSerializer/WorldWriter.h>
#include <PhysXPlugin/Components/PxCharacterProxyComponent.h>
#include <PhysXPlugin/Components/PxRaycastInteractComponent.h>
#include <PhysXPlugin/WorldModule/PhysXWorldModule.h>


//////////////////////////////////////////////////////////////////////////

// clang-format off
EZ_IMPLEMENT_MESSAGE_TYPE(ezMsgTriggerRaycastInteractionComponent);
EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezMsgTriggerRaycastInteractionComponent, 1, ezRTTIDefaultAllocator<ezMsgTriggerRaycastInteractionComponent>)
{
  EZ_BEGIN_ATTRIBUTES
  {
    new ezAutoGenVisScriptMsgSender,
  }
  EZ_END_ATTRIBUTES;
}
EZ_END_DYNAMIC_REFLECTED_TYPE;

//////////////////////////////////////////////////////////////////////////

EZ_BEGIN_COMPONENT_TYPE(ezPxRaycastInteractComponent, 1, ezComponentMode::Static)
{
  EZ_BEGIN_PROPERTIES
  {
    EZ_MEMBER_PROPERTY("CollisionLayer", m_uiCollisionLayer)->AddAttributes(new ezDynamicEnumAttribute("PhysicsCollisionLayer")),
    EZ_MEMBER_PROPERTY("MaxDistance", m_fMaxDistance)->AddAttributes(new ezClampValueAttribute(0, ezVariant()), new ezDefaultValueAttribute(1.0f)),
    EZ_MEMBER_PROPERTY("UserMessage", m_sUserMessage),
  }
  EZ_END_PROPERTIES;
  EZ_BEGIN_MESSAGEHANDLERS
  {
    EZ_MESSAGE_HANDLER(ezMsgTriggerRaycastInteractionComponent, OnTrigger),
  }
  EZ_END_MESSAGEHANDLERS;
  EZ_BEGIN_ATTRIBUTES
  {
    new ezCategoryAttribute("Input"),
    new ezDirectionVisualizerAttribute(ezBasisAxis::PositiveX, 0.2f, ezColor::LightGreen, "MaxDistance"),
  }
  EZ_END_ATTRIBUTES;
}
EZ_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

ezPxRaycastInteractComponent::ezPxRaycastInteractComponent() {}
ezPxRaycastInteractComponent::~ezPxRaycastInteractComponent() {}

void ezPxRaycastInteractComponent::SerializeComponent(ezWorldWriter& stream) const
{
  SUPER::SerializeComponent(stream);
  auto& s = stream.GetStream();

  s << m_uiCollisionLayer;
  s << m_fMaxDistance;
  s << m_sUserMessage;
}

void ezPxRaycastInteractComponent::DeserializeComponent(ezWorldReader& stream)
{
  SUPER::DeserializeComponent(stream);
  // const ezUInt32 uiVersion = stream.GetComponentTypeVersion(GetStaticRTTI());
  auto& s = stream.GetStream();

  s >> m_uiCollisionLayer;
  s >> m_fMaxDistance;
  s >> m_sUserMessage;
}

void ezPxRaycastInteractComponent::OnTrigger(ezMsgTriggerRaycastInteractionComponent& msg)
{
  ezPhysXWorldModule* pModule = GetWorld()->GetOrCreateModule<ezPhysXWorldModule>();

  const ezVec3 vDirection = GetOwner()->GetGlobalDirForwards().GetNormalized();

  ezUInt32 uiIgnoreShapeID = ezInvalidIndex;

  ezGameObject* pCur = GetOwner();
  while (pCur)
  {
    ezPxCharacterProxyComponent* pProxy;
    if (pCur->TryGetComponentOfBaseType<ezPxCharacterProxyComponent>(pProxy))
    {
      uiIgnoreShapeID = pProxy->GetShapeId();
      break;
    }

    pCur = pCur->GetParent();
  }

  ezPhysicsHitResult res;
  if (!pModule->CastRay(GetOwner()->GetGlobalPosition(), vDirection, m_fMaxDistance, m_uiCollisionLayer, res,
                        ezPhysicsShapeType::Static | ezPhysicsShapeType::Dynamic, uiIgnoreShapeID))
  {
    return;
  }

  SendMessage(res);
}

void ezPxRaycastInteractComponent::SendMessage(const ezPhysicsHitResult& hit)
{
  ezMsgGenericEvent msg;
  msg.m_sMessage = m_sUserMessage;

  GetWorld()->SendMessage(hit.m_hActorObject, msg);
}