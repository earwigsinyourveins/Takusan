<img width="1600" height="1143" alt="banner4" src="https://github.com/user-attachments/assets/8b3e9ccd-7324-473f-a747-f38377d738f2" />


# Takusan is a fork of Shadow Warrior 1.2 with multiplayer-related QoL, bugfixes, and restored features.

Changes as of yet:
1. Automatic demo recording of multiplayer games, regardless if it was started from COMMIT/SETUP or in-game menu, including composure of .WB files for WangBang demo player.
2. Re-enabled fast forward/slo-mo demo controls, added demo pause and rewinding. Rewinding could be toggled off by setting DemoRewind to 0 under ;TAKUSAN SETTINGS in your SW.CFG
3. Re-enabled bots with uncommented better pathfinding, could be toggled on with -bots# argument. Example: sw -level23 -bots7
4. -aim# argument to toggle AutoAim on and off. This helps with multiplayer games started via -autonet.
5. Patched MACT386.LIB with Build Mouse Fix by CTPAX aka CHE@TER.

Builds with Watcom C/C++ 10.6. 

For the sake of transparency, this project extensively uses AI code.
