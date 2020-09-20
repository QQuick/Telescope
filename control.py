# ====== Legal notices
#
# Copyright (C) 2013 - 2020 GEATEC engineering
#
# This program is free software.
# You can use, redistribute and/or modify it, but only under the terms stated in the QQuickLicence.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the QQuickLicence for details.
#
# The QQuickLicense can be accessed at: http://www.qquick.org/license.html
#
# __________________________________________________________________________
#
#
#  THIS PROGRAM IS FUNDAMENTALLY UNSUITABLE FOR CONTROLLING REAL SYSTEMS !!
#
# __________________________________________________________________________
#
# It is meant for training purposes only.
#
# Removing this header ends your licence.
#

# Stepper motors type: PFC2H-48081
# 48 pulses per rotor revolution, gear 1/120, 18 Ohm, 9G2A
# (360 / 120) / 48 = 1/16 degree per pulse
#
# Mount type: EQ5
# Worm gear: 1/144
# So 1/16 * 1/144 = 1/2304 degree per pulse
#
# Original DUO (DK3) control, according to astroeq.co.uk/tutorials.php?link=ratios
# 1/4 or 1/2 microstepping
#
# To counteract earth rotation we need (360 / 3600) / 24 = 1/240 degrees per second
# This requires 2304 / 240 = 9.6 pulses per second
#

# Microstepping not yet implemented in this code

from SimPyLC import *

class Control (Module):
    def __init__ (self):
        Module.__init__ (self)  
        
        self.page ('Telescope control')
        
        self.group ('Inputs', True)

        self.downButton = Marker ()
        self.upButton = Marker ()
        self.declActualPos = Register ()

        self.leftButton = Marker ()
        self.rightButton = Marker ()
        self.raActualPos = Register ()
                
        self.group ('Outputs')
        self.led = Marker ()

        self.declSpeed = Register ()
        self.declTargetPos = Register ()

        self.raSpeed = Register ()
        self.raTargetPos = Register ()
        
        self.group ('System')
        self.runner = Runner ()
        
        self.group ('Toggles')
        
        self.followToggleCombi = Marker ()
        self.followToggle = Oneshot ()
        self.follow = Marker ()

        self.ledToggleCombi = Marker ()
        self.ledToggle = Oneshot ()
        self.ledFull = Marker ()
        self.ledCount = Register ()

        self.southToggleCombi = Marker ()
        self.southToggle = Oneshot ()
        self.south = Marker ()

        self.group ('Motions', True)

        self.declDownCombi = Marker ()
        self.declUpCombi = Marker ()
        self.declGoalDist = Register ()

        self.raLeftCombi = Marker ()
        self.raRightCombi = Marker ()
        self.raGoalDist = Register ()
        self.raAnyCombi = Marker ()
        self.raBaseSpeed = Register ()

        self.group ('Constants')

        self.goalDist = Register (1000)
        self.followSpeed = Register (9.6)
        
        self.minSpeed = Register (1)
        self.maxSpeed = Register (200)
            
    def sweep (self):
        self.followToggleCombi.mark (not self.downButton and not self.upButton and self.leftButton and self.rightButton)
        self.ledToggleCombi.mark (self.downButton and self.upButton and not self.leftButton and not self.rightButton)
        self.southToggleCombi.mark (self.downButton and self.upButton and self.leftButton and self.rightButton)
        
        self.declDownCombi.mark (self.downButton and not self.upButton)
        self.declUpCombi.mark (not self.downButton and self.upButton)

        self.raLeftCombi.mark (self.leftButton and not self.rightButton)
        self.raRightCombi.mark (not self.leftButton and self.rightButton)
        self.raAnyCombi.mark (self.raLeftCombi or self.raRightCombi)
        
        self.followToggle.trigger (self.followToggleCombi)
        self.follow.mark (not self.follow, self.followToggle)

        self.ledToggle.trigger (self.ledToggleCombi)
        self.ledFull.mark (not self.ledFull, self.ledToggle)
        self.ledCount.set ((self.ledCount + 1) % 8192, self.follow, -1)
        self.led.mark (self.ledFull or self.ledCount == 0)

        self.southToggle.trigger (self.southToggleCombi)
        self.south.mark (not self.south, self.southToggle)

        self.declSpeed.set (self.maxSpeed, self.declDownCombi or self.declUpCombi, 0)
        self.declGoalDist.set (-self.goalDist, self.declDownCombi, self.goalDist)
        self.declGoalDist.set (0, self.declSpeed < self.minSpeed)
        
        self.raBaseSpeed.set (self.followSpeed, self.follow, 0)
        self.raSpeed.set (self.maxSpeed, self.raLeftCombi or self.raRightCombi, self.raBaseSpeed)
        self.raGoalDist.set (-self.goalDist, self.raLeftCombi or (not self.raAnyCombi and self.south), self.goalDist)
        self.raGoalDist.set (0, self.raSpeed < self.minSpeed)
        
