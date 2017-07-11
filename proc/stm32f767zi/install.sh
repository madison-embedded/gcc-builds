#!/bin/bash

# ST-Link V2
if [ -d "/media/$USERNAME/NODE_F767ZI" ]; then
	cp badgerloop.bin /media/$USERNAME/NODE_F767ZI
	echo "Copying badgerloop.bin to /media/$USERNAME/NODE_F767ZI"
# J-Link
else
	JLinkExe -device STM32F767ZI -if SWD -speed 4000 -autoconnect 1 -CommanderScript ./proc/stm32f767zi/flash.jlink
fi

