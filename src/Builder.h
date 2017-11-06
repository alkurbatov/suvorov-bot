#ifndef BUILDER_H
#define BUILDER_H

struct Order;

struct Builder
{
    bool buildStructure(const sc2::Point2D &starting_point_, Order& order_);

    bool trainUnit(const Order& order_);
};

#endif
