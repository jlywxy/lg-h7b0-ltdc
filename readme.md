# STM32H7B0 LTDC Display Controller for LG LD050WV1
this project intended to drive LG LD050WV1-SP01 LCD display using STM32H7B0VBT6 LTDC.

## what will it do
display a "daikin pichonkun"(ダイキン　ぴちょんくん💧) image on the display screen. (defined in `Core/Src/lgb-logo-rbgdata.h`)<br/>
this project may perform a external display hardware for macOS in the future.

## LG LD050WV1 screen configuration (lighting up) in brief
### steps
1. "Power On Set"
2. "Display Initial Set"
3. "Sleep Out And Display On Set"
4. BackLight On
5. LTDC Signal (this is can be async from step 1-4)

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
this configuration have been tested on two pieces of LD050WV1-SP01 screen produced in 2011 and configurations above are valid.<br/>

finally, LD050WV1-SP01 specification is easy to find at google.

## how to compile and upload
the code have been compiled by arm-none-eabi-gcc on macOS, uploaded via STLink v2 and debugged by openocd on a CLion development environment. the board config file (.cfg) indicated the upload method.

## bugs have been confirmed
1. display initialization fails some times, considering circuit contact problem or SPI clock too fast.
2. screen not displaying from LTDC layer position(0,0), haven't find the solution.

this document is needed to be completed.