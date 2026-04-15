@echo off
"C:/raylib/w64devkit/bin/g++.exe" -O1 -s -std=c++14 src/main.cpp src/player.cpp src/bullet.cpp src/enemy.cpp src/screen.cpp src/map.cpp -o main.exe -I"C:/raylib/raylib/src" -I"C:/raylib/raylib/src/external" -I. -L"C:/raylib/raylib/src" -lraylib -lopengl32 -lgdi32 -lwinmm -DPLATFORM_DESKTOP -D_DEFAULT_SOURCE -Wno-missing-braces
if %errorlevel% == 0 (
    echo Build successful!
    .\main.exe
) else (
    echo Build failed!
)
pause
