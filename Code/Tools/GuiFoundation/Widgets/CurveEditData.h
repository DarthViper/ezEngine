#pragma once

#include <GuiFoundation/Basics.h>
#include <Foundation/Reflection/Reflection.h>
#include <Foundation/Math/Vec2.h>
#include <Foundation/Math/Color8UNorm.h>
#include <Foundation/Containers/DynamicArray.h>
#include <Foundation/Math/Curve1D.h>

class ezCurve1D;

EZ_DECLARE_REFLECTABLE_TYPE(EZ_GUIFOUNDATION_DLL, ezCurveTangentMode);

class EZ_GUIFOUNDATION_DLL ezCurve1DControlPoint : public ezReflectedClass
{
  EZ_ADD_DYNAMIC_REFLECTION(ezCurve1DControlPoint, ezReflectedClass);
public:

  double GetTickAsTime() const { return m_iTick / 4800.0; }
  void SetTickFromTime(double time, ezInt64 fps);

  //double m_fTime;
  ezInt64 m_iTick; // 4800 ticks per second
  double m_fValue;
  ezVec2 m_LeftTangent;
  ezVec2 m_RightTangent;
  bool m_bTangentsLinked = true;
  ezEnum<ezCurveTangentMode> m_LeftTangentMode;
  ezEnum<ezCurveTangentMode> m_RightTangentMode;
};

class EZ_GUIFOUNDATION_DLL ezCurve1DData : public ezReflectedClass
{
  EZ_ADD_DYNAMIC_REFLECTION(ezCurve1DData, ezReflectedClass);
public:
  ezColorGammaUB m_CurveColor;
  ezDynamicArray<ezCurve1DControlPoint> m_ControlPoints;

  void ConvertToRuntimeData(ezCurve1D& out_Result) const;
};

class EZ_GUIFOUNDATION_DLL ezCurve1DAssetData : public ezReflectedClass
{
  EZ_ADD_DYNAMIC_REFLECTION(ezCurve1DAssetData, ezReflectedClass);
public:

  ezDynamicArray<ezCurve1DData> m_Curves;
  ezUInt16 m_uiFramesPerSecond = 60;

  ezInt64 TickFromTime(double time);

  void ConvertToRuntimeData(ezUInt32 uiCurveIdx, ezCurve1D& out_Result) const;
};

struct EZ_GUIFOUNDATION_DLL ezSelectedCurveCP
{
  EZ_DECLARE_POD_TYPE();

  ezUInt16 m_uiCurve;
  ezUInt16 m_uiPoint;
};