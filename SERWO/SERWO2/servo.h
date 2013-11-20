void DetectorInit(void);

typedef enum {
 ACTIVE,
 INACTIVE
}ServoStateDetector;

ServoStateDetector eReadDetector(void);

enum ServoState {CALLIB, IDDLE, IN_PROGRESS};

struct Servo
{
enum ServoState eState;
unsigned int uiCurrentPosition;
unsigned int uiDesiredPosition;
};




