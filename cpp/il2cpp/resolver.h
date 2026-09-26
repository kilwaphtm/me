#pragma once

#include "il2cpp.h"
#include <cstdint>

Il2CppImage* FindImage(
        const char* imageName
);
Il2CppImage* FindUnityInputImage();
bool FindUnityInputMethods();

Il2CppClass* FindClass(
        const char* imageName,
        const char* namespaceName,
        const char* className
);

const MethodInfo* FindMethod(
        const char* imageName,
        const char* namespaceName,
        const char* className,
        const char* methodName,
        int parameterCount = -1
);


extern Il2CppImage* g_InputImage;
extern Il2CppClass* g_InputClass;

extern const MethodInfo* g_GetTouchMethod;
extern const MethodInfo* g_GetTouchCountMethod;
extern const MethodInfo* g_GetMouseButtonMethod;


// =========================================================
// UNITY TOUCH TYPES
// =========================================================

struct UnityVector2
{
    float x;
    float y;
};

enum class UnityTouchPhase : int32_t
{
    Began      = 0,
    Moved      = 1,
    Stationary = 2,
    Ended      = 3,
    Canceled   = 4
};

enum class UnityTouchType : int32_t
{
    Direct   = 0,
    Indirect = 1,
    Stylus   = 2
};

struct UnityTouch
{
    int32_t m_FingerId;

    UnityVector2 m_Position;
    UnityVector2 m_RawPosition;
    UnityVector2 m_PositionDelta;

    float m_TimeDelta;

    int32_t m_TapCount;

    UnityTouchPhase m_Phase;
    UnityTouchType m_Type;

    float m_Pressure;
    float m_MaximumPossiblePressure;

    float m_Radius;
    float m_RadiusVariance;

    float m_AltitudeAngle;
    float m_AzimuthAngle;
};

bool GetUnityTouch(
        int index,
        UnityTouch& output
);