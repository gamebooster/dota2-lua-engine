@echo off
set expanded_list=
for /R %%F in (*.cc) do call set expanded_list=%%expanded_list%% "%%F"

CMD /C %~dp0\cpplint.bat %expanded_list%

set expanded_list=
for /R %%F in (*.h) do call set expanded_list=%%expanded_list%% "%%F"

CMD /C %~dp0\cpplint.bat %expanded_list%