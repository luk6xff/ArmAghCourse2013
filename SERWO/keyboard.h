#include "stdbool.h"

typedef enum {RELEASED, PRESSED} ButtonState;

typedef enum  {RELEASED_, BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4} KeyboardState;

void KeyboardInit(void);
ButtonState ReadButton1(void);
KeyboardState eKeyboard_Read(void);

