# ====== Legal notices
#
# Copyright (C) 2013 - 2018 GEATEC engineering
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
        
        self.group ('Internals', True)
        
        self.ledToggleCombi = Marker ()
        self.ledToggle = Oneshot ()

        self.hemiToggleCombi = Marker ()
        self.hemiToggle = Oneshot ()
        self.hemi = Marker ()

        self.declDownCombi = Marker ()
        self.declUpCombi = Marker ()

        self.raLeftCombi = Marker ()
        self.raRightCombi = Marker ()

        self.group ('Constants')

        self.finity = Register (100_000)
        self.minSpeed = Register (0.1)
        self.maxSpeed = Register (100)
            
    def sweep (self):
        self.ledToggleCombi.mark (self.downButton and self.upButton)
        self.hemiToggleCombi.mark (self.leftButton and self.rightButton)
        
        self.declDownCombi.mark (self.downButton and not self.upButton)
        self.declUpCombi.mark (not self.downButton and self.upButton)

        self.raLeftCombi.mark (self.leftButton and not self.rightButton)
        self.raRightCombi.mark (not self.leftButton and self.rightButton)
          
        self.ledToggle.trigger (self.ledToggleCombi)
        self.led.mark (not self.led, self.ledToggle)

        self.hemiToggle.trigger (self.hemiToggleCombi)
        self.hemi.mark (not self.hemi, self.hemiToggle)

        self.declSpeed.set (self.maxSpeed, self.declDownCombi or self.declUpCombi, 0)
        self.declTargetPos.set (-self.finity, self.declDownCombi, self.finity)
        self.declTargetPos.set (self.declActualPos, self.declSpeed < self.minSpeed)
        
        self.raSpeed.set (self.maxSpeed, self.raLeftCombi or self.raRightCombi, 0)
        self.raTargetPos.set (-self.finity, self.raLeftCombi, self.finity)
        self.raTargetPos.set (self.raActualPos, self.raSpeed < self.minSpeed)
        
