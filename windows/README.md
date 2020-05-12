# 构建 windows 可执行程序不容易

全局环境变量

## 在VS外部运行 windows/scripts/prepare_dependencies.bat

```windows/scripts/a.bat
set PROJECT_DIR=.
set VERBOSE_SCRIPT_LOGGING=1
set FLUTTER_TARGET=lib\main.dart
rem set FLUTTER_ENGINE

set FLUTTER_EPHEMERAL_DIR=windows\flutter\ephemeral

dart c:\w\flutter\packages\flutter_tools\bin\tool_backend.dart windows-x64 debug
```

## 启动 VS
```windows/start-vs.bat
flutter pub get

cd windows

set FLUTTER_EPHEMERAL_DIR=..\windows\flutter\ephemeral
start Runner.sln
```
