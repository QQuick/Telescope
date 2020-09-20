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

import SimPyLC as sp

class Timing (sp.Chart):
    def __init__ (self):
        sp.Chart.__init__ (self)
        
    def define (self):
        ct = sp.world.control
        
        self.channel (ct.led, sp.red)
        self.channel (ct.follow, sp.green)
        self.channel (ct.south,sp.blue)

        self.channel (ct.downButton, sp.yellow)
        self.channel (ct.upButton, sp.yellow)
        self.channel (ct.declGoalDist, sp.yellow, -ct.goalDist, ct.goalDist, 50)
        self.channel (ct.declSpeed, sp.yellow, 0, ct.maxSpeed, 100)

        self.channel (ct.leftButton, sp.white)
        self.channel (ct.rightButton, sp.white)
        self.channel (ct.raGoalDist, sp.white, -ct.goalDist, ct.goalDist, 50)
        self.channel (ct.raSpeed, sp.white, 0, ct.maxSpeed, 100)
        
        
