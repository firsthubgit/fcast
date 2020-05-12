
x64adapter:
	cl adapter.cpp /DADAPTER_LIBRARY /Iinclude /link /DLL lib\tcd.lib /MACHINE:x64 /out:x64adapter.dll
	copy x64adapter.dll ..\build\windows\x64\Debug\Runner

all: x64adapter
