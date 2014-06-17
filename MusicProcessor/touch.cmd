@echo off
cd %ATLANTIS_MUSIC%
for /f "delims=" %%f in ('dir /s /b music_ok music_errors') do copy /by %%f + nul %%f > nul
