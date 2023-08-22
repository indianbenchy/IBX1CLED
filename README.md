Bambu Lab's X1C LED Mod Code
This is a modified version of the BambulabLedController developed by DutchDevelopers (https://github.com/DutchDevelop/BLLEDController)

I have modified it slitely to work with my own PCB design and included a DNS name and master pass so that you can access the configuration page even if the ip changes of the mod and you don't have to take it out of the PCB to reconfigure it.

I will share a video link of how to build your own PCB to use this.

It will be connected to your X1c via MQTT

Use a RGBCCT light strip

- The warm and cold light with light up the printer while printing.
- It will turn to RED when any error occurs
- It will turn to Green when print is finished.
- The light can be turned on and off with printer light option
- The light will be turned off when Lidar is on so that it doesn't interfare.

License
This is a modified code with license applied by its original owner under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license.

This license ("License") governs the use, distribution, and modification of the Bambulanb Controller's code, firmware, forks, and firmware of forks ("Material"). By using, distributing, or modifying the Material, you agree to abide by the terms and conditions of this License.

You are free to:

- Share: Copy and redistribute the Material in any medium or format.
- Adapt: Remix, transform, and build upon the Material.

Under the following terms:

- Attribution: You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
- Non-commercial: You may not use the Material for commercial purposes.
- Device and DIY Non-Commercial Restriction: The Material may only be used for the "bambulanb controller" device or for non-commercial DIY projects.
- Share-alike: If you remix, transform, or build upon the Material, you must distribute your contributions under the same license as the original.

No additional restrictions: You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.

For the full text of the CC BY-NC-SA 4.0 license, please visit: https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

This License is based on the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. The terms of that license apply to the extent they do not conflict with the terms of this License.