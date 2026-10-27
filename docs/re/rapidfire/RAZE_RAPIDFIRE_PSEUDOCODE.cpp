// REFERENCE ONLY. Reconstructed with IDA Pro 9.4 / ida-pro-mcp.
// Raze SHA256 dfa8632e85c89c318577ef619f20bae2f4db5136042d680ee4dae4109234691a.
// Names/types are explanatory, not recovered source. Not compiled by this project.
// Unrelated features, exception machinery and UI string obfuscation are omitted.

int extra_moves = -1;           // 0x103F6DBC, shared by multiple features
int shift_ring[150][2];         // 0x103F690C, first dword of each 8-byte slot
bool force_send;                // 0x103F6908, shared flag

// 0x101486E0, rapidfire-specific tail. Requires local entity and initial move.
void ArmRapidfire(Player* local, Command* cmd) {
    if (!enabled) return;
    if (method < 0 || method > 2) return;
    if (!KeyHeld(configured_key)) {
        // +0x10 is a raw field in this binary. Its semantic name is unresolved.
        local->opaque_int_at_0x10 = max(local->opaque_int_at_0x10, 1) - 1;
        return;
    }
    int configured = method == 0 ? storm_speed
                   : method == 1 ? method2_speed : method3_speed;
    int n = max(configured, int(0.06f / globals.interval_per_tick + 0.5f));
    extra_moves = n;
    int correction = max(n, 0);
    shift_ring[cmd->number % 150][0] = correction;
    local->opaque_int_at_0x10 = min(local->opaque_int_at_0x10 + 1, correction);
    force_send = true;
}

// 0x1016CE80. This intentionally preserves the binary's loop and same args.
void HookCLMove(float samples, bool final) {
    int next = -1;
    do {
        extra_moves = next;
        OtherOptionalFeature();
        OriginalCLMove(samples, final);
        next = extra_moves - 1;
    } while (extra_moves > 0);
}

// Relevant ordering in 0x1016FF00. Original code has many other features.
void RelevantCreateMove(Command* cmd, bool* send_packet) {
    if (!cmd || !cmd->number) return;
    shift_ring[cmd->number % 150][0] = 0;
    Player* local = GetLocalPlayer();
    if (local && extra_moves == -1) {
        OtherOptionalArming();
        ArmRapidfire(local, cmd);
    }
    PredictionUpdate(delta_tick, delta_tick > 0,
                     last_command_ack, outgoing_command + choked_commands);
    OtherCommandFeatures();
    ApplyNetChannelMode(cmd);
    if (force_send && !other_suppression_flag) *send_packet = true;
    // The full hook chains OriginalCreateMove except specific other feature paths.
}

// Relevant portion of 0x1016CF10. The binary also supports nonlocal RunCommand.
void RelevantRunCommand(Player* player, Command* cmd, MoveHelper* helper) {
    if (special_prediction_guard && player == GetLocalPlayer()) return;
    if (Player* local = GetLocalPlayer()) {
        int shift = shift_ring[cmd->number % 150][0];
        local->tickbase -= shift;           // confirmed SUB at 0x1016CFD3
        if (player == local) saved_tickbase = local->tickbase;
    }
    OriginalRunCommand(player, cmd, helper);
    OtherPostPredictionFeatures();
}

// 0x10148C40. Raw offsets refer to the channel, not clientstate command fields.
void ApplyNetChannelMode(Command* cmd) {
    if (!enabled) return;
    if (method == 0) {
        NetChannel* channel = ChannelViaEngineRoot();
        if (KeyHeld(configured_key) && storm_ticks > 0) {
            int current = cmd->number % 150;
            int previous = (cmd->number - 1) % 150;
            if (current == previous) {      // unequal for normal positive numbers
                shift_ring[current][0] = 0;
                extra_moves = max(storm_ticks,
                    int(9.0f / globals.interval_per_tick + 9.0f));
            } else {
                ++storm_auxiliary_counter;
            }
            if (shift_ring[previous][0] == 0) {
                channel->int_at_0x10 = -1; // third sequence / outgoing ACK field
                channel->int_at_0x1c = 255;// packet choke count, serialized as byte
            }
        }
    } else if (method == 1) {
        if (KeyHeld(configured_key) && EngineGetNetChannelInfo()) {
            ChannelViaResolvedState()->int_at_0x1c = method2_choked_packets;
            force_send = true;
        }
    } else if (method == 2) {
        if (KeyHeld(configured_key)) {
            NetChannel* channel = ChannelViaEngineRoot();
            channel->int_at_0x10 = -1;
            channel->int_at_0x1c = 255;
            force_send = true;
        }
    }
}
