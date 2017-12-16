#ifndef GLOBALS_H
#define GLOBALS_H

    //#define DEBUG

    #ifdef DEBUG
    #define DEBUG_PRINT(x) delay(100); Serial.println(F(x))
    #else
        #define DEBUG_PRINT(x)
    #endif

#endif // GLOBALS_H