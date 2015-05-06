/** @file
 * @author Orestis Zachariadis
 * @brief Processsor statistics. Currently only CPU Load.
 */
#include "statistics.hpp"

void CpuLoadTask(void const *args) {
    const uint32_t dt = 1e6;
    uint32_t tStart;
    uint32_t tLast;
    uint32_t tNow;
    uint32_t tAccumul;

    while (true) {
        tStart = us_ticker_read();
        tLast = tStart;
        tNow = tStart;
        tAccumul = 0;
        do {
            tNow = us_ticker_read();
            if (tNow == tLast + 1) {
                ++tAccumul;
            }
            tLast = tNow;
        } while (tNow - tStart < dt);
        //DEBUG_PRINT(("Load = %5.2f %%\r\n", 100.0 - 100.0 * (float) tAccumul / dt)); //TODO Send over USB ?
    }
}
