# 3d-chase
3d cube of led's as interface for simple game of snake without the body of the snake. Made by me for Hack Club Half Life

## Premise

3d led cube:

<img width="1024" height="576" alt="image" src="https://github.com/user-attachments/assets/d306eff3-7d95-4c40-acc8-74bf9b94216e" />

But it's snake:

<img width="647" height="637" alt="image" src="https://github.com/user-attachments/assets/f89a05fb-0613-4646-b4ce-3c3daa56e9a4" />

But the cube is 9x9x9 so i dont think the snake's body would work, so it's just going to be a led moving towards blinking leds.

## Stuff you need

[BOM.csv](https://github.com/NestOfGhosts/3d-chase/blob/main/BOM.csv) is just a copy of the things I personally had to buy for the project and their prices, not a complete list of things you need. The following table is the complete parts list.

|Thing you need|What you need it for|
|--------------|--------------------|
|Soldering equipment and expertise|A simple soldering iron and through-hole skills needed|
|[The Pcb](https://github.com/NestOfGhosts/3d-chase/tree/main/Pcb%20Files)|To hold everything together|
|[The Chassis](https://github.com/NestOfGhosts/3d-chase/tree/main/Chassis%20Files)|To hold everything together but in like a different way :3|
|Arduino Nano Atmega328 with usb c|Just kinda get some clone or something that seems legit enough and compatible/same pin layout as og nano. I got mine from [Aliexpress](https://www.aliexpress.com/item/1005006128051481.html?spm=a2g0o.productlist.0.0.1b334a261XQdtX&mp=1&pdp_npi=6%40dis%21CAD%21CAD+2.74%21CAD+2.74%21%21CAD+2.74%21%21%21%4021032e4e17933253312398755e0fb7%2112000035881712636%21ct%21CA%216330886401%21%211%210%21)|
|7 switches and keycaps|For the controls. Make sure they're mx compatible and 5 pin. MAKE SURE THEY'RE 5 PIN|
|27 Diode leds|The ones with only an anode and cathode, in any colour of your choosing. Im using flattop whites|
|3 Mosfets|Allows you to open and close electrical paths|
|9 ~130-330 ohm resistors|To limit electric flow going to leds. Higher number is more resistant = duller leds. Recommended ohm value is for white leds|
|3 4.7k-100k ohm resistors|Closes electrical path if mosfet isnt *fully* off|
|M4 screws and nuts|To hold the chassis together|
|Wires|I'm using some spare jumpers. Make sure you can connect points at ~20mm, ~40mm, ~60mm. (120mm of wires)|

### Additional notes: 
  - I got my pcb and chassis from JLCPCB and JLC3DP (respectively).
  - The pcb is fully designed for through hole parts
  - Flat topped, diffused leds work better for this projects, because realistically you're looking at the leds from multiple angles and you will be looking at them for periods of time (diffused spreads light out and reduces sharpness). However, diffused leds are expensive, so you can sand the led lightly with a high grit sandpaper (400-1000). Higher is less diffused.
  - Colour of leds correspond to resistor strengths. If you want a different led colour google "____ led resistor ohm"
  - The screws are ISO7380 M4. You may need to edit chassis screw holes if the screws you buy are bigger
  - The firmware is built for the old Atmega328 bootloader. If you want to change it you need to copy the firmware folder, open it in [VS Code](https://code.visualstudio.com), and download [Platformio](https://platformio.org/) (you can also find this in vscode's built in extension store). Then, in the platformio.ini folder change from REMEMBER TO CHANGE THIS
  - The resistor value is are for white leds. You can use [All About Circuits Led Resistor Calculators](https://www.allaboutcircuits.com/tools/led-resistor-calculator/) to calculate it. Aim for about roughly 10-20ma~? It really depends on every Led though so the value is a broad number. You can look at the listing for max ma, and maybe do a couple underneath that.

## Assembly
### Firmware
Instructions are windows
1. Download firmware .hex from [INSERT LINK]()
2. Download a .hex firmware flasher [Avrdude](https://blog.zakkemble.net/avrdudess-a-gui-for-avrdude/)
3. Open device manager, and expand the "Ports (COM & LPT)"
4. Plug in the nano. A new port should appear. Take note of the COM number (etcetcetc (COM___))
5. In Avrdude, set preset to "Arduino Nano (Atmega328p)". You may have purchased a Nano with a Atmega328 or Atmega328PB MCU. If so, change MCU to the correct one.
<img width="183" height="181" alt="image" src="https://github.com/user-attachments/assets/28b670cf-2430-4d30-9024-e5042e50c510" />

6. In Avrdude, set port to the COM number. Other programmer setting should be correct because of the preset.
<img width="443" height="122" alt="image" src="https://github.com/user-attachments/assets/4b8433a0-273b-47e7-8f7e-57ddd79a063f" />

7. Add the path to the .hex in the "Flash" field. Set it to "Write", and press "Go"
8. Wait until the terminal at the bottom of the gui says "Success" or smth

### Soldering
Hopefully you have a fume extractor, or have accepted asthma and metals in your lungs
1. Solder the Nano, MOSFETS, and resistors on the backside.
2. Solder switches on frontside.
#### The led cube
3. (Optional): Sand your leds if you more diffusion
4. Bend the cathodes(shorter side) of the leds 90 degrees, with a 1-3mm gap between the bottom of the led and the start of the bend
5. Lay out the leds in a 3x3 grid. Match it up with the hole distances in the pcb (all 2 leds are seperated by 15mm). Make a second bend on the horizontal plane to make each distance between leds 15mm.
<img width="831" height="467" alt="image" src="https://github.com/user-attachments/assets/4e0f1659-fb81-40f8-9564-adb84ef46df0" />

(PLACEHOLDER IMAGE)

6. Connect the anodes(longer side) up and down. Make the distance between the layers 15mm to make C U B E.
<img width="1024" height="576" alt="image" src="https://github.com/user-attachments/assets/ea398145-774e-4d0e-86a9-3dda38fc25e0" />

(PLACEHOLDER IMAGE)

7. Get the wires. Make sure both ends are stripped/open/metal. Now solder one end of the wire on one of the three holes to the left of the pcb (pins labeled L1, L2, L3). IMPORTANT: Wire the bottom most layer to L1, top layer to L3 etc
<img width="606" height="805" alt="image" src="https://github.com/user-attachments/assets/aee77e0f-61a4-4458-964e-1372808c91ea" />

8. Grab the chassis cover, and route the wires through the holes in the cover.
INSERT IMAGE
9. Solder the wires to the 3x3 layers, and solder the anode columns to the pcb
10. Put the chassis together 

## Arduino Pins:
D0 = Power button
D1, D2, D3, D4, D5, D6 = X+, X-, Y+, Y-, Z+, Z-
D7, D8, D9 = MOS1, MOS2, MOS3
D10, D11, D12, D13, D14, D15, D16, D17, D18 = Column 1, 2, 3, 4, 5, 6, 7, 8, 9


