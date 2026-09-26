#pragma once

bool InstallEglHook();
void StartHookThread();

void UpdateImGuiTouch();

int HookGetTouchCount();
bool HookGetMouseButton(int button);

bool InstallUnityInputHooks();