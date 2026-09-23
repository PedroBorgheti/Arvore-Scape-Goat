@echo off
gcc -Wall main.c ScapegoatTree.c -o main.exe -lm
if %errorlevel% equ 0 (
    main.exe
)