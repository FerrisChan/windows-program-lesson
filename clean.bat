@echo off
echo "clean compile file"

cd %~dp0
REM cd .\homework1-dll-1\Release
echo %~dp0

SET THEDIR=%~dp0homework1-dll-1\Release
DEL "%THEDIR%\*" /F /Q /A


SET exeDIR=%~dp0homework1-exe-1\Release
DEL "%exeDIR%\*" /F /Q /A

SET homework2-gTest-for-lesson1=%~dp0homework1-exe-1\Release
DEL "%homework2-gTest-for-lesson1%\*" /F /Q /A

pause