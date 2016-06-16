Mr Paine -{
You are the only person to go to my github repo, pizza4life. You are probably here to try and code the RGB Glasses. Everything inside this curly bracket (inside the readme) is placed here by me to explain a bit of this code.

The master file is RGBShades-master.ino, and everything is necessary to have in your working directory. 

Layout:
The glasses are organized in a grid fashion, with most lcd displays, and they work on a coordinate grid (obviously starting with 0) A great display of this is on XYMap, on lines 32-40

Effects:
In the effects sketch, there are a lot of 'for' loops that basically just set the RGB's to a constantly changing hue. The function to change a color is essentially:
leds[XY(x, y)] = CRGB(Red,Green,Blue);
with Red being your red value, Green to the green value, and Blue as the blue value.
The lowercase x and y are your coordinate values, and the uppercase X and Y should remain with no change.
When you make an effect, it is a void function that you have to start with these lines:

  if (effectInit == false) {
    effectInit = true;
    effectDelay = 50;
  }
  
  you can then proceed to map whatever RGB's you want. Note that previous effects will remain in RGB's you do not change, so it is best to do a 'for' loop to wipe everything with 'leds[XY(x, y)] = CRGB(0,0,0);'
  
  Uploading:
  "This version has the standard Arduino bootloader. R9 and R10 near the control buttons will be present.
Select the “Arduino Pro or Pro Mini” option. Then, go back into the Tools menu and find the Processor option and select “ATmega328 (5V, 16MHz)”."

So the arduino pro or pro mini board, with atmega328 for processor. (under tools in the arduino ide)
}

Pull Requests - For new effects, please target the "experimental" branch. We'll try to throw everything in experimental, and bubble finished effects into the master branch.

RGBShades
=========

Firmware for RGB Shades (Arduino-compatible sunglasses with 16x5 matrix of WS2812B pixels)

Requires FastLED 2.1 or higher, available here: https://github.com/FastLED/FastLED

RGB Shades described here: https://www.kickstarter.com/projects/macetech/rgb-led-shades

When downloading ZIP file, remove "-master" at the end of the folder name before attempting
to open in the Arduino IDE

If your RGB Shades were purchased before July 2015:
This version has the standard Arduino bootloader. R9 and R10 near the control buttons will be present.
Select the “Arduino Pro or Pro Mini” option. Then, go back into the Tools menu and find the Processor option and select “ATmega328 (5V, 16MHz)”.

If your RGB Shades were purchased after July 2015:
This version has the Optiboot bootloader. R9 and 10 near the control buttons will be missing.
Select the “Arduino Mini” option. Then, go back into the Tools menu and find the Processor option and select “ATmega328”.
