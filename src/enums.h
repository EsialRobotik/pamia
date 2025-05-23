#ifndef _ENUMS_HPP
#define _ENUMS_HPP

enum PAMIA_STATE {
    WAIT_TIRETTE_PLUG = 1,
    WAITING_TIRETTE_UNPLUG = 2,
    NO_ROAD_MAP = 3,
    RUN_ROADMAP = 4,
    END = 5,
};

enum PAMIA_COLOR {
    YELLOW = 0,
    BLUE,
};

enum ROADMAP_COMMAND {
    GO_TO,
    WAIT,
    CELEBRATE,
};

#endif