// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Alexander Kurbatov

#include "API.h"
#include "Converter.h"
#include "Helpers.h"

#include <limits>
#include <memory>

namespace API {

Action::Action(sc2::ActionInterface* action_): m_action(action_) {
}

void Action::Build(const Order& order_) {
    m_action->UnitCommand(order_.assignee, order_.data.ability_id);
}

void Action::Build(const Order& order_, const sc2::Unit* unit_) {
    m_action->UnitCommand(order_.assignee, order_.data.ability_id, unit_);
}

void Action::Build(const Order& order_, const sc2::Point2D& point_) {
    m_action->UnitCommand(order_.assignee, order_.data.ability_id, point_);
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

size_t Observer::CountUnitType(sc2::UNIT_TYPEID type_) const {
    return GetUnits(IsUnit(type_)).size();
}

const sc2::GameInfo& Observer::GameInfo() const {
    return m_observer->GetGameInfo();
}

sc2::Point3D Observer::StartingLocation() const {
    return m_observer->GetStartLocation();
}

int32_t Observer::GetMinerals() const {
    return m_observer->GetMinerals();
}

int32_t Observer::GetVespene() const {
    return m_observer->GetVespene();
}

float Observer::GetAvailableFood() const {
    return static_cast<float>(
        m_observer->GetFoodCap() - m_observer->GetFoodUsed());
}

const sc2::UnitTypes& Observer::GetUnitTypeData() const {
    return m_observer->GetUnitTypeData();
}

const sc2::UnitTypeData& Observer::GetUnitTypeData(sc2::UNIT_TYPEID id_) const {
    return m_observer->GetUnitTypeData()[convert::ToUnitTypeID(id_)];
}

const std::vector<sc2::ChatMessage>& Observer::GetChatMessages() const {
    return m_observer->GetChatMessages();
}

Query::Query(sc2::QueryInterface* query_): m_query(query_) {
}

bool Query::CanBePlaced(const Order& order_, const sc2::Point2D& point_) {
    return m_query->Placement(order_.data.ability_id, point_);
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
