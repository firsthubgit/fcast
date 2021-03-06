:: Copyright 2018 Google LLC
::
:: Licensed under the Apache License, Version 2.0 (the "License");
:: you may not use this file except in compliance with the License.
:: You may obtain a copy of the License at
::
::      http://www.apache.org/licenses/LICENSE-2.0
::
:: Unless required by applicable law or agreed to in writing, software
:: distributed under the License is distributed on an "AS IS" BASIS,
:: WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
:: See the License for the specific language governing permissions and
:: limitations under the License.
rem @echo off
set PROJECT_DIR=..
set VERBOSE_SCRIPT_LOGGING=1
rem set FLUTTER_TARGET=../lib/main.dart
set FLUTTER_EPHEMERAL_DIR=.\flutter\ephemeral
set FLUTTER_ROOT=C:\w\flutter

:: Run flutter tool backend.
set BUILD_MODE=%~1
"%FLUTTER_ROOT%\packages\flutter_tools\bin\tool_backend" windows-x64 %BUILD_MODE%
