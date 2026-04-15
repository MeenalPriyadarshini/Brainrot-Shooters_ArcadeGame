# Feedback Fixes: Smooth wave end + GAMEOVER screen

## Steps:
1. ✅ Plan approved (2s delay + fix GAMEOVER overlay while keeping game background)
2. Add clearedWave flag + waveClearTimer = 2.0f
3. Modify check to set flag/timer
4. Add timer countdown in update
5. Modify draw for GAMEOVER: draw game world/UI first, then DrawGameOverScreen on top (no overlap issue - already structured, perhaps fade?)
6. Reset flag on restart
7. Build/test
8. Complete

✅ Implemented delay + vars/reset.\nNext: GAMEOVER draw fix (if overlap persists, DrawGameOverScreen likely draws UI on top).\nRun .\build.bat + test.
