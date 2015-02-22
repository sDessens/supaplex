@echo off
echo. >> levels.z80
del levels.z80
for /L %%I IN (1, 1, 111) DO (
echo #ifdef level_%%I >> levels.z80
echo label_%%I >> levels.z80
IF %%I LSS 10 (
echo #include "/compressed/00%%I.z80" >> levels.z80 
) ELSE (
IF %%I LSS 100 (
echo #include "/compressed/0%%I.z80" >> levels.z80 
) ELSE (
IF %%I LSS 1000 (
echo #include "/compressed/%%I.z80" >> levels.z80 
)
)
)


echo #endif >> levels.z80
echo. >> levels.z80
)
pause