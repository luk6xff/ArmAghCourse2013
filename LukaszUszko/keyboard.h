#include "stdbool.h"

typedef enum {RELEASED, PRESSED} ButtonState;

typedef enum  {RELEASED_, BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4} KeyboardState;

void vToggleBitPort1(int ucPin);
bool ReadButton1(void);
void ButtonInit(void);
ButtonState ReadButtonEn1(void);
KeyboardState eKeyboard_Read(void);
void KeyboardInit(void);
