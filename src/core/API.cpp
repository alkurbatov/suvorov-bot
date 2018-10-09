// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "API.h"
#include "Converter.h"
#include "Helpers.h"

#include <sc2api/sc2_map_info.h>

#include <limits>
#include <memory>

namespace API {

Action::Action(sc2::ActionInterface* action_): m_action(action_) {
}

void Action::Build(const Order& order_) {
    m_action->UnitCommand(order_.assignee, order_.ability_id);
}

void Action::Build(const Order& order_, const sc2::Unit* unit_) {
    m_action->UnitCommand(order_.assignee, order_.ability_id, unit_);
}

void Action::Build(const Order& order_, const sc2::Point2D& point_) {
    m_action->UnitCommand(order_.assignee, order_.ability_id, point_);
}

void Action::Attack(const sc2::Units& units_, const sc2::Point2D& point_) {
    m_action->UnitCommand(units_, sc2::ABILITY_ID::ATTACK_ATTACK, point_);
}

void Action::Cast(const sc2::Unit& assignee_, sc2::ABILITY_ID ability_,
    const sc2::Unit& target_) {
    m_action->UnitCommand(&assignee_, convert::ToAbilityID(ability_), &target_);
}

void Action::SendMessage(const std::string& text_) {
    m_action->SendChat(text_);
}

Control::Control(sc2::ControlInterface* control_): m_control(control_) {
}

void Control::SaveReplay() {
    m_control->SaveReplay("LastReplay.SC2Replay");
}

Debug::Debug(sc2::DebugInterface* debug_): m_debug(debug_) {
}

void Debug::DrawText(const std::string& message_) const {
    m_debug->DebugTextOut(message_);
}

void Debug::EndGame() const {
    m_debug->DebugEndGame(true);
    SendDebug();
}

void Debug::SendDebug() const {
    m_debug->SendDebug();
}

Observer::Observer(const sc2::ObservationInterface* observer_):
    m_observer(observer_) {
}

const sc2::Unit* Observer::GetUnit(sc2::Tag tag_) const {
    return m_observer->GetUnit(tag_);
}

sc2::Units Observer::GetUnits(sc2::Unit::Alliance alliance_) const {
    return m_observer->GetUnits(alliance_);
}

sc2::Units Observer::GetUnits(const sc2::Filter& filter_,
    sc2::Unit::Alliance alliance_) const {
    return m_observer->GetUnits(alliance_, filter_);
}

const sc2::Unit* Observer::GetClosestUnit(const sc2::Point2D& point_,
    const sc2::Filter& filter_, sc2::Unit::Alliance alliance_) const {
    float distance = std::numeric_limits<float>::max();

    sc2::Units units = GetUnits(filter_, alliance_);

    const sc2::Unit* target = nullptr;
    for (const auto& i : units) {
        float d = sc2::DistanceSquared2D(i->pos, point_);
        if (d < distance) {
            distance = d;
            target = i;
        }
    }

    return target;
}

size_t Observer::CountUnitType(sc2::UNIT_TYPEID type_, bool with_not_finished) const {
    return GetUnits(IsUnit(type_, with_not_finished)).size();
}

const sc2::GameInfo& Observer::GameInfo() const {
    return m_observer->GetGameInfo();
}

sc2::Point3D Observer::StartingLocation() const {
    return m_observer->GetStartLocation();
}

int32_t Observer::GetFoodCap() const {
    return m_observer->GetFoodCap();
}

int32_t Observer::GetFoodUsed() const {
    return m_observer->GetFoodUsed();
}

int32_t Observer::GetMinerals() const {
    return m_observer->GetMinerals();
}

int32_t Observer::GetVespene() const {
    return m_observer->GetVespene();
}

float Observer::GetAvailableFood() const {
    return static_cast<float>(GetFoodCap() - GetFoodUsed());
}

sc2::UnitTypeData Observer::GetUnitTypeData(sc2::UNIT_TYPEID id_) const {
    sc2::UnitTypeData data = m_observer->GetUnitTypeData()[convert::ToUnitTypeID(id_)];

    switch (id_) {
        // NOTE (alkurbatov): Unfortunally SC2 API returns wrong mineral cost
        // and tech_requirement for orbital command, planetary fortress,
        // lair, hive and greater spire.
        // so we use a workaround.
        // See https://github.com/Blizzard/s2client-api/issues/191
        case sc2::UNIT_TYPEID::TERRAN_ORBITALCOMMAND:
            data.mineral_cost = 150;
            data.tech_requirement = sc2::UNIT_TYPEID::TERRAN_BARRACKS;
            break;

        case sc2::UNIT_TYPEID::ZERG_GREATERSPIRE:
            data.mineral_cost = 100;
            data.vespene_cost = 150;
            data.tech_requirement = sc2::UNIT_TYPEID::ZERG_HIVE;
            break;

        case sc2::UNIT_TYPEID::TERRAN_PLANETARYFORTRESS:
            data.mineral_cost = 150;
            data.tech_requirement = sc2::UNIT_TYPEID::TERRAN_ENGINEERINGBAY;
            break;

        case sc2::UNIT_TYPEID::ZERG_BANELING:
            data.mineral_cost = 25;
            data.tech_alias.push_back(sc2::UNIT_TYPEID::ZERG_ZERGLING);
            break;

        case sc2::UNIT_TYPEID::ZERG_LAIR:
            data.mineral_cost = 150;
            data.tech_requirement = sc2::UNIT_TYPEID::ZERG_SPAWNINGPOOL;
            break;

        case sc2::UNIT_TYPEID::ZERG_OVERSEER:
            data.mineral_cost = 50;
            data.tech_requirement = sc2::UNIT_TYPEID::ZERG_LAIR;
            break;

        case sc2::UNIT_TYPEID::ZERG_RAVAGER:
            data.mineral_cost = 25;
            data.vespene_cost = 75;
            data.tech_alias.push_back(sc2::UNIT_TYPEID::ZERG_ROACH);
            data.tech_requirement = sc2::UNIT_TYPEID::ZERG_ROACHWARREN;
            break;

        case sc2::UNIT_TYPEID::ZERG_HIVE:
            data.mineral_cost = 200;
            data.vespene_cost = 150;
            data.tech_requirement = sc2::UNIT_TYPEID::ZERG_INFESTATIONPIT;
            break;

        // NOTE (alkurbatov): By some reason all zerg buildings
        // include drone mineral cost.
        case sc2::UNIT_TYPEID::ZERG_BANELINGNEST:
        case sc2::UNIT_TYPEID::ZERG_EVOLUTIONCHAMBER:
        case sc2::UNIT_TYPEID::ZERG_EXTRACTOR:
        case sc2::UNIT_TYPEID::ZERG_INFESTATIONPIT:
        case sc2::UNIT_TYPEID::ZERG_HYDRALISKDEN:
        case sc2::UNIT_TYPEID::ZERG_ROACHWARREN:
        case sc2::UNIT_TYPEID::ZERG_SPAWNINGPOOL:
        case sc2::UNIT_TYPEID::ZERG_SPINECRAWLER:
        case sc2::UNIT_TYPEID::ZERG_SPIRE:
        case sc2::UNIT_TYPEID::ZERG_SPORECRAWLER:
        case sc2::UNIT_TYPEID::ZERG_ULTRALISKCAVERN:
            data.mineral_cost -= 50;
            break;

        // NOTE (alkurbatov): There is no sense in summoning protoss buildings
        // without a pylon.
        case sc2::UNIT_TYPEID::PROTOSS_FORGE:
        case sc2::UNIT_TYPEID::PROTOSS_GATEWAY:
            data.tech_requirement = sc2::UNIT_TYPEID::PROTOSS_PYLON;
            break;

        default:
            break;
    }

    return data;
}

sc2::UpgradeData Observer::GetUpgradeData(sc2::UPGRADE_ID id_) const {
    return m_observer->GetUpgradeData()[convert::ToUpgradeID(id_)];
}

sc2::AbilityData Observer::GetAbilityData(sc2::ABILITY_ID id_) const {
    return m_observer->GetAbilityData()[convert::ToAbilityID(id_)];
}

sc2::Race Observer::GetCurrentRace() const {
    uint32_t id = m_observer->GetPlayerID();
    return m_observer->GetGameInfo().player_info[id - 1].race_actual;
}

const std::vector<sc2::ChatMessage>& Observer::GetChatMessages() const {
    return m_observer->GetChatMessages();
}

uint32_t Observer::GetGameLoop() const {
    return m_observer->GetGameLoop();
}

Query::Query(sc2::QueryInterface* query_): m_query(query_) {
}

bool Query::CanBePlaced(const Order& order_, const sc2::Point2D& point_) {
    return m_query->Placement(order_.ability_id, point_);
}

Interface::Interface(sc2::ActionInterface* action_,
    sc2::ControlInterface* control_, sc2::DebugInterface* debug_,
    const sc2::ObservationInterface* observer_, sc2::QueryInterface* query_):
    m_action(action_), m_control(control_), m_debug(debug_),
    m_observer(observer_), m_query(query_) {
}

Action Interface::action() const {
    return Action(m_action);
}

Control Interface::control() const {
    return Control(m_control);
}

Debug Interface::debug() const {
    return Debug(m_debug);
}

Observer Interface::observer() const {
    return Observer(m_observer);
}

Query Interface::query() const {
    return Query(m_query);
}

}  // namespace API

std::unique_ptr<API::Interface> gAPI;
