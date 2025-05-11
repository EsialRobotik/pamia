#ifndef _DETECTIONMANAGER_HPP
#define _DETECTIONMANAGER_HPP

#include "PamiHardware.hpp"

class DetectionManager {

    public:
        DetectionManager(PamiHardWare* pamiHardware);
        /**
         * @brief Renvoie la istance de détection de l'obstacle
         */
        int getObstacleDistance();

        /**
         * @brief Actualise la détection de l'obstacle
         */
        void heartBeat();
    private:
        PamiHardWare* pamiHardware;
        unsigned int lastDistance;
};
#endif