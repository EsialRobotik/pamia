#include "AsservManager.hpp"
#include <Arduino.h>

bool operator!=(const AsservData& l, const AsservData& r)
{
    return l.queueSize != r.queueSize || l.rad != r.rad || l.status != r.status || l.x != r.x || l.y != r.y;
}

AsservManager::AsservManager(Stream* asservSerial, Stream* debugSerial)
: asservSerial(asservSerial)
, debugSerial(debugSerial)
, asservData({0,0,0.,0,0,0})
{
}

void setPosition(int x, int y, float rad) {
    asservSerial->print('P');
    asservSerial->print(x);
    asservSerial->print('#');
    asservSerial->print(y);
    asservSerial->print('#');
    asservSerial->println(rad);
    asservSerial->flush();
}

void AsservManager::goTo(int x, int y) {
    asservSerial->print('g');
    asservSerial->print(x);
    asservSerial->print('#');
    asservSerial->println(y);
    asservSerial->flush();
}

void AsservManager::face(int x, int y) {
    asservSerial->print('f');
    asservSerial->print(x);
    asservSerial->print('#');
    asservSerial->println(y);
    asservSerial->flush();
}

void AsservManager::goToBack(int x, int y) {
    asservSerial->print('b');
    asservSerial->print(x);
    asservSerial->print('#');
    asservSerial->println(y);
    asservSerial->flush();
}

void AsservManager::goToChain(int x, int y) {
    asservSerial->print('e');
    asservSerial->print(x);
    asservSerial->print('#');
    asservSerial->println(y);
    asservSerial->flush();
}

void AsservManager::go(int distanceMm) {
    asservSerial->print('v');
    asservSerial->println(distanceMm);
    asservSerial->flush();
}

void AsservManager::turn(int degrees) {
    asservSerial->print('t');
    asservSerial->println(degrees);
    asservSerial->flush();
}



bool AsservManager::asservIdle() {
    return asservData.time > 0 && asservData.status == AsservStatus::IDLE;
}

void AsservManager::emergencyStop() {
    asservSerial->println('h');
    asservSerial->flush();
}

void AsservManager::emergencyReset() {
    asservSerial->println('r');
    asservSerial->flush();
}

bool AsservManager::asservAvailable() {
    return asservData.time > 0;
}

AsservData AsservManager::getLastData() {
    return asservData;
}

void AsservManager::heartBeat() {
    String line = asservSerial->readString();
    if (line.length() == 0 || line.indexOf('#') != 0) {
        return;
    }
    line = line.substring(1);
    
    AsservData ad;
    if (sscanf(
        line.c_str(),
        "%d;%d;%f;%d;%d;",
        &(ad.x),
        &(ad.y),
        &(ad.rad),
        &(ad.status),
        &(ad.queueSize)
    )) {
        ad.time = millis();
        // Débug de l'asserv que si les valeurs changent, sinon ça spam
        if (ad != asservData) {
            debugSerial->print("asserv : x");
            debugSerial->print(ad.x);
            debugSerial->print("; y");
            debugSerial->print(ad.y);
            debugSerial->print("; a");
            debugSerial->print(ad.rad);
            debugSerial->print("; s");
            debugSerial->print(ad.status);
            debugSerial->print("; q");
            debugSerial->println(ad.queueSize);
        }
        asservData = ad;
    }
}