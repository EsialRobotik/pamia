#include "RoadMap.hpp"

RoadMap::RoadMap(int srfAddress, PAMIA_COLOR color, std::vector<RoadMapItem> items)
: srfAddress(srfAddress)
, color(color)
, items(items)
, itemsIndex(0)
{
}

bool RoadMap::hasNextItem() {
    return itemsIndex < items.size();
}

RoadMapItem RoadMap::getNextItem() {
    return items[itemsIndex++];
}

RoadMapItem RoadMap::current() {
    return items[itemsIndex - 1];
}

int RoadMap::getSrfAddress() {
    return srfAddress;
}

PAMIA_COLOR RoadMap::getColor() {
    return color;
}