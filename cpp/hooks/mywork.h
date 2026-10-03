#pragma once
#include "../il2cpp/il2cpp.h"
#include <functional>


bool PostToMainThread(std::function<void()> callback);
Il2CppObject* GetMainThreadSynchronizationContext();
Il2CppObject* TestGetInstance(const char* className);

void Test_Schedule_SpinSlot();
void Schedule_spinSlot();
void Schedule_SetBetState(int bet);
void get_CurrentBet();
void GetSpinResultSymbolsAnalyticFormat();
void spinSlot();
void m_SetBetState();

