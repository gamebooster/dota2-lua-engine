xcopy /y Injector.exe ..\..\Release\
cd ..\..\Release
.\Injector.exe --module-name dota2-lua-engine.dll --process-name dota.exe --eject
exit 0