#!/bin/bash

JLinkExe -device STM32F767ZI -if SWD -speed 4000 -autoconnect 1 -CommanderScript ./proc/stm32f767zi/flash.jlink

