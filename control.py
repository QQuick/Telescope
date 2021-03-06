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

import SimPyLC as sp

class Control (sp.Module):
    def __init__ (self):
        sp.Module.__init__ (self)  
        
        self.page ('Telescope control')
        
        self.group ('Inputs', True)

        self.downButton = sp.Marker ()
        self.upButton = sp.Marker ()
        self.declActualPos = sp.Register ()

        self.leftButton = sp.Marker ()
        self.rightButton = sp.Marker ()
        self.raActualPos = sp.Register ()
                
        self.group ('Outputs')
        self.led = sp.Marker ()

        self.declSpeed = sp.Register ()
        self.declTargetPos = sp.Register ()

        self.raSpeed = sp.Register ()
        self.raTargetPos = sp.Register ()
        
        self.group ('System')
        self.runner = sp.Runner ()
        
        self.group ('Toggles')
        
        self.followToggleCombi = sp.Marker ()
        self.followToggle = sp.Oneshot ()
        self.follow = sp.Marker ()

        self.ledToggleCombi = sp.Marker ()
        self.ledToggle = sp.Oneshot ()
        self.ledFull = sp.Marker ()
        self.ledCount = sp.Register ()

        self.southToggleCombi = sp.Marker ()
        self.southToggle = sp.Oneshot ()
        self.south = sp.Marker ()

        self.group ('Motions', True)

        self.declDownCombi = sp.Marker ()
        self.declUpCombi = sp.Marker ()
        self.declGoalDist = sp.Register ()

        self.raLeftCombi = sp.Marker ()
        self.raRightCombi = sp.Marker ()
        self.raGoalDist = sp.Register ()
        self.raAnyCombi = sp.Marker ()
        self.raBaseSpeed = sp.Register ()

        self.group ('Constants')

        self.goalDist = sp.Register (1000)
        self.followSpeed = sp.Register (9.6)
        
        self.minSpeed = sp.Register (1)
        self.maxSpeed = sp.Register (200)
            
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
        
