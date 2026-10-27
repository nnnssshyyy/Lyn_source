#include "hooks.h"
#include "log.h"
#include "license.h"
#include "remote_hooks.h"

namespace {
DWORD WINAPI Start(void* parameter) {
    app::InitLog(static_cast<HMODULE>(parameter));
    app::Log("lyn - nueva base x86");
    try {
        for (unsigned i = 0; i < 600; ++i) {
            if (GetModuleHandleW(L"client.dll") && GetModuleHandleW(L"engine.dll") &&
                GetModuleHandleW(L"vguimatsurface.dll")) {
                app::Log("VS 3: esperando autorizacion firmada del loader.");
                while(!licensing::RefreshFromLoader()) Sleep(500);
                source::Interfaces api{};
                if (!source::Bind(api) || !hooks::Install(api)) app::Log("Inicio incompleto. Consulta las lineas anteriores.");
                unsigned ticks{},failures{};ULONGLONG next_plan{};
                for(;;) {
                    if(!(ticks++%4)) licensing::RefreshFromLoader();
                    const int activated=GetTickCount64()>=next_plan?remote_hooks::Pump():0;
                    if(activated>0) {app::Log("Servidor: %d hooks activados con firma valida.",activated);failures=0;}
                    else if(activated<0) {next_plan=GetTickCount64()+5000;if(failures++==0)app::Log("Esperando un plan de hooks firmado; se reintentara en segundo plano.");}
                    Sleep(500);
                }
            }
            Sleep(100);
        }
        app::Log("Tiempo agotado esperando los modulos de L4D2.");
    } catch (...) { app::Log("No se pudo inicializar la nueva base."); }
    return 0;
}
}
BOOL WINAPI DllMain(HINSTANCE module,DWORD reason,void*) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);
        HANDLE worker = CreateThread(nullptr,0,&Start,module,0,nullptr);
        if (!worker) return FALSE;
        CloseHandle(worker);
    }
    return TRUE;
}
