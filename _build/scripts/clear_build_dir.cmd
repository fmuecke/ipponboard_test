IF "%BUILD_DIR%"=="" GOTO :EOF

IF NOT EXIST "%BUILD_DIR%" GOTO :EOF

RD /S /Q "%BUILD_DIR%"

exit /B 0