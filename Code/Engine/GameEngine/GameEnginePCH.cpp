#include <GameEnginePCH.h>

EZ_STATICLINK_LIBRARY(GameEngine)
{
  if (bReturn)
    return;

  EZ_STATICLINK_REFERENCE(GameEngine_AI_Implementation_AgentSteeringComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_AI_Implementation_NpcComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_AI_Implementation_PointOfInterestGraph);
  EZ_STATICLINK_REFERENCE(GameEngine_Animation_Implementation_ColorAnimationComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Animation_Implementation_MoveToComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Animation_Implementation_PropertyAnimComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Animation_Implementation_PropertyAnimResource);
  EZ_STATICLINK_REFERENCE(GameEngine_Animation_Implementation_RotorComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Animation_Implementation_SliderComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Animation_Implementation_TransformComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Animation_Skeletal_Implementation_AnimatedMeshComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Animation_Skeletal_Implementation_AnimationControllerComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Animation_Skeletal_Implementation_JointAttachmentComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Animation_Skeletal_Implementation_MotionMatchingComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Animation_Skeletal_Implementation_SimpleAnimationComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Configuration_Implementation_InputConfig);
  EZ_STATICLINK_REFERENCE(GameEngine_Configuration_Implementation_RendererProfileConfigs);
  EZ_STATICLINK_REFERENCE(GameEngine_Configuration_Implementation_XRConfig);
  EZ_STATICLINK_REFERENCE(GameEngine_DearImgui_Implementation_DearImgui);
  EZ_STATICLINK_REFERENCE(GameEngine_DearImgui_Implementation_DearImguiRenderer);
  EZ_STATICLINK_REFERENCE(GameEngine_Debugging_Implementation_LineToComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Effects_Wind_Implementation_SimpleWindComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Effects_Wind_Implementation_SimpleWindWorldModule);
  EZ_STATICLINK_REFERENCE(GameEngine_GameApplication_Implementation_GameApplication);
  EZ_STATICLINK_REFERENCE(GameEngine_GameApplication_Implementation_GameApplicationInit);
  EZ_STATICLINK_REFERENCE(GameEngine_GameApplication_Implementation_WindowOutputTargetGAL);
  EZ_STATICLINK_REFERENCE(GameEngine_GameState_Implementation_FallbackGameState);
  EZ_STATICLINK_REFERENCE(GameEngine_GameState_Implementation_GameState);
  EZ_STATICLINK_REFERENCE(GameEngine_Gameplay_Implementation_AreaDamageComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Gameplay_Implementation_GrabbableItemComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Gameplay_Implementation_GreyBoxComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Gameplay_Implementation_HeadBoneComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Gameplay_Implementation_InputComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Gameplay_Implementation_MarkerComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Gameplay_Implementation_PlayerStartPointComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Gameplay_Implementation_ProjectileComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Gameplay_Implementation_RaycastComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Gameplay_Implementation_SpawnComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Gameplay_Implementation_TimedDeathComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Messages_Implementation_DamageMessage);
  EZ_STATICLINK_REFERENCE(GameEngine_Physics_Implementation_CharacterControllerComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_Physics_Implementation_CollisionFilter);
  EZ_STATICLINK_REFERENCE(GameEngine_Physics_Implementation_SurfaceResource);
  EZ_STATICLINK_REFERENCE(GameEngine_Physics_Implementation_SurfaceResourceDescriptor);
  EZ_STATICLINK_REFERENCE(GameEngine_VisualScript_Implementation_VisualScriptComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_VisualScript_Implementation_VisualScriptInstance);
  EZ_STATICLINK_REFERENCE(GameEngine_VisualScript_Implementation_VisualScriptNode);
  EZ_STATICLINK_REFERENCE(GameEngine_VisualScript_Implementation_VisualScriptResource);
  EZ_STATICLINK_REFERENCE(GameEngine_VisualScript_Nodes_VisualScriptLogicNodes);
  EZ_STATICLINK_REFERENCE(GameEngine_VisualScript_Nodes_VisualScriptMathExpressionNode);
  EZ_STATICLINK_REFERENCE(GameEngine_VisualScript_Nodes_VisualScriptMathNodes);
  EZ_STATICLINK_REFERENCE(GameEngine_VisualScript_Nodes_VisualScriptMessageNodes);
  EZ_STATICLINK_REFERENCE(GameEngine_VisualScript_Nodes_VisualScriptObjectNodes);
  EZ_STATICLINK_REFERENCE(GameEngine_VisualScript_Nodes_VisualScriptReferenceNodes);
  EZ_STATICLINK_REFERENCE(GameEngine_VisualScript_Nodes_VisualScriptVariableNodes);
  EZ_STATICLINK_REFERENCE(GameEngine_XR_Implementation_Declaration);
  EZ_STATICLINK_REFERENCE(GameEngine_XR_Implementation_DeviceTrackingComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_XR_Implementation_SpatialAnchorComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_XR_Implementation_StageSpaceComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_XR_Implementation_VisualizeHandComponent);
  EZ_STATICLINK_REFERENCE(GameEngine_XR_Implementation_XRInputDevice);
  EZ_STATICLINK_REFERENCE(GameEngine_XR_Implementation_XRInterface);
  EZ_STATICLINK_REFERENCE(GameEngine_XR_Implementation_XRWindow);
}
