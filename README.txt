# What's this?
An port of the classical DOS game supaplex.　Now available for your graphics calculator!

![animated gameplay gif](/../../wiki/sDessens/supaplex/supaplex43.gif)

# Is this game finished?
No. There are still a few bugs and missing features. You can't die yet.
I stopped the development of this game once i realized just how bad the LCD on the TI-84+ is.

# Technical teatures
For something built with less than a year experience in programming, the feature set is quite nice:
levels compressed with lz77 + von huffmann encoding.
custom custom tilemap engine.
most of the features in the original game, such as AI, bombs and physics, work.
The only thing i've copied from someone else is a routine called fastcopy.

The physics are not completely identical to the original game, my version only accepts key input every 8th frame.

# controls
2nd to start a level
arrow keys move your character
alpha hides the menu
hold 2nd to lay down a bomb (you need to find one first)
hold 2nd and press arrow key to remove blocks next to your character without moving.
clear to return to menu, clear again to quit.

# custiom levels
drag your uncompressed file.z80 to "compress.exe", this will compress the level and dump it into ..\compressed\
then add the appropriate information into "table.z80" and "levels.z80".
additionally, if you name your custom level XXX.z80, where XXX is a number below 111, you can just modify "levelset.z80".
the level names are in "data.z80"

what are levels.bat and table.bat?
these recreate levels.z80 and table.z80, if you entered your custom levels in here: don't run these

Initial version released 11-04-2010
