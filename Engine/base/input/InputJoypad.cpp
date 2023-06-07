#include "InputJoypad.h"
#include "Vector2D.h"

void InputJoypad::SetDeadZone(int16_t& sThumb, int32_t deaadzone)
{
    if ((sThumb < deaadzone) && sThumb > -deaadzone) sThumb = 0;
}

InputJoypad* InputJoypad::GetInstance()
{
    static InputJoypad instance;
    return &instance;
}

void InputJoypad::Update()
{
    prevState_ = state_;

    //  óLå¯Ç©ê›íË
    if (ERROR_SUCCESS == XInputGetState(0, &state_)) active_ = true;
    else                                             active_ = false;

    //  DeadZoneê›íË
    SetDeadZone(state_.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    SetDeadZone(state_.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    SetDeadZone(state_.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
    SetDeadZone(state_.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
}

bool InputJoypad::GetButton(WORD button)
{
    return (state_.Gamepad.wButtons & button) && active_;
}

bool InputJoypad::GetTriggerButton(WORD button)
{
    return (state_.Gamepad.wButtons & button) && !(prevState_.Gamepad.wButtons & button) && active_;
}

bool InputJoypad::GetLTrigger()
{
    return state_.Gamepad.bLeftTrigger != 0 && active_;
}

bool InputJoypad::GetRTrigger()
{
    return state_.Gamepad.bRightTrigger != 0 && active_;
}

Vector2D InputJoypad::GetThumbR()
{
    return Vector2D(state_.Gamepad.sThumbRX, state_.Gamepad.sThumbRY);
}

Vector2D InputJoypad::GetThumbL()
{
    return Vector2D(state_.Gamepad.sThumbLX, state_.Gamepad.sThumbLY);
}
