//  SuperTuxKart - a fun racing game with go-kart
//
//  Copyright (C) 2009-2013 Marianne Gagnon
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef INPUT_DEVICE_HPP
#define INPUT_DEVICE_HPP

#include <string>

#include "config/device_config.hpp"
#include "input/input.hpp"
#include "input/input_manager.hpp"
#include "states_screens/state_manager.hpp"
#include "utils/no_copy.hpp"

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/can.h>

/**
  * \brief Input device type
  * \ingroup input
  */
enum DeviceType
{
    DT_KEYBOARD,
    DT_GAMEPAD,
    DT_CANBUS
};

/**
  * \brief base class for input devices
  * \ingroup input
  */
class InputDevice: public NoCopy
{
    friend class DeviceManager;
protected:
    DeviceType m_type;
    StateManager::ActivePlayer* m_player;
    DeviceConfig* m_configuration;

public:
     /** If device has a name; unused for keyboards since AFAIK we
      *  can't tell keyboards apart. */
    std::string m_name;

    InputDevice();
    void setConfiguration(DeviceConfig *config) {m_configuration = config;}
    DeviceConfig *getConfiguration() {return m_configuration;}

    DeviceType getType() const { return m_type; };

    void setPlayer(StateManager::ActivePlayer* owner);
    StateManager::ActivePlayer *getPlayer() { return m_player; }
};

/**
  * \brief specialisation of InputDevice for canbus type devices
  * \ingroup input
  */
class CanbusDevice : public InputDevice
{
public:


    CanbusDevice();
    CanbusDevice(CanbusConfig *configuration);

    int            canfd;
    struct can_frame frame;
    struct sockaddr_can caddr;
    struct ifreq ifr;
    socklen_t caddrlen;


    /**
     * Checks if this key belongs to this device. if yes, sets action and
     *  returns true; otherwise returns false
     *
     * \param      id      ID of the key that was pressed
     * \param      mode    Used to determine whether to bind menu actions or
     *                     game actions
     * \param[out] action  The action associated to this input (only check
     *                     this value if method returned true)
     */
    bool processAndMapInput(const int id, InputManager::InputDriverMode mode,
                            PlayerAction* action);

};

/**
  * \brief specialisation of InputDevice for keyboard type devices
  * \ingroup input
  */
class KeyboardDevice : public InputDevice
{
public:


    KeyboardDevice();
    KeyboardDevice(KeyboardConfig *configuration);

    /**
     * Checks if this key belongs to this device. if yes, sets action and
     *  returns true; otherwise returns false
     *
     * \param      id      ID of the key that was pressed
     * \param      mode    Used to determine whether to bind menu actions or
     *                     game actions
     * \param[out] action  The action associated to this input (only check
     *                     this value if method returned true)
     */
    bool processAndMapInput(const int id, InputManager::InputDriverMode mode,
                            PlayerAction* action);

};

/**
  * \brief specialisation of Inputdevice for gamepad type devices
  * \ingroup input
  */
class GamePadDevice : public InputDevice
{
    void resetAxisDirection(const int axis, Input::AxisDirection direction,
                            StateManager::ActivePlayer* player);
    bool m_buttonPressed[SEvent::SJoystickEvent::NUMBER_OF_BUTTONS];

public:
    Input::AxisDirection *m_prevAxisDirections;

    /** used to determine if an axis is valid; an axis is considered valid
      * when at least 2 different values are read from this axis (if an axis
      * keeps on sending the exact same value continuously, chances are that
      * it's not meant by the user - for instance some gamepads have hats or
      * analog switches that get reported as axis, we even had a report that
      * on linux some hard disks may be reported as gamepads with
      * uninteresting axis values)
      */
    int                  *m_prevAxisValue;
    /** \see m_prevAxisValue */
    bool                 *m_axis_ok;

    int                   m_deadzone;
    int                   m_index;
    int                   m_axis_count;
    int                   m_button_count;

    /** Constructor for GamePadDevice from a connected gamepad for which no
      * configuration existed (defaults will be used)
      *  \param irrIndex Index of stick as given by irrLicht.
      */
    GamePadDevice(const int irrIndex, const std::string name,
                  const int axis_number,
                  const int btnAmount, GamepadConfig *configuration);
    ~GamePadDevice();

    bool isButtonPressed(const int i);
    void setButtonPressed(const int i, bool isButtonPressed);

    /**
     * Invoked when this device it used. Verifies if the key/button that
     * was pressed is associated with a binding. If yes, sets action and
     * returns true; otherwise returns false.
     *
     * \param      player  Only passed to know where to send 'axis reset's
     *                     when necessary
     * \param      id      ID of the key that was pressed or of the axis
     *                     that was triggered (depending on
     *                     the value of the 'type' parameter)
     * \param      mode    Used to determine whether to map menu actions or
     *                     game actions
     * \param[out] action  The action associated to this input (only check
     *                     this value if method returned true)
     *
     * \return Whether the pressed key/button is bound with an action
     */
    bool processAndMapInput(Input::InputType type, const int id,
                            int* value,
                            InputManager::InputDriverMode mode,
                            StateManager::ActivePlayer* player,
                            PlayerAction* action);

};


#endif
