@echo off
call ..\env_cfg.bat

REM -- language file
SET DEST=..\bin
SET THIRDPARTY=..\..\3rdParty

rem (mkdir is recursive) if not exist "%DEST%" mkdir "%DEST%">nul
if not exist "%DEST%\lang" mkdir "%DEST%\lang">nul
"%QTDIR%\lrelease" -compress -silent ..\i18n\core_de.ts -qm ..\i18n\core_de.qm
"%QTDIR%\lrelease" -compress -silent ..\i18n\Ipponboard_team_de.ts -qm ..\i18n\Ipponboard_team_de.qm
"%QTDIR%\lrelease" -compress -silent ..\i18n\VersionSelector_de.ts -qm ..\i18n\VersionSelector_de.qm
copy /Y "..\i18n\core_de.qm" "%DEST%\lang">nul
copy /Y "..\i18n\Ipponboard_team_de.qm" "%DEST%\lang">nul
copy /Y "..\i18n\VersionSelector_de.qm" "%DEST%\lang">nul

REM -- sounds
if not exist "%DEST%\sounds" mkdir "%DEST%\sounds">nul
copy /Y "..\base\sounds\buzzer.wav" "%DEST%\sounds">nul
copy /Y "%THIRDPARTY%\sounds\*.wav" "%DEST%\sounds">nul

REM -- binaries
copy /Y "%THIRDPARTY%\qt\4.8.5-vc120xp\*.dll" "%DEST%">nul
copy /Y "%THIRDPARTY%\msvc120\*.dll" "%DEST%">nul

REM -- doc
rem copy /Y "%DEST%\doc\Anleitung.pdf" "%DEST%">nul
rem copy /Y "%DEST%\doc\manual.pdf" "%DEST%">nul

REM -- clubs
if not exist "%DEST%\clubs" mkdir "%DEST%\clubs">nul
copy /Y "clubs\clubs.xml" "%DEST%\clubs">nul
copy /Y "clubs\*.png" "%DEST%\clubs">nul

REM -- templates
if not exist "%DEST%\templates" mkdir "%DEST%\templates">nul
copy /Y "templates\*.*" "%DEST%\templates">nul

REM -- modes
copy /Y TournamentModes.ini "%DEST%">nul

REM -- programme
REM if not exist "%DEST%\Ipponboard_team.exe" (
	REM echo ERROR: Ipponboard_team.exe not found
	REM pause
	REM exit 1
REM )

copy /Y "..\CHANGELOG.txt" "%DEST%">nul