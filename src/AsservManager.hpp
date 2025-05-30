#ifndef _ASSERVMANAGER_HPP
#define _ASSERVMANAGER_HPP

#include <Stream.h>
#include <Arduino.h>
#include <stdio.h>

enum AsservStatus {
    IDLE = 0,
    RUNNING = 1,
    HALTED = 2,
    BLOCKED = 3,
};

struct AsservData {
    int x;
    int y;
    float rad;
    int queueSize;
    int status;
    unsigned long time;
};

class AsservManager {
    public:
        AsservManager(Stream* asservSerial, Stream* debugSerial);
        /**
         * @brief Définit la position de la PAMI
         * 
         * @param x Position en mm sur l'axe X
         * @param y Position en mm sur l'axe Y
         * @param rad Angle en radians            
         */
        void setPosition(int x, int y, float rad);

        /**
         * @brief Va aux coordonnées indiquées
         */
        void goTo(int x, int y);

        /**
         * @brief Tourne la PAMI en direction des coordonnées indiquées
         */
        void face(int x, int y);

        /**
         * @brief Va aux coordonnées indiquées en marche arrière
         */
        void goToBack(int x, int y);

        /**
         * @brief Va aux coordonnées indiquées en enchainant les mouvements
         */
        void goToChain(int x, int y);

        /**
         * @brief Avance en ligne droit de la distance indiquée en mm
         */
        void go(int distanceMm);

        /**
         * @brief Tourne de l'angle indiqué en radians
         */
        void turn(int degrees);

        /**
         * @brief Arrêt d'urgence
         */
        void emergencyStop();

        /**
         * @brief Reset de l'arrêt d'urgence qui permet de resaisir de nouvelles commandes
         */
        void emergencyReset();

        /**
         * @brief Indique si l'asserv a fini de faire bouger la PAMI
         */
        bool asservIdle();

        /**
         * @brief Indique si l'asserv est disponible
         */
        bool asservAvailable();

        /**
         * @brief Denrière tram de données reçu de l'asserv
         */
        AsservData getLastData();

        /**
         * @brief Lit le retour de l'asserv et met à jour les données intéressantes
         * 
         */
        void heartBeat();

        /**
         * @brief Active ou désactive l'envoi de la plupart des commandes à l'asserv
         */
        void enableAsserv(bool enable);
    private:
        Stream* asservSerial;
        Stream* debugSerial;
        AsservData asservData;
        bool enabled;
};
#endif