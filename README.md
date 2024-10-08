# Metal Gear Rising: Revengeance RedTrainer
The trainer was based on [MGR Plugin SDK](https://github.com/Frouk3/mgr-plugin-sdk) (Exceptional thanks to [Frouk3](https://github.com/Frouk3)), [ImGUI](https://github.com/ocornut/imgui), [libmem](https://github.com/rdbo/libmem) 2.14, and [DX9-BaseHook](https://github.com/rdbo/DX9-BaseHook). Thanks to [Mr.Beacon00](https://www.youtube.com/@MrBeacon00) for his table.

## Features
### Stats
- Change health
- Change money
- Invincibility
- Infinite energy
### Items
- Infinite additional weapons
- Shop items changer
### Customization
- Change character
- Change body
- Change weapon
- Change wig
### Movement
- Change game speed
- Fly mode
### Mission
- Change mission
- No damage
- No killed and no alert (now doesnt works)
- Change mission timer
- Change mission battlepoints
- Change max combo
- Change kills and zandzutsu
### Other
- Camera filter
### Enemy
- Enemy changer
## Hotkeys
- Show/hide menu (Num+)
- Show/hide additional menus (Num-)
- Keyboard control (Cursor keys)
- Deatach trainer (End)
- Activate FlyMode (Num5)
- FlyMode forward (Num8)
- FlyMode back (Num2)
- FlyMode left (Num4)
- FlyMode right (Num6)
- FlyMode up (Num7)
- FlyMode down (Num1)

![alt text](https://github.com/Baromir19/MGR-RedTrainer/blob/master/readme/hotkeys.png)

## Installation
Download Compiled.zip from releases, after you need to open the archive and unpack it into a folder with Metal Gear Rising
You must also have dinput8.dll (last archive), as well as [Visual C ++ Redistributable(x86) 2015-2022](https://aka.ms/vs/17/release/vc_redist.x86.exe)

![alt text](https://github.com/Baromir19/MGR-RedTrainer/blob/master/readme/Installation.png)

## Screenshots
![alt text](https://github.com/Baromir19/MGR-RedTrainer/blob/master/readme/Screen1.png)
![alt text](https://github.com/Baromir19/MGR-RedTrainer/blob/master/readme/Screen2.png)
![alt text](https://github.com/Baromir19/MGR-RedTrainer/blob/master/readme/Screen3.png)
![alt text](https://github.com/Baromir19/MGR-RedTrainer/blob/master/readme/Screen4.png)

## How to use
### Main info
To use the functions, you need to click on the corresponding button. Sometimes you should select the required function from the list or manually enter it into the text field. In any case, you need to click on the corresponding button.
### Mission changer
To use the function for changing the mission, you need to enter the textual name of the subphase (on the left) and its phase ID (on the right, numeric). This information can be found in the readme folder, in the PhaseInfo.xml file.
### Enemy changer
The function changes all (!) enemies. To use it, you need to enter its ID in the enemy ID field. In the Type field, you should enter the subtype of the enemy (for example, type 0 for Tripod is Tripod with a rifle, while type 1 is Tripod without a weapon). SubType is probably not used (it may be removed in the future). Flag sets the flag of the enemies (there are flags that stop enemies, disable their ability to attack, or, for example, drop the officer's hand, everything is individual, not all enemies have a flag). It is also worth noting that the ID may start with the following digits: Pl-models (1), Em-models (2, set by default), Wp-models (3), Et-models (4), It-models (7), Ba-models (d), Bh-models (e). Bm models were not found. The dropped weapons of It-models cannot be picked up, special flags are likely used for this. It is impossible to spawn a character using this function, other functions are used for that (when trying to spawn a character, the game crashes). [Description](https://web.archive.org/web/20160608040641/http://z13.invisionfree.com/MetalGearRising_Mods/index.php?showtopic=164) of most models of characters and things
### Change resolution
To change the window resolution, you need to deatach the trainer (press End), change the window resolution in the settings, save the settings, and re-enter the game.
