@ echo off
setlocal
set BINPATH="C:\avr-gcc\avr-gcc-8.0_2017-07-19_mingw32\bin"
set INCPATH="C:\avr-gcc\avr-gcc-8.0_2017-07-19_mingw32\avr\include"

set SOURCEPATH=.\source
set BUILDPATH=.\build

set OPTFLAGS=-Os -fno-jump-tables -flto -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax
set CSTDFLAGS=-funsigned-char -funsigned-bitfields -c -std=gnu++14

echo Compiling...
%BINPATH%\avr-g++.exe -DNDEBUG  -I%INCPATH% %OPTFLAGS% %CSTDFLAGS% -Wall  -mmcu=atmega328p -o UPDI_lo_lvl.o %SOURCEPATH%/UPDI_lo_lvl.cpp
%BINPATH%\avr-g++.exe -DNDEBUG  -I%INCPATH% %OPTFLAGS% %CSTDFLAGS% -Wall  -mmcu=atmega328p -o stk2updi.o %SOURCEPATH%/stk2updi.cpp
%BINPATH%\avr-g++.exe -DNDEBUG  -I%INCPATH% %OPTFLAGS% %CSTDFLAGS% -Wall  -mmcu=atmega328p -o stk_io.o %SOURCEPATH%/stk_io.cpp
%BINPATH%\avr-g++.exe -DNDEBUG  -I%INCPATH% %OPTFLAGS% %CSTDFLAGS% -Wall  -mmcu=atmega328p -o STK500.o %SOURCEPATH%/STK500.cpp
%BINPATH%\avr-g++.exe -DNDEBUG  -I%INCPATH% %OPTFLAGS% %CSTDFLAGS% -Wall  -mmcu=atmega328p -o updi_io.o %SOURCEPATH%/updi_io.cpp

echo Linking...
mkdir %BUILDPATH%
set OPTFLAGS=-Os -flto -mrelax
%BINPATH%\avr-g++.exe -o %BUILDPATH%\STK2UPDI.elf  stk2updi.o STK500.o stk_io.o UPDI_lo_lvl.o updi_io.o   -Wl,-Map="%BUILDPATH%\STK2UPDI.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega328p %OPTFLAGS%

echo Cleaning up...
del *.o

cd %BUILDPATH%

echo Creating HEX file...
%BINPATH%\avr-objcopy.exe -O ihex "STK2UPDI.elf" "STK2UPDI.hex"

echo Creating LSS file...
%BINPATH%\avr-objdump.exe -h -S "STK2UPDI.elf" > "STK2UPDI.lss"
