// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Alexander Kurbatov

#pragma once

#include "Order.h"
#include "Units.h"

#include <sc2api/sc2_control_interfaces.h>
#include <sc2api/sc2_gametypes.h>
#include <sc2api/sc2_interfaces.h>
#include <sc2api/sc2_score.h>

#include <memory>
#include <set>

namespace API {

struct Action {
    explicit Action(sc2::ActionInterface* action_);

    void Build(const Order& order_);

    void Build(const Order& order_, const sc2::Unit* unit_);

    void Build(const Order& order_, const sc2::Point2D& point_);

    void Attack(const sc2::Units& units_, const sc2::Point2D& point_);

    void Cast(const sc2::Unit& assignee_, sc2::ABILITY_ID ability_);

    void Cast(const sc2::Unit& assignee_, sc2::ABILITY_ID ability_,
        const sc2::Unit& target_, bool queued = false);

    void ToggleAutocast(sc2::Tag tag_, sc2::AbilityID ability_);

    void Cancel(const sc2::Unit& assignee_);

    void CancelConstruction(const sc2::Unit& assignee_);

    void OpenGate(const sc2::Unit& assignee_);

    void SendMessage(const std::string& text_);

 private:
    sc2::ActionInterface* m_action;
};

struct Control {
    explicit Control(sc2::ControlInterface* control_);

    void SaveReplay();

 private:
    sc2::ControlInterface* m_control;
};

struct Debug {
    explicit Debug(sc2::DebugInterface* debug_);

    void DrawText(const std::string& message_) const;

    void DrawText(const std::string& message_, const sc2::Point2D& point_) const;

    void DrawText(const std::string& message_, const sc2::Point2DI& point_) const;

    void DrawText(float value_, const sc2::Point3D& point_,
        const sc2::Color& color_ = sc2::Colors::White) const;

    void DrawText(const std::string& message_, const sc2::Point3D& pos_) const;

    void DrawSphere(const sc2::Point3D& center_, float radius_,
        const sc2::Color& color_ = sc2::Colors::White) const;

    void DrawTile(const sc2::Point3D& point_, const sc2::Color& color_) const;

    void DrawBox(const sc2::Point3D& min_, const sc2::Point3D& max_,
        const sc2::Color& color_ = sc2::Colors::White) const;

    void DrawLine(const sc2::Point3D& start_, const sc2::Point3D& end_,
        const sc2::Color& color_ = sc2::Colors::White) const;

    void EndGame() const;

    void SendDebug() const;

 private:
    sc2::DebugInterface* m_debug;
};

struct Observer {
    explicit Observer(const sc2::ObservationInterface* observer_);

    const sc2::Unit* GetUnit(sc2::Tag tag_) const;

    Units GetUnits(
        sc2::Unit::Alliance alliance_ = sc2::Unit::Alliance::Self) const;

    Units GetUnits(const sc2::Filter& filter_,
        sc2::Unit::Alliance alliance_ = sc2::Unit::Alliance::Self) const;

    size_t CountUnitType(sc2::UNIT_TYPEID type_) const;

    size_t CountUnitsTypes(const std::set<sc2::UNIT_TYPEID>& types_);

    const sc2::GameInfo& GameInfo() const;

    sc2::Point3D StartingLocation() const;

    sc2::Point2D GetCameraPos() const;

    bool IsPlayableTile(const sc2::Point2DI& pos_) const;

    float GetFoodCap() const;

    float GetFoodUsed() const;

    uint32_t GetMinerals() const;

    uint32_t GetVespene() const;

    uint32_t GetLarvaCount() const;

    float GetAvailableFood() const;

    sc2::UnitTypeData GetUnitTypeData(sc2::UNIT_TYPEID id_) const;

    sc2::UpgradeData GetUpgradeData(sc2::UPGRADE_ID id_) const;

    sc2::AbilityData GetAbilityData(sc2::ABILITY_ID id_) const;

    sc2::Race GetCurrentRace() const;

    std::string GetEnemyName() const;

    const std::vector<sc2::ChatMessage>& GetChatMessages() const;

    uint32_t GetGameLoop() const;

    const sc2::ScoreDetails GetScoreDetails() const;

    bool HasCreep(const sc2::Point2D& point_) const;

 private:
    const sc2::ObservationInterface* m_observer;
};

struct Query {
    explicit Query(sc2::QueryInterface* query_);

    bool CanBePlaced(sc2::ABILITY_ID ability_id_, const sc2::Point2D& point_);

    bool CanBePlaced(const Order& order_, const sc2::Point2D& point_);

    std::vector<bool> CanBePlaced(
        const std::vector<sc2::QueryInterface::PlacementQuery>& queries_);

 private:
    sc2::QueryInterface* m_query;
};

struct Interface {
    Interface(
        sc2::ActionInterface* action_,
        sc2::ControlInterface* control_,
        sc2::DebugInterface* debug_,
        const sc2::ObservationInterface* observer_,
        sc2::QueryInterface* query_);

    Action action() const;

    Control control() const;

    Debug debug() const;

    Observer observer() const;

    Query query() const;

 private:
    sc2::ActionInterface* m_action;
    sc2::ControlInterface* m_control;
    sc2::DebugInterface* m_debug;
    const sc2::ObservationInterface* m_observer;
    sc2::QueryInterface* m_query;
};

}  // namespace API

extern std::unique_ptr<API::Interface> gAPI;
