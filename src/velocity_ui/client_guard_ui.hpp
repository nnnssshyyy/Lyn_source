#pragma once
#include "config.hpp"
#include "../client_guard_logic.h"

namespace feature_ui {
struct GuardRules:config::custom_field {
    client_guard::Rules value=client_guard::DefaultRules();
    unsigned revision{1};
    GuardRules() {
        auto& registry=config::detail::get_registry();const auto key=config::detail::make_key("l4d2 protection","rules");
        config::detail::register_field({key,config::field_type::custom,this,1});
        registry.missing_defaults[key]=nlohmann::json::object();
        for(auto label:{"filter ConVar queries","filter server ConVars","filter server commands","filter user messages",
                       "block crosshair angle","block file CRC messages","block null sounds","mute heartbeat loop","log filtered events"})
            registry.missing_defaults[config::detail::make_key("l4d2 protection",label)]={{"v",false},{"b",nullptr}};
    }
    nlohmann::json serialize() const override {
        nlohmann::json out{{"queries",nlohmann::json::array()},{"settings",nlohmann::json::array()},
            {"commands",value.commands},{"messages",value.messages}};
        for(const auto& r:value.queries)out["queries"].push_back({{"name",r.name},{"value",r.value}});
        for(const auto& r:value.settings)out["settings"].push_back({{"name",r.name},{"value",r.value}});
        return out;
    }
    void Reset(){value=client_guard::DefaultRules();++revision;}
    void deserialize(const nlohmann::json& j) override {
        if(!j.is_object())return;
        if(j.empty()){Reset();return;}
        try {
            auto next=client_guard::DefaultRules();
            for(auto field:{"queries","settings"})if(j.contains(field)) {
                const auto& a=j.at(field);if(!a.is_array()||a.size()>client_guard::RuleLimit)return;
                auto& destination=std::string_view(field)=="queries"?next.queries:next.settings;destination.clear();
                for(const auto& r:a)destination.push_back({r.at("name").get<std::string>(),r.at("value").get<std::string>()});
            }
            for(auto field:{"commands","messages"})if(j.contains(field)) {
                const auto& a=j.at(field);if(!a.is_array()||a.size()>client_guard::RuleLimit)return;
                auto& destination=std::string_view(field)=="commands"?next.commands:next.messages;
                destination=a.get<std::vector<std::string>>();
            }
            if(client_guard::Valid(next)){value=std::move(next);++revision;}
        }catch(...){}
    }
};
inline GuardRules guard_rules;
void DrawGuardRules();
}
