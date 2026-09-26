#pragma once

#include "il2cpp.h"

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