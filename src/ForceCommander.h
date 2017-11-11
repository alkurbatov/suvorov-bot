#ifndef FORCECOMMANDER_H
#define FORCECOMMANDER_H

#include <sc2api/sc2_unit.h>

struct ForceCommander
{
    ForceCommander();

    void onStep();

    void OnUnitCreated(const sc2::Unit* unit_);

private:
    sc2::Units m_units;
    bool m_attackStarted;
};

#endif
