cmake_minimum_required(VERSION 3.6)
project(Lab41)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Lab41/src/ASF/common/boards/board.h
        Lab41/src/ASF/common/utils/interrupt/interrupt_avr8.h
        Lab41/src/ASF/common/utils/interrupt.h
        Lab41/src/ASF/common/utils/parts.h
        Lab41/src/ASF/mega/boards/stk600/rcx_x/init.c
        Lab41/src/ASF/mega/utils/assembler/gas.h
        Lab41/src/ASF/mega/utils/preprocessor/mrepeat.h
        Lab41/src/ASF/mega/utils/preprocessor/preprocessor.h
        Lab41/src/ASF/mega/utils/preprocessor/stringz.h
        Lab41/src/ASF/mega/utils/preprocessor/tpaste.h
        Lab41/src/ASF/mega/utils/assembler.h
        Lab41/src/ASF/mega/utils/compiler.h
        Lab41/src/ASF/mega/utils/progmem.h
        Lab41/src/ASF/mega/utils/status_codes.h
        Lab41/src/config/conf_board.h
        Lab41/src/asf.h
        Lab41/src/main.c
        Lab41/codeUtils.c
        Lab41/codeUtils.h
        Lab41/GUI.c
        Lab41/GUI.h
        Lab41/KEY.c
        Lab41/KEY.h
        Lab41/main.c
        Lab41/PWM.c
        Lab41/PWM.h
        Lab41/TinyTimber.c
        Lab41/TinyTimber.h)

add_executable(Lab41 ${SOURCE_FILES})