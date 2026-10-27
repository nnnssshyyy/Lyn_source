// RECONSTRUCCION ESTATICA, NO CODIGO DE INYECCION NI IMPLEMENTACION PARA VS3.
// Binario: RZL4D2_[unknowncheats.me]_.dll, SHA256
// dfa8632e85c89c318577ef619f20bae2f4db5136042d680ee4dae4109234691a
// VA con ImageBase 0x10000000. API/estructuras simbolicas para mostrar el flujo.
// No se encontro una funcion de juego conectada a bFallDamage en esta copia.

// Configuracion comprobada:
// bool bFallDamage       @ 0x103F53A5
// int  iKeyFallDamage    @ 0x103F53A8
// bool bAddonBypass      @ 0x103F53A4 (otra opcion, byte anterior)

void NoFall_ConfigurationOnly() {
    // Carga: funcion 0x10003C90; escrituras 0x10009746 y 0x1000978C.
    bFallDamage    = ReadConfigBool("bFallDamage");
    iKeyFallDamage = ReadConfigInt("iKeyFallDamage");

    // Menu 0x100F46E0; argumentos guardados en 0x100F4715 y 0x100F471C.
    // Callback de usuario nulo. Se captura el puntero para dibujar el checkbox.
    WidgetsManager_Checkbox("Anti fall damage", &bFallDamage, &iKeyFallDamage,
                            /* optionalFloat */ nullptr,
                            /* customCallback */ nullptr);

    // Guardado: funcion 0x10102390; lecturas 0x1010C163 y 0x1010C1D4.
    SaveConfigBool("bFallDamage", bFallDamage);
    SaveConfigInt("iKeyFallDamage", iKeyFallDamage);
    // La tecla de desactivar todo pone ambos bytes de 0x103F53A4 a cero
    // en 0x100E8A18. Esto es escritura de reset, no lectura de gameplay.
}

// EL SIGUIENTE BLOQUE ES LAG MANUAL, NO LA IMPLEMENTACION DE NO FALL.
// Funcion 0x10146F50, llamada por CreateMove en 0x101701BF.
// Entradas V1: bool @ 0x103F5360, key @ 0x103F5364, factor @ 0x103F536C.
// Entradas V2: bool @ 0x103F5370, key @ 0x103F5374, factor @ 0x103F537C.
void LagV1V2_Reconstructed() {
    if (bLagExploitV1 == 1 && GetAsyncKeyState(ikeyLagExploitV1) < 0) {
        auto* local = GetLocalPlayer();
        if (local != nullptr) {
            if (local->lifeState == 0) {
                auto* channel = EngineVirtual74();
                channel->outSequenceNr += iFactorLagExploitV1; // +0x08; 0x10146FF1
                // El original no comprueba channel != nullptr en esta rama.
            }
            ForceSend = true; // 0x10146FF4; compartido con otros features.
        }
    }
    if (bLagExploitV2 == 1 && GetAsyncKeyState(ikeyLagExploitV2) < 0) {
        if (EngineVirtual74() != nullptr) {
            auto* channel = ReadPointer(ClientStateGlobal_103EC9A8 + 0x18);
            channel->outSequenceNr += iFactorLagExploitV2; // +0x08; 0x1014703E
        } else {
            ForceSend = true; // 0x10147043
        }
    }
    // No lee bFallDamage ni iKeyFallDamage; no comprueba suelo, velocidad Z,
    // altura o velocidad de caida. No escribe vida ni m_flFallVelocity.
}

// Traduccion de la solicitud compartida de envio, CreateMove 0x101706F7...:
void SharedSendPolicy(bool& sendPacket) {
    if (ForceSend == 1 && OtherFeatureSuppressesSend == 0)
        sendPacket = true; // escritura en 0x10170708
}

// Hay tambien dos sumas de V1 en 0x10147B2B y 0x10147B77 dentro del flujo de
// free camera (bFreeCamActive @ 0x103F5397), en 0x101472B0. Esa rama tampoco
// lee bFallDamage y no se debe presentar como un algoritmo anti-caida.
