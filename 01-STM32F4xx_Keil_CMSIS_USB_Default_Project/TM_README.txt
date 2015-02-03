This is Keil uVision default project for STM32F4xx development board with clock set for USB.

In case of USB FS, you need 48MHz clock for that.
If you have 180MHz or 100MHz boards, then you can't get exactly 48MHz from PLL, so for this case, some 
boards have decreased system core clock.

It is designed to work with different boards: (Max clock / Set clock)
- STM32F401-Discovery (84MHz/84MHz)
- STM32F4-Discovery (168MHz/168MHz)
- STM32F411-Discovery (100MHz/96MHz)
- STM32F429-Discovery (180MHz/96MHz)
- Nucleo F411 with external 8MHz crystal (100MHz/96MHz)
- Nucleo F411 with internal 16MHz crystal (100MHz/96MHz)
- Nucleo F401 with external 8MHz crystal (84MHz/84MHz)
- Nucleo F401 with internal 16MHz crystal (84MHz/84MHz)

When you open project, first select your target from list above.
You can select this in the toolbar, right on the "Download" button.
Note: select Nucleo board with internal crystal only in case
when you don't have external clock connected.
In other case, external clock will be automatically used

PLL settings are set for each target separatelly,
if you need to change them, they are set in Keil project.
Open "Options for Target" -> "C/C++" -> "Defines" and edit them.
Be careful what you are doing!

If you are using my libs, then place it into TM folder.
Include paths are already set for CMSIS libs. They are in folder which is included in this project.
When you need those files (or any of them) just open project, on the left side right click and "Add to project" and find CMSIS files you need.

After your compile has finished, you will found .hex and everything else in "Target" folder.
