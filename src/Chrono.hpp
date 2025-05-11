#ifndef _CHRONO_HPP
#define _CHRONO_HPP

#include <Arduino.h>

class Chrono {
    public:
        /**
         * @param period Intervalle en ms du chronomètre
         */
        Chrono(unsigned long period);

        /**
         * @brief Démarre le chronomètre
         * 
         */
        void start();

        /**
         * @brief Stope et remet à zéro le chronomètre 
         * 
         */
        void reset();

        /**
         * @brief Indique si la période du chronomètre a été dépassée depuis la dernière vérification
         */
        bool triggered();

        /**
         * @brief Indique si le temps du chronomètre a été atteint
         */
        bool isElapsed();

        /**
         * @brief Change la valeur du chrono en ms
         */
        void setPeriod(unsigned long period);

        /**
         * @brief Renvoie la date de démarrage du chrono. 0 si pas démarré.
         */
        unsigned long getStartTime();

        /**
         * @brief Renvoie les ms depuis le démarrage du chrono. 0 si pas démarré.
         */
        unsigned long getElapsedTime();

        /**
         * @brief Pour savoir si le chrono est pair ou impair en cas d'utilisation de triggered()
         */
        bool isEven();
    private:
        unsigned long startTime;
        unsigned long nextTriggerTime;
        unsigned long period;
        bool even; // pair
};

#endif