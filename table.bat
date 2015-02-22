@echo off
del table.z80
for /L %%I IN (1, 1, 111) DO (
echo #ifdef level_%%I >> table.z80
echo .db %%I >> table.z80
echo .dw label_%%I >> table.z80
echo #endif >> table.z80
echo. >> table.z80
)
echo .db 0 >> table.z80
pause