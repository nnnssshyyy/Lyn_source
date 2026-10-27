// REFERENCE PSEUDOCODE, NOT COMPILED INTO VS 3.
// Raze SHA256: dfa8632e85c89c318577ef619f20bae2f4db5136042d680ee4dae4109234691a
// Reconstructed with IDA Pro MCP on 2026-10-21 (America/Lima).
// The JSON assembly exports are authoritative when Hex-Rays loses AVX flow.
// UI/font initialization and individual visual draw bodies are abstracted.

bool StreamProof_Enabled;                       // 0x103F5396; bStreamProof
uintptr_t StreamProof_OverlayReturnAddress;      // 0x103F73B0; initially zero
EndSceneFn StreamProof_OriginalEndScene;         // 0x103F746C
ResetFn StreamProof_OriginalReset;               // 0x103F7470

// 0x1016E6B0: install on device vtable[42], byte offset 0xA8.
void InstallEndScene(Device* device) {
    InstallHook(device->vtable[42], EndSceneHook, &StreamProof_OriginalEndScene);
}

// 0x1016E6D0; assembly epilogue retn 4 confirms a single stdcall argument.
HRESULT __stdcall EndSceneHook(Device* device) {
    const uintptr_t caller = ReturnAddressOfThisCall(); // [ebp+4]

    if (StreamProof_OverlayReturnAddress == 0) {
        MEMORY_BASIC_INFORMATION information;
        char filename[MAX_PATH];
        // The original does not check these two API return values here.
        VirtualQuery((void*)caller, &information, sizeof(information));
        GetModuleFileNameA((HMODULE)information.AllocationBase, filename, MAX_PATH);
        // Assembled on the stack from bytes, not a literal named DLL path.
        if (strstr(filename, "gameoverlay") != nullptr)
            StreamProof_OverlayReturnAddress = caller;
    }

    // 0x1016E780 -> 0x1016E963: reject BEFORE building any UI frame.
    // With the option off, other callers are also allowed to draw.
    if (StreamProof_Enabled && caller != StreamProof_OverlayReturnAddress)
        return StreamProof_OriginalEndScene(device);

    InitializeUiOnceForExistingGameWindowAndDevice(device);
    BeginUiFrame();
    UpdateCommonUiState();

    if (StreamProof_Enabled == 1) {
        // 0x1016E8AC..0x1016E930: thirteen alternate DX9 visual helpers.
        DrawAlternateVisualsAndWidgets();
    }
    // 0x1016E93B..0x1016E950: chat panels / other common UI.
    DrawCommonWindows();
    FinishUiFrame();

    // 0x1016E95B -> 0x1006CC30: current device render target.
    SubmitDx9DrawData(GetUiDrawData());
    return StreamProof_OriginalEndScene(device);
}

// 0x1016CC80: engine Paint callback, separate from the DX9 callback above.
int PaintHook(int mode) {
    const int result = OriginalPaint(mode);
    if ((mode & 1) == 0) return result;
    UpdateScreenInformation();
    if (!StreamProof_Enabled) {
        BeginSurfaceDrawing();
        DrawNormalSurfaceVisuals();
        EndSurfaceDrawing();
    } else {
        // Seven helpers at 0x1016CD6C..0x1016CDAE remain in this branch.
        // Their full contents are not expanded by this routing pseudocode.
        PrepareAlternateVisualState();
    }
    return ResultOfLastOriginalHelper();
}

// 0x1006CC30: assembly confirms vertex/index buffers and state preservation.
void SubmitDx9DrawData(DrawData* data) {
    if (data->DisplaySize.x <= 0 || data->DisplaySize.y <= 0) return;
    EnsureDynamicVertexAndIndexBuffers();
    StateBlock* saved = CreateStateBlock(ALL); // +0xEC, slot59; 0x1006CD3E
    saved->Capture();                         // +0x10, slot4; 0x1006CD56
    SaveTransformMatrices();
    UploadVerticesAndIndices();
    SetupDx9RenderState();                    // 0x1006D0D0
    SubmitClippedIndexedDrawsAndCallbacks();
    RestoreTransformMatrices();
    saved->Apply();                           // +0x14, slot5; 0x1006D09B
    saved->Release();
    // No additional swapchain, target rebinding or Present in this backend.
    // Error exits/cleanup are simplified in this reference representation.
}

// 0x1016EA00 installs device vtable[16], byte offset 0x40.
// 0x1016EA20 invalidates DX9 resources before Reset and recreates on success.
HRESULT __stdcall ResetHook(Device* device, Parameters* parameters) {
    InvalidateUiDeviceObjects();
    const HRESULT result = StreamProof_OriginalReset(device, parameters);
    if (result >= 0) {
        SavePresentationParameters(parameters);
        RecreateUiDeviceObjects();
    }
    return result;
}

// Also observed: material/glow-like engine draw paths at 0x1014D5B7 and
// 0x1014E630 do not submit their effects while StreamProof_Enabled != 0.
// The binary has no frame-number deduplication or FPS budget in EndSceneHook.
// Without discovery of a matching caller, enabled mode skips private drawing
// and repeats the module lookup; it is not a universal capture exclusion API.
