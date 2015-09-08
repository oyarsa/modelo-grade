@echo off

rem Get start time:
for /F "tokens=1-4 delims=:.," %%a in ("%time%") do (
   set /A "start=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

rem Any process here...
set contador=1

if not "%1" == "" (
	set file=%1
) else (
	set file=resultado.txt
)

setlocal ENABLEDELAYEDEXPANSION

for %%a in (*.dat) do (
	set aln=Aluno !contador!
	echo !aln! >> !file!
	echo. >> !file!
	set text=Resolvendo aluno !contador!
	echo !text!
	echo.
	echo. >> !file!
	oplrun grade.opl %%a >> !file! 
	set /A contador=contador+1
	echo. >> !file!
)

endlocal

rem Get end time:
for /F "tokens=1-4 delims=:.," %%a in ("%time%") do (
   set /A "end=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

rem Get elapsed time:
set /A elapsed=end-start

rem Show elapsed time:
set /A hh=elapsed/(60*60*100), rest=elapsed%%(60*60*100), mm=rest/(60*100), rest%%=60*100, ss=rest/100, cc=rest%%100
if %mm% lss 10 set mm=0%mm%
if %ss% lss 10 set ss=0%ss%
if %cc% lss 10 set cc=0%cc%
echo %hh%:%mm%:%ss%,%cc% > tempo.txt
echo.
echo %hh%:%mm%:%ss%,%cc%
pause

