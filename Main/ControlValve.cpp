#include "ControlValve.h"

#define DEBUG_ControlValve false
#define MainRelayValveTimeOUTSec 15000
bool Prev_MainRelayValVePosition_DO = DO_Off_Relay;
bool MainRelayValVePosition_DO = DO_Off_Relay;
bool SelectRelaysValvesPosition_DO = CLOSE_VALVE;
int PrevForceMode = -1, PrevRequetMode = -1;
unsigned long MainRelayValveTimeOUT = 0;
unsigned long MainRalyValveWait = 0;
unsigned long SelectValvesWaitTime = 0;
int ValveOpenDesicion = -1, ValveCloseDesicion = -1;

bool get_MainRelayValVePosition();

void turnMainRelayValve(bool DO_Value);


void StartupRelayValves() {
 //    turnMainRelayValve(DO_Off_Relay);

}

void turnMainRelayValve(bool DO_Value) {
    MainRelayValVePosition_DO = DO_Value;
    digitalWrite(MainRelayValve, MainRelayValVePosition_DO);
}

bool get_MainRelayValVePosition() {
    return MainRelayValVePosition_DO;
}

void CheckMainRelayValveTimeOut() {
    if (get_MainRelayValVePosition() == DO_Off_Relay) { // if in off posotion do not need chck anything
        Prev_MainRelayValVePosition_DO = get_MainRelayValVePosition();
        return;
    }
    if (Prev_MainRelayValVePosition_DO == DO_Off_Relay) { // if current position is on and in first time check
        MainRelayValveTimeOUT = millis();                         // set time out for main relay valve
        Prev_MainRelayValVePosition_DO = get_MainRelayValVePosition();
        return;
    }
    if ((millis() > MainRelayValveTimeOUT + MainRelayValveTimeOUTSec))// || (millis() > 10000 && millis() < 11000))
    {
        turnMainRelayValve(DO_Off_Relay);
        // todo SetAlarm()
    }
}

boolean Demo_PositionSwitchOPEN();

boolean Demo_PositionSwitchCLOSE();

int get_ValvePosition() {
    bool OpenPosition = digitalRead(PositionSwitchOPEN);//Demo_PositionSwitchOPEN(); //
    bool ClosePosition =  digitalRead(PositionSwitchCLOSE);//Demo_PositionSwitchCLOSE();//
    if (OpenPosition == false && ClosePosition == true)
        PrevForceMode = OPEN_VALVE;
    else if (OpenPosition == true && ClosePosition == false)
        PrevForceMode = CLOSE_VALVE;
    else PrevForceMode = ERROR_VALVE;
    return PrevForceMode;
}

bool get_SelectRelaysValvesPosition() {
    return SelectRelaysValvesPosition_DO;
}

bool SetSelectRelaysValvesPosition(bool Value) {
    SelectRelaysValvesPosition_DO = Value;
    digitalWrite(SelectRelayValve_1_, SelectRelaysValvesPosition_DO);
    digitalWrite(SelectRelayValve_2_, SelectRelaysValvesPosition_DO);

}

void ForceOpenValve() {
    if (get_ValvePosition() == OPEN_VALVE) {
        turnMainRelayValve(DO_Off_Relay);
        return;
    }
    if (PrevRequetMode != OPEN_VALVE) {
        PrevRequetMode = OPEN_VALVE;
        ValveOpenDesicion = 0;
    }
    switch (ValveOpenDesicion) {
        case 0:
            if (get_MainRelayValVePosition() == DO_On_Relay) {
                turnMainRelayValve(DO_Off_Relay);
                MainRalyValveWait = millis();
            } else
                MainRalyValveWait = 0;
            ValveOpenDesicion = 1;
            return;
            //do something when var equals 1
            //     break;
        case 1:
            if ((millis() > MainRalyValveWait + 1000) || (millis() > 1000 && millis() < 2000)) {
                // if (get_SelectRelaysValvesPosition() != OPEN_VALVE) {
                SetSelectRelaysValvesPosition(OPEN_VALVE);
                SelectValvesWaitTime = millis();
                // } else
                //  SelectValvesWaitTime = 0;
                ValveOpenDesicion = 2;
            }
            return;
            //do something when var equals 2
            //    break;
        case 2:
            if ((millis() > SelectValvesWaitTime + 1000) || (millis() > 1000 && millis() < 2000)) {
                turnMainRelayValve(DO_On_Relay);
            }
        default:
            // if nothing else matches, do the default
            // default is optional
            break;
    }
}

void ForceCloseValve() {

    if (get_ValvePosition() == CLOSE_VALVE) {
        turnMainRelayValve(DO_Off_Relay);
        return;
    }
    if (PrevRequetMode != CLOSE_VALVE) {

        PrevRequetMode = CLOSE_VALVE;
        ValveCloseDesicion = 0;
    }


    switch (ValveCloseDesicion) {
        case 0:
            if (get_MainRelayValVePosition() == DO_On_Relay) {
                turnMainRelayValve(DO_Off_Relay);
                MainRalyValveWait = millis();
            } else
                MainRalyValveWait = 0;
            ValveCloseDesicion = 1;
            return;
            //do something when var equals 1
            //     break;
        case 1:
            if ((millis() > MainRalyValveWait + 1000) || (millis() > 1000 && millis() < 2000)) {
                //    if (get_SelectRelaysValvesPosition() != CLOSE_VALVE) {
                SetSelectRelaysValvesPosition(CLOSE_VALVE);
                SelectValvesWaitTime = millis();
                //    } else
                //      SelectValvesWaitTime = 0;
                ValveCloseDesicion = 2;
            }
            return;
            //do something when var equals 2
            //    break;
        case 2:
            if ((millis() > SelectValvesWaitTime + 1000) || (millis() > 1000 && millis() < 2000)) {
                turnMainRelayValve(DO_On_Relay);
            }
        default:
            // if nothing else matches, do the default
            // default is optional
            break;
    }
}

