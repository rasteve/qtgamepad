// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

/*
  Originally based on code from "platform/windows/joypad_windows.h" from Godot Engine v4.0
  Copyright (c) 2014-present Godot Engine contributors
  Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
*/

#ifndef WINDOWSJOYSTICKINPUT_H
#define WINDOWSJOYSTICKINPUT_H

#include <QtUniversalInput/private/qjoystickinput_p.h>

#include <QtCore/QLibrary>


#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <xinput.h>

QT_BEGIN_NAMESPACE

class WindowsJoystickInput : public QJoystickInput
{
    Q_OBJECT
public:
    WindowsJoystickInput();
    ~WindowsJoystickInput();

    void probeJoypads();
    void processJoypads();

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    enum {
        JOYPADS_MAX = 16,
        JOY_AXIS_COUNT = 6,
        MIN_JOY_AXIS = 10,
        MAX_JOY_AXIS = 32768,
        MAX_JOY_BUTTONS = 128,
        KEY_EVENT_BUFFER_SIZE = 512,
        MAX_TRIGGER = 255
    };

    struct dinput_gamepad {
        int id;
        bool attached;
        bool confirmed;
        bool last_buttons[MAX_JOY_BUTTONS];
        DWORD last_pad;

        LPDIRECTINPUTDEVICE8 di_joy;
        QList<LONG> joy_axis;
        GUID guid;

        dinput_gamepad() {
            id = -1;
            last_pad = -1;
            attached = false;
            confirmed = false;
            di_joy = nullptr;
            guid = {};

            for (int i = 0; i < MAX_JOY_BUTTONS; i++) {
                last_buttons[i] = false;
            }
        }
    };

    struct xinput_gamepad {
        int id = 0;
        bool attached = false;
        bool vibrating = false;
        DWORD last_packet = 0;
        XINPUT_STATE state;
        uint64_t ff_timestamp = 0;
        uint64_t ff_end_timestamp = 0;
    };

    typedef DWORD(WINAPI *XInputGetState_t)(DWORD dwUserIndex, XINPUT_STATE *pState);
    typedef DWORD(WINAPI *XInputSetState_t)(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration);

    HWND hWnd = nullptr;
    LPDIRECTINPUT8 dinput = nullptr;

    int id_to_change;
    int slider_count;
    int joypad_count;
    bool attached_joypads[JOYPADS_MAX];
    dinput_gamepad d_joypads[JOYPADS_MAX];
    xinput_gamepad x_joypads[XUSER_MAX_COUNT];

    static BOOL CALLBACK enumCallback(const DIDEVICEINSTANCE *p_instance, void *p_context);
    static BOOL CALLBACK objectsCallback(const DIDEVICEOBJECTINSTANCE *instance, void *context);

    void setup_joypad_object(const DIDEVICEOBJECTINSTANCE *ob, int p_joy_id);
    void close_joypad(int id = -1);
    void load_xinput();
    void unload_xinput();

    void post_hat(int p_device, DWORD p_dpad);

    bool have_device(const GUID &p_guid);
    bool is_xinput_device(const GUID *p_guid);
    bool setup_dinput_joypad(const DIDEVICEINSTANCE *instance);
    void joypad_vibration_start_xinput(int p_device, float p_weak_magnitude, float p_strong_magnitude, float p_duration, uint64_t p_timestamp);
    void joypad_vibration_stop_xinput(int p_device, uint64_t p_timestamp);

    float axis_correct(int p_val, bool p_xinput = false, bool p_trigger = false, bool p_negate = false) const;

    QLibrary xinput_dll;
    XInputGetState_t xinput_get_state = nullptr;
    XInputSetState_t xinput_set_state = nullptr;
};

QT_END_NAMESPACE

#endif // WINDOWSJOYSTICKINPUT_H
