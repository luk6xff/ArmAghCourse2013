/*void DetectorInit(void);
//cw1
typedef enum {
ACTIVE,
INACTIVE
}ServoStateDetector;

ServoStateDetector eReadDetector(void);

//cw2
enum ServoState {CALLIB, IDDLE, IN_PROGRESS};
struct Servo
{
enum ServoState eState;
unsigned int uiCurrentPosition;
unsigned int uiDesiredPosition;
};

 struct Servo sServo;
*/

void Servo_Init(unsigned int uiServoFrequency);
void Servo_Callib(void);
void Servo_GoTo(unsigned int uiPosition);

