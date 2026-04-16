# Brainrot Shooter Wave 1 Implementation TODO

## Plan Summary
**Information Gathered:**
- enemy.h: Basic Enemy struct, old 3 function decls.
- enemy.cpp: Old SpawnEnemies(small/big), simple UpdateEnemies left-move+collide, Draw circles. No types/lanes/boss.
- main.cpp: Old spawn in MENU, UpdateEnemies(player.pos, hp) [needs camX], clear→GAMEOVER. No boss logic. Uses AreAllEnemiesGone (needs removal? but not conflicting).

**Files to Edit:**
- include/enemy.h
- src/enemy.cpp (full replace)
- src/main.cpp

**Followup:** Run `build.bat` to compile/test.

## Steps (check off as completed):
- [x] Step 1: Update include/enemy.h (enum + struct + new decls)
- [x] Step 2: Replace src/enemy.cpp with new Wave1 version
- [x] Step 3: Update src/main.cpp (vars + spawn + UpdateEnemies args + wave logic)
- [x] Step 4: Compile with build.bat and test wave progression (spawn→etype1→boss→etype2→gameover)

Progress will be updated after each step.

