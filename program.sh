sudo openocd -f /usr/share/openocd/scripts/interface/stlink-v2.cfg \
             -f /usr/share/openocd/scripts/target/stm32f0x.cfg \
             -c "program build/speedctrl.elf verify reset exit"