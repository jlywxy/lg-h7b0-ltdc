# STM32H7B0 LTDC Display Controller for LG LD050WV1
this project intended to drive LG LD050WV1-SP01 LCD display using STM32H7B0VBT6 LTDC.

## what will it do
display "daikin pichonkun"(ダイキン　ぴちょんくん💧) image and a rotating circle with fps info displaying on the screen. <br/>
the "pichonkun" image is defined in `Core/Src/lgb-logo-rbgdata.h`![](demo3.png)<br/>

## improvements
220528:<br/>
1. changed LTDC freq to 29MHz to get stability.
2. changed buffer type to from ARGB888 to RGB888, making display almost in full screen(480*720) within internal sram (cost 99.5%).
3. imporved demo algos by creating sine and cosine LUT to improve performance. now rotating circle is bigger and fps promoted.

## LG LD050WV1 screen configuration (lighting up) in brief
### steps
1. "Power On Set"
2. "Display Initial Set"
3. "Sleep Out And Display On Set"
4. BackLight On (this can be async, backlight module circuit is separate from lcd module circuit.)
5. LTDC signal start transfer(this CANNOT be async, LTDC have to be inited after step 1-3).
to do this, move `MX_LTDC_Init();` to where step 3 code done, after cubemx regenerated code.

for step 1-3, read the LD050WV1-SP01 Specification PDF.<br/>
"Display Initial Set" and "Sleep Out And Display On Set" means stm32 should send SPI data to the screen to initialize it, which was described in the specification datasheet.
### important electrical interface specification (conf in cubemx)
1. LD050WV1 requires LTDC `Pixel Clock Polarity` Inverted Input
2. LD050WV1 requires SPI Hardware NSS Signal, `NSSP Mode` Disabled, `CPOL` High, `CPHA` 2 Edge.

hint 1-2 are based on specification.

3. LD050WV1 DON'T need a DE input, it use HSYNC and VSYNC only. leave the LTDC_DE output pin on stm32 ALONE.
4. screen BackLight voltage is 16v(Typical), it's very necessary to be careful with physical interface connection(LCD driver ic only need 2.9v(Typical) for VCC). when using a `Board To Board` FPC Connector to pin header converting board, make sure pin header sequence is matched with FPC Connector, otherwise the screen driver module may be damaged permanently. <br/>

the FPC Connector for converting board "mating" to the screen FPC connector is Panasonic AXT640124 or equivalent(easy to buy at Taobao), based on the specification(specification says AX640124 without "T").<br/>
for other interface configuraton, check .ioc file by STM32CubeMX and screen specification book.<br/>

5. LTDC Synchoronization parameters need to be set manually in code, because cubemx generates WRONG parameters.<br/>
set manually in `Core/Src/ltdc.c`:<br/>
```c
  //for LD050WV1-SP01 only:
  hltdc.Init.HorizontalSync = 48;
  hltdc.Init.VerticalSync = 10;
  hltdc.Init.AccumulatedHBP = 72;
  hltdc.Init.AccumulatedVBP = 18;
  hltdc.Init.AccumulatedActiveW = 624-70;
  hltdc.Init.AccumulatedActiveH = 831-10;
  hltdc.Init.TotalWidth = 624;
  hltdc.Init.TotalHeigh = 831;

```
(data above are based on lcd specification and experimentation, which can make screen display in full-screen and improve timimg performance.)<br/>
code have to be modified manually every time after cubemx generate code.

6. LTDC Overclock is viable, the fastest LTDC clock tested is 54.6 MHz, while specification says 36MHz max. under this freq, touching the wiring may cause screen temporarily stop working (color over saturation, sudden black, image sync problem, etc) because of the electromagnetic interference.
7. providing higher voltage to module (3.1v measured at stm32 VCC and going directly to LCM without resistor) is also viable, which can improve screen color sensory performance: saturation promoted(however this screen color gamut have only 49% NTSC). since ABS(absolute max ratings) in spec says LCM VCC 4.5v max, it's a bit confusing why LCM VCC(Typical) is 2.9v and Max is 3.0v.

this configuration have been tested on two pieces of LD050WV1-SP01 screen produced in 2011 and configurations above are valid.<br/>

finally, LD050WV1-SP01 specification is easy to find at google.

## how to compile and upload
the code have been compiled by arm-none-eabi-gcc on macOS, uploaded via STLink v2 and debugged by openocd on a CLion development environment. the board config file (.cfg) indicated the upload method.

## performance testing
when refreshing LTDC Layer0 with single color shifting by hue(370*670 area), refresh rate can up to 101fps, with Cortex M7 ICache and DCache enabled.
![](fps.png)

## bugs have been confirmed
1. when Cortex M7 DCache is enabled, refreshing screen may cause "tearing", considering solving it using buffer/gfxmmu/ltdc line interrupt.

## bugs have been fixed
~~1. display initialization fails some times, considering circuit contact problem or SPI clock too fast.~~<br/>
LTDC initialization must be started AFTER LCD SPI init. after putting `MX_LTDC_Init();` to the after of `LCD_Sleep_Out_N_Display_On_Set();`, every time screen init successes.

~~2. screen not displaying from LTDC layer position(0,0), haven't find the solution.~~<br/>
see `important electrical interface specification` 5.
