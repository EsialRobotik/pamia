#include "RoadMap.hpp"

RoadMap::RoadMap(int srfAddress, std::vector<RoadMapItem> items)
: srfAddress(srfAddress)
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