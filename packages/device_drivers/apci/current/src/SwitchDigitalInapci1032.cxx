/***************************************************************************
  tag: Peter Soetens  Mon May 10 19:10:32 CEST 2004  SwitchDigitalInapci1032.cxx 

                        SwitchDigitalInapci1032.cxx -  description
                           -------------------
    begin                : Mon May 10 2004
    copyright            : (C) 2004 Peter Soetens
    email                : peter.soetens@mech.kuleuven.ac.be
 
 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/
#include <pkgconf/system.h>
#include <pkgconf/device_drivers_apci.h>
#include <device_drivers/SwitchDigitalInapci1032.hpp>
#include <os/fosi.h>

#if defined(OROINT_DEVICE_DRIVERS_APCI1032) && defined(OROPKG_OS_LXRT)
#include "apci_lxrt.h"
#elif defined (OROINT_DEVICE_DRIVERS_APCI1032_IK)
extern "C"
{
#define new _new
#define class _class
#define LINUX_PCI_H
#include "apci1032/apci1032.h"
#undef class
#undef new
}
#endif

namespace ORO_DeviceDriver
{

    SwitchDigitalInapci1032::SwitchDigitalInapci1032( const std::string& name ) :
          DigitalInInterface( name )
#ifdef OROCLS_DEVICE_DRIVERS_APCI1032_EVENTS
          ,ListenerRegistration( &inputEvent ) // fires when state changed
          ,TaskPreemptible(0.1)
          ,inputEvent( Event::SYNSYN )
#endif
    {
        BoardHandle = bh;
        rtos_printf( "BoardHandle is %i\n", bh );
        bh++;
#ifdef OROCLS_DEVICE_DRIVERS_APCI1032_EVENTS
        old_InputValue = 0;
        new_InputValue = 0;
#endif

        //SlotNumber=checkAndGetPCISlotNumber();
    }

    SwitchDigitalInapci1032::~SwitchDigitalInapci1032()
    {
        closeBoardHandle();
        bh--;
    }

    inline
    bool SwitchDigitalInapci1032::isOn( unsigned int bit) const
    {
#ifdef OROINT_DEVICE_DRIVERS_APCI1032
        if ( bit >= 0 && bit < 32 )
        {
            unsigned long pb_ChannelValue;
            i_APCI1032_Read1DigitalInput( BoardHandle, bit, &pb_ChannelValue );
            return pb_ChannelValue == 1;
        }
#endif
        return false;
    }

    bool SwitchDigitalInapci1032::isOff( unsigned int bit) const
    {
        return !isOn(bit);
    }


    bool SwitchDigitalInapci1032::readBit( unsigned int bit) const
    {
        return isOn(bit);
    }

    unsigned int SwitchDigitalInapci1032::readSequence(unsigned int start_bit, unsigned int stop_bit) const
    {
        unsigned long pul_InputValue=0;
#ifdef OROINT_DEVICE_DRIVERS_APCI1032
        i_APCI1032_Read32DigitalInput( BoardHandle, &pul_InputValue );
#endif
        return (pul_InputValue >> start_bit) & ( 1 << (stop_bit - start_bit + 1)) - 1;
    }

#ifdef OROCLS_DEVICE_DRIVERS_APCI1032_EVENTS
    unsigned long SwitchDigitalInapci1032::getNewValue()
    {
        return new_InputValue;
    }

    unsigned long SwitchDigitalInapci1032::getOldValue()
    {
        return old_InputValue;
    }
#endif

    int SwitchDigitalInapci1032::checkAndGetPCISlotNumber()
    {
        //BoardInformations = &s_APCI1032_DriverStructure.s_BoardInformations[ BoardHandle ];
        //return i_APCI1032_CheckAndGetPCISlotNumber( SlotNumberArray );
        return -1;
    }

    int SwitchDigitalInapci1032::setBoardInformation()
    {
        //return i_APCI1032_SetBoardInformation( SlotNumber, &BoardHandle );
        return -1;
    }

    int SwitchDigitalInapci1032::getHardwareInformation( unsigned int * pui_BaseAddress, unsigned char * pb_InterruptNbr, unsigned char * pb_SlotNumber )
    {
        //return i_APCI1032_GetHardwareInformation( BoardHandle, pui_BaseAddress, pb_InterruptNbr, pb_SlotNumber );
        return -1;
    }

    int SwitchDigitalInapci1032::closeBoardHandle()
    {
        //return i_APCI1032_CloseBoardHandle( BoardHandle );
        return -1;
    }

#ifdef OROCLS_DEVICE_DRIVERS_APCI1032_EVENTS
    void SwitchDigitalInapci1032::step( )
    {
#ifdef OROINT_DEVICE_DRIVERS_APCI1032
        i_APCI1032_Read32DigitalInput( BoardHandle, &new_InputValue );
#endif

        if ( new_InputValue != old_InputValue )
        {
            inputEvent.fire();
            old_InputValue = new_InputValue;
        }
    }
#endif

    int SwitchDigitalInapci1032::SwitchDigitalInapci1032::bh = 0;
};

