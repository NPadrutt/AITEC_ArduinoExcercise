#ifndef GENERAL_H
#define GENERAL_H

    //#define DEBUG

    #ifdef DEBUG
    #define DEBUG_PRINT(x) delay(100); Serial.println(F(x))
    #else
        #define DEBUG_PRINT(x)
    #endif

#endif // GENERAL_H