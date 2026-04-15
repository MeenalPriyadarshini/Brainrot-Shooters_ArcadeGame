# New Task: Fix game issues

**Information Gathered:**
- Gun misalignment: DrawPlayer uses screen mouse, but shooting/aim uses world mouse from GetScreenToWorld2D
- Bullets despawn: src/bullet.cpp removes bullets outside bounds (-1000..5000). Make infinite.
- Game over on wave clear: Logic in main.cpp seems correct (allDead sets GAMEOVER), but may not trigger properly - investigate.

**Plan:**
1. src/main.cpp: Pass world mouse to DrawPlayer(player, mouse) instead of screen mouse.
2. src/bullet.cpp: Remove position bounds from erase_if - only deactivate on collision.
3. Verify allDead logic (add debug print if needed). Ensure GameState in screen.h matches.

**Dependent Files:**
- None.

**Followup steps:**
1. Rebuild: mingw32-make clean && mingw32-make main
2. Test game for alignment, infinite bullets, wave end GAMEOVER.
3. .\main.exe

Confirm plan?
