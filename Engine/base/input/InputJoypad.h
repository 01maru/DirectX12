#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <stdint.h>

#pragma comment (lib, "xinput.lib")

class Vector2D;

class InputJoypad
{
private:
    XINPUT_STATE state_{};
    XINPUT_STATE prevState_{};

    //  pad‚ª—LŒø‚©
    bool active_ = false;
private:
    void SetDeadZone(int16_t& sThumb, int32_t deaadzone);

    InputJoypad() {};
    ~InputJoypad() {};
public:
    static InputJoypad* GetInstance();
    InputJoypad(const InputJoypad& obj) = delete;
    InputJoypad& operator=(const InputJoypad& obj) = delete;

    void Update();

    //  Getter
    bool IsActive() { return active_; }
    bool GetButton(WORD button);
    bool GetTriggerButton(WORD button);
    bool GetLTrigger();
    bool GetRTrigger();
    Vector2D GetThumbR();
    Vector2D GetThumbL();
    bool GetTriggerThumbLX() { return ((state_.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && (prevState_.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) || ((state_.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && (prevState_.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)); }
    bool GetTriggerThumbLY() { return ((state_.Gamepad.sThumbLY > 0) && (prevState_.Gamepad.sThumbLY <= 0)) || ((state_.Gamepad.sThumbLY <= 0) && (prevState_.Gamepad.sThumbLY > 0)); }
};
