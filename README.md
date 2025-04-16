# ToggleStacker
v1.1.0

Allows you to toggle the ability to stack any part (yes including mouths) on each other through a console command. It's basically SporeStacker but better, works in all editors (except the cell and adventure editors).

Simply type `ToggleStacker` into the console (Shift + Ctrl + C) to toggle the ability to stack any part on each other (most notable in the Creature editors).

## Modes Option

As of v1.1.0, ToggleStacker has modes, to switch to a specific mode run the command:
`ToggleStacker -mode MODENAME`

Here's the modes you can chose from:
- all / stack / yes
All parts will stack on each other.
Most notable in the creature editor.
- none / unstack / no
No parts will stack on each other (except for limbs, spines, and other base parts), even vanilla stacking. 
Most notable in any outfitter.
- default / deactivate / off / false / 
Parts will follow their vanilla/default behavior.

## Custom Option

As of v1.1.0, ToggleStacker has the ability to modify the stack mode for specific parts. This controls how the part interacts with other parts, not how parts will interact with it.
To use this, select or hover over a part and run the command `ToggleStacker -custom add`. The target part will now always follow the rules of your current stack mode.
You can even combine this with the previous option to quickly change the property of the target part.

It's worth nothing that the custom list is cleared upon leaving the editor.

The custom option actually has a few arguments:
- add / include
Adds the current part to the custom list, retaining its current properties.
- remove / delete / erase
Removes the part from the custom list, returning it to the current mode's properties.
- reset / clear
Clears the custom list, making all parts follow the current mode.
