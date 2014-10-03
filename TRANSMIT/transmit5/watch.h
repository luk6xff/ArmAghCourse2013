#include "stdbool.h"

void WatchInit(void);
unsigned char WatchGetSeconds(void);
unsigned char WatchGetMinutes(void);
bool WatchAreSecondsChanged(void);
bool WatchAreMinutesChanged(void);
