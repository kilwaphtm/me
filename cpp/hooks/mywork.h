#pragma once
#include "../il2cpp/il2cpp.h"
#include <functional>


bool PostToMainThread(std::function<void()> callback);
Il2CppObject* GetMainThreadSynchronizationContext();
Il2CppObject* TestGetInstance(const char* className);


void get_CurrentBet(int action);
void Replace_spinSlot(int action);
void monitor_set_timeScale(int action);
void Schedule_set_timeScale(float speed);