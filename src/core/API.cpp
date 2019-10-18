// The MIT License (MIT)
//
// Copyright (c) 2017-2019 Alexander Kurbatov

#include "API.h"
#include "Converter.h"
#include "Helpers.h"
#include "objects/GameObject.h"

#include <sc2api/sc2_map_info.h>

namespace API {

Action::Action(sc2::ActionInterface* action_): m_action(action_) {
}

void Action::Build(const Order& order_) {
    sc2::Unit unit = GameObject::ToUnit(order_.assignee);
    m_action->UnitCommand(&unit, order_.ability_id);
}

void Action::Build(const Order& order_, const sc2::Unit* unit_) {
    sc2::Unit unit = GameObject::ToUnit(order_.assignee);
    m_action->UnitCommand(&unit, order_.ability_id, unit_);
}

void Action::Build(const Order& order_, const sc2::Point2D& point_) {
    sc2::Unit unit = GameObject::ToUnit(order_.assignee);
    m_action->UnitCommand(&unit, order_.ability_id, point_);
}

void Action::Attack(const sc2::Units& units_, const sc2::Point2D& point_) {
    m_action->UnitCommand(units_, sc2::ABILITY_ID::ATTACK_ATTACK, point_);
}

void Action::Cast(const sc2::Unit& assignee_, sc2::ABILITY_ID ability_,
    const sc2::Unit& target_) {
    m_action->UnitCommand(&assignee_, convert::ToAbilityID(ability_), &target_);
}

void Action::Cancel(const sc2::Unit& assignee_) {
    m_action->UnitCommand(&assignee_, sc2::ABILITY_ID::CANCEL);
}

void Action::CancelConstruction(const sc2::Unit& assignee_) {
    m_action->UnitCommand(&assignee_, sc2::ABILITY_ID::CANCEL_BUILDINPROGRESS);
}

void Action::OpenGate(const sc2::Unit& assignee_) {
    m_action->UnitCommand(&assignee_, sc2::ABILITY_ID::MORPH_WARPGATE);
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

void Debug::DrawText(const std::string& message_, const sc2::Point2D& point_) const {
    m_debug->DebugTextOut(message_, sc2::Point3D(point_.x, point_.y, 0.35f));
}

void Debug::DrawText(const std::string& message_, const sc2::Point2DI& point_) const {
    sc2::Point3D dst = {
        static_cast<float>(point_.x),
        static_cast<float>(point_.y),
        0.35f
    };
    m_debug->DebugTextOut(message_, dst);
}

void Debug::DrawText(float value_, const sc2::Point3D& point_,
    const sc2::Color& color_) const {
    char buff[10] = {0};
    std::snprintf(buff, sizeof(buff), "%.2f", static_cast<double>(value_));

    m_debug->DebugTextOut(buff, {point_.x + 0.1f, point_.y + 0.1f, point_.z}, color_);
}

void Debug::DrawText(const std::string& message_, const sc2::Point3D& pos_) const {
    m_debug->DebugTextOut(message_, pos_);
}

void Debug::DrawSphere(const sc2::Point3D& center_, float radius_,
    const sc2::Color& color_) const {
    m_debug->DebugSphereOut(center_, radius_, color_);
}

void Debug::DrawTile(const sc2::Point3D& point_, const sc2::Color& color_) const {
    DrawBox(
        {point_.x + 0.1f, point_.y + 0.1f, point_.z},
        {point_.x + 0.8f, point_.y + 0.8f, point_.z},
        color_);
}

void Debug::DrawBox(const sc2::Point3D& min_, const sc2::Point3D& max_,
    const sc2::Color& color_) const {
    m_debug->DebugBoxOut(min_, max_, color_);
}

void Debug::DrawLine(const sc2::Point3D& start_, const sc2::Point3D& end_,
    const sc2::Color& color_) const {
    m_debug->DebugLineOut(start_, end_, color_);
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

Units Observer::GetUnits(sc2::Unit::Alliance alliance_) const {
    return Units(m_observer->GetUnits(alliance_));
}

Units Observer::GetUnits(const sc2::Filter& filter_,
    sc2::Unit::Alliance alliance_) const {
    return Units(m_observer->GetUnits(alliance_, filter_));
}

size_t Observer::CountUnitType(sc2::UNIT_TYPEID type_) const {
    return m_observer->GetUnits(IsUnit(type_)).size();
}

size_t Observer::CountUnitsTypes(const std::set<sc2::UNIT_TYPEID>& types_) {
    return m_observer->GetUnits(OneOfUnits(types_)).size();
}

const sc2::GameInfo& Observer::GameInfo() const {
    return m_observer->GetGameInfo();
}

sc2::Point3D Observer::StartingLocation() const {
    return m_observer->GetStartLocation();
}

sc2::Point2D Observer::GetCameraPos() const {
    return m_observer->GetCameraPos();
}

bool Observer::IsPlayableTile(const sc2::Point2DI& pos_) const {
    auto info = GameInfo();
    return sc2::Rect2DI(info.playable_min, info.playable_max).Contain(pos_);
}

float Observer::GetFoodCap() const {
    return static_cast<float>(m_observer->GetFoodCap());
}

float Observer::GetFoodUsed() const {
    return static_cast<float>(m_observer->GetFoodUsed());
}

uint32_t Observer::GetMinerals() const {
    return m_observer->GetMinerals();
}

uint32_t Observer::GetVespene() const {
    return m_observer->GetVespene();
}

float Observer::GetAvailableFood() const {
    return GetFoodCap() - GetFoodUsed();
}

sc2::UnitTypeData Observer::GetUnitTypeData(sc2::UNIT_TYPEID id_) const {
    sc2::UnitTypeData data = m_observer->GetUnitTypeData()[convert::ToUnitTypeID(id_)];

    switch (id_) {
        // NOTE (alkurbatov): Unfortunally SC2 API returns wrong mineral cost
        // and tech_requirement for orbital command, planetary fortress,
        // lair, hive and greater spire.
        // so we use a workaround.
        // See https://github.com/Blizzard/s2client-api/issues/191
        case sc2::UNIT_TYPEID::TERRAN_GHOST:
            data.tech_requirement = sc2::UNIT_TYPEID::TERRAN_GHOSTACADEMY;
            break;

        case sc2::UNIT_TYPEID::TERRAN_MARAUDER:
        case sc2::UNIT_TYPEID::TERRAN_REAPER:
            data.tech_requirement = sc2::UNIT_TYPEID::TERRAN_BARRACKSTECHLAB;
            break;

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
            data.food_required = 0.0f;
            data.tech_alias.push_back(sc2::UNIT_TYPEID::ZERG_ZERGLING);
            break;

        case sc2::UNIT_TYPEID::ZERG_BROODLORD:
            data.mineral_cost = 150;
            data.vespene_cost = 150;
            data.food_required = 2.0f;
            data.tech_alias.push_back(sc2::UNIT_TYPEID::ZERG_CORRUPTOR);
            data.tech_requirement = sc2::UNIT_TYPEID::ZERG_GREATERSPIRE;
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
            data.food_required = 1.0f;
            data.tech_alias.push_back(sc2::UNIT_TYPEID::ZERG_ROACH);
            data.tech_requirement = sc2::UNIT_TYPEID::ZERG_ROACHWARREN;
            break;

        case sc2::UNIT_TYPEID::ZERG_HIVE:
            data.mineral_cost = 200;
            data.vespene_cost = 150;
            data.tech_requirement = sc2::UNIT_TYPEID::ZERG_INFESTATIONPIT;
            break;

        case sc2::UNIT_TYPEID::ZERG_LURKERMP:
            data.mineral_cost = 50;
            data.vespene_cost = 100;
            data.ability_id = sc2::ABILITY_ID::MORPH_LURKER;
            data.food_required = 1.0f;
            data.tech_alias.push_back(sc2::UNIT_TYPEID::ZERG_HYDRALISK);
            data.tech_requirement = sc2::UNIT_TYPEID::ZERG_LURKERDENMP;
            break;

        // NOTE (alkurbatov): By some reason all zerg buildings
        // include drone mineral cost.
        case sc2::UNIT_TYPEID::ZERG_BANELINGNEST:
        case sc2::UNIT_TYPEID::ZERG_EVOLUTIONCHAMBER:
        case sc2::UNIT_TYPEID::ZERG_EXTRACTOR:
        case sc2::UNIT_TYPEID::ZERG_HYDRALISKDEN:
        case sc2::UNIT_TYPEID::ZERG_INFESTATIONPIT:
        case sc2::UNIT_TYPEID::ZERG_LURKERDENMP:
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

const sc2::ScoreDetails Observer::GetScoreDetails() const {
    return m_observer->GetScore().score_details;
}

bool Observer::HasCreep(const sc2::Point2D& point_) const {
    return m_observer->HasCreep(point_);
}

Query::Query(sc2::QueryInterface* query_): m_query(query_) {
}

bool Query::CanBePlaced(sc2::ABILITY_ID ability_id_, const sc2::Point2D& point_) {
    return m_query->Placement(ability_id_, point_);
}

bool Query::CanBePlaced(const Order& order_, const sc2::Point2D& point_) {
    return m_query->Placement(order_.ability_id, point_);
}

std::vector<bool> Query::CanBePlaced(
    const std::vector<sc2::QueryInterface::PlacementQuery>& queries_) {
    return m_query->Placement(queries_);
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
