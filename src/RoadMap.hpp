#ifndef _ROADMAP_HPP
#define _ROADMAP_HPP

#include <vector>
#include "enums.h"

struct RoadMapItem {
    ROADMAP_COMMAND command;
    unsigned long waitTime;
    int x;
    int y;
    int detectionThreshold; // Valeur en dessous de laquelle on considère qu'il y a un obstacle et qu'on doit s'arrêter
};

class RoadMap
{
    public:
        RoadMap(int srfAddress, PAMIA_COLOR color, std::vector<RoadMapItem> items);
        bool hasNextItem();
        RoadMapItem getNextItem();
        RoadMapItem current();
        int getSrfAddress();
        PAMIA_COLOR getColor();
    private:
        std::vector<RoadMapItem> items;
        unsigned int itemsIndex;
        int srfAddress;
        PAMIA_COLOR color;
};
#endif