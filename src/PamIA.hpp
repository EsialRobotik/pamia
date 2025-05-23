#ifndef _PAMIA_HPP
#define _PAMIA_HPP

#include "enums.h"
#include "PamiHardware.hpp"
#include "Chrono.hpp"
#include "AsservManager.hpp"
#include "DetectionManager.hpp"
#include "RoadMap.hpp"
#include <vector>
#include <Stream.h>

class PamIA
{
    public:
        /**
         * @param serial
         * @param pamiHardware 
         * @param asservManager
         * @param detectionManager
         * @param roadMaps
         * @param matchDuration Temps en ms de fin de match à partir du retrait de la tirette
         */
        PamIA(Stream* serial, PamiHardWare* pamiHardware, AsservManager* asservManager, DetectionManager* detectionManager, std::vector<RoadMap> roadMaps, unsigned long matchDuration);
        
        /**
         * @brief Boucle d'évenement de l'IA à appeler très régulièrement
         */
        void heartBeat();

    private:
        Stream* serial;
        PamiHardWare* pamiHardware;
        AsservManager* asservManager;
        DetectionManager* detectionManager;
        std::vector<RoadMap> roadMaps;
        int roadMapIndexToUse;
        PAMIA_STATE state;
        Chrono chronoStopMoving;
        Chrono chronoStateTicker;
        Chrono chronoCelebration;
        bool obstacleDetected;
        unsigned long lastCommandStartTime;
        int celebrateServoMin;
        int celebrateServoMax;

        /**
         * @brief Passe à l'exécution de la commande suivante. S'il y a une commande suivante, renvoie true, sinon renvoie false
         */
        bool nextCommand();

        /**
         * @brief Exécute la commmand courante et indique si elle est terminée (true) ou pas (false)
         */
        bool currentCommandHeartBeat();
};

#endif