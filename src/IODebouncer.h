#ifndef IODEBOUNCER_H
#define IODEBOUNCER_H

#include <Arduino.h>

/**
 * @brief Gère la lecture de l'état d'une IO avec gestion des rebonds
 * 
 */
class IODebouncer {

    public:
        /**
         * @brief
         * 
         * @param pin IO du bouton
         * @param debouncePeriod Durée pendant laquelle le bouton doit être continuellement appuyé/relâché pour être considéré comme tel
         * @param highIsPressed Si true, l'état logique haut est considéré comme appuyé, si false, c'est l'état logique bas qui est considéré comme appuyé 
         */
        IODebouncer(int pin, unsigned long debouncePeriod, bool highIsPressed = true);

        /**
         * @brief Indique si le bouton est appuyé
         * 
         * @return true 
         * @return false 
         */
        bool isPressed();

        /**
         * @brief Indique si le bouton est relâché
         * 
         * @return true 
         * @return false 
         */
        bool isReleased();

        /**
         * @brief Indique si le bouton vient juste d'être appuyé
         * 
         * @return true 
         * @return false 
         */
        bool isJustPressed();

        /**
         * @brief Indique si le bouton vient juste d'être relâché
         * 
         * @return true 
         * @return false 
         */
        bool isJustReleased();

        /**
         * @brief Inidique si le bouton est appuyé ou non, snas débouncer
         */
        bool isOn();
    private:
        int pin;
        unsigned long debouncePeriod;
        bool highIsPressed;
        unsigned long lastPressed;
        unsigned long lastReleased;
        bool previousPressed;
        bool previousReleased;

        bool is(bool actualValue, unsigned long * last);

        bool isJust(bool actualValue, bool * previousValue);
};

#endif