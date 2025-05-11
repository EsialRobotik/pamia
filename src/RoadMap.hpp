#ifndef _ROADMAP_HPP
#define _ROADMAP_HPP

#include <vector>

enum ROADMAP_COMMAND {
    GO_TO,
    WAIT,
    CELEBRATE,
};

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
        RoadMap(int srfAddress, std::vector<RoadMapItem> items);
        bool hasNextItem();
        RoadMapItem getNextItem();
        RoadMapItem current();
        int getSrfAddress();
    private:
        std::vector<RoadMapItem> items;
        unsigned int itemsIndex;
        int srfAddress;
};
#endif