#ifndef GAMEPAD_H_
#define GAMEPAD_H_
#include <stdio.h>

class DriverStation;

/**
 * Handle input from Logitech Dual Action Gamepad connected to the Driver
 * Station.
 */
class Gamepad
{
public:
    typedef enum
    {
        kLeftXAxis, kLeftYAxis, kRightXAxis, kRightYAxis, kLTrigger, kRTrigger
    } AxisType;

    typedef enum
    {
        kCenter, kUp, kUpLeft, kLeft, kDownLeft, kDown, kDownRight, kRight,
        kUpRight
    } DPadDirection;

    Gamepad(int port);
    ~Gamepad();

    float GetLeftX();
    float GetLeftY();
    float GetRightX();
    float GetRightY();
    float GetAxis(AxisType axis);

    bool GetNumberedButton(int buttonNumber);
    bool GetLeftPush();
    bool GetRightPush();
    bool GetA() { return GetNumberedButton(1); }
    bool GetB() { return GetNumberedButton(2); }
    bool GetY() { return GetNumberedButton(4); }
    bool GetX() { return GetNumberedButton(3); }
    bool GetLeftBumper() { return GetNumberedButton(5); }
    bool GetRightBumper() { return GetNumberedButton(6); }
    bool GetBack() { return GetNumberedButton(7); }
    bool GetStart() { return GetNumberedButton(8); }
    bool GetLeftTrigger() { return GetNumberedButton(7); }
    bool GetRightTrigger() { return GetNumberedButton(9); }
    float GetLTriggerAxis() { return GetRawAxis(Gamepad::kLeftTriggerAxis); }
    float GetRTriggerAxis() { return GetRawAxis(Gamepad::kRightTriggerAxis); }
    int GetPOV();

    DPadDirection GetDPad();

protected:
    typedef enum {
		kLeftXAxisNum = 0,
		kLeftYAxisNum = 1,
		kLeftTriggerAxis = 2,
		kRightTriggerAxis = 3,
		kRightXAxisNum = 4,
		kRightYAxisNum = 5,
		kDPadXAxisNum = 6,
		kDPadYAxisNum = 7
	} AxisNumber;

	typedef enum {
		kLeftAnalogStickButton  = 11,
		kRightAnalogStickButton = 12,
	} StickButton;

	float GetRawAxis(AxisNumber axis);
    DriverStation *ap_ds;
    int a_port;
};

#endif // GAMEPAD_H_
