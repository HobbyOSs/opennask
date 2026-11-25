#----------------------------------------------------------
message(STATUS "Entering directory projects/30_day/harib27e/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(30_day_harib27e_OS    ${root_BINARY_DIR}/projects/30_day/harib27e/os.img)
set(30_day_harib27e_SYS	  ${root_BINARY_DIR}/projects/30_day/harib27e/os.sys)
set(30_day_harib27e_IPLB  ${root_BINARY_DIR}/projects/30_day/harib27e/ipl.bin)
set(30_day_harib27e_IPLS  ${root_SOURCE_DIR}/projects/30_day/harib27e/haribote/ipl09.nas)
set(30_day_harib27e_HEADB ${root_BINARY_DIR}/projects/30_day/harib27e/asmhead.bin)
set(30_day_harib27e_HEADS ${root_SOURCE_DIR}/projects/30_day/harib27e/haribote/asmhead.nas)
set(30_day_harib27e_BOOTB ${root_BINARY_DIR}/projects/30_day/harib27e/boot.bin)
set(30_day_harib27e_CCO	  ${root_BINARY_DIR}/projects/30_day/harib27e/boot.o)
file(GLOB 30_day_harib27e_CCS ${root_SOURCE_DIR}/projects/30_day/harib27e/haribote/*.c)
set(30_day_harib27e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(30_day_harib27e_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(30_day_harib27e_FUNCS ${root_SOURCE_DIR}/projects/30_day/harib27e/haribote/naskfunc.nas)
set(30_day_harib27e_FUNCO ${root_BINARY_DIR}/projects/30_day/harib27e/naskfunc.o)
set(30_day_harib27e_FONTS ${root_SOURCE_DIR}/projects/30_day/harib27e/haribote/hankaku.txt)
set(30_day_harib27e_FONTB ${root_BINARY_DIR}/projects/30_day/harib27e/hankaku.bin)
set(30_day_harib27e_FONTO ${root_BINARY_DIR}/projects/30_day/harib27e/hankaku.o)
set(30_day_harib27e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(30_day_harib27e_LIBGC ${root_BINARY_DIR}/projects/30_day/harib27e/libgo.o)
set(30_day_harib27e_WILDOBJ ${root_BINARY_DIR}/projects/30_day/harib27e/*.o)


add_custom_target(30_day_harib27e_run
  COMMAND ${QEMU} ${QEMUOPT} ${30_day_harib27e_OS}
  DEPENDS 30_day_harib27e_img
)
add_custom_target(30_day_harib27e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${30_day_harib27e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 30_day_harib27e_img
)
add_custom_target(30_day_harib27e_clean
  COMMAND rm -f ${30_day_harib27e_OS}
  COMMAND rm -f ${30_day_harib27e_SYS}
  COMMAND rm -f ${30_day_harib27e_IPLB}
  COMMAND rm -f ${30_day_harib27e_HEADB}
  COMMAND rm -f ${30_day_harib27e_BOOTB}
  COMMAND rm -f ${30_day_harib27e_WILDOBJ}
)
add_custom_target(30_day_harib27e_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27e_IPLS} ${30_day_harib27e_IPLB}
  DEPENDS ${30_day_harib27e_IPLS}
)
add_custom_command(
  OUTPUT ${30_day_harib27e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27e_HEADS} ${30_day_harib27e_HEADB}
  DEPENDS ${30_day_harib27e_HEADS}
  COMMENT "Generating asmhead.bin for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${30_day_harib27e_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27e_FUNCS} ${30_day_harib27e_FUNCO}
  DEPENDS ${30_day_harib27e_FUNCS}
  COMMENT "Generating naskfunc.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${30_day_harib27e_FONTO}
  COMMAND ${FONT} ${30_day_harib27e_FONTS} ${30_day_harib27e_FONTB}
  COMMAND ${B2O}  ${30_day_harib27e_FONTB} ${30_day_harib27e_FONTO} _hankaku
  DEPENDS ${30_day_harib27e_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${30_day_harib27e_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${30_day_harib27e_LIBGE} ${30_day_harib27e_LIBGC}
  DEPENDS ${30_day_harib27e_LIBGE}
  COMMENT "Converting golibc for 30_day_harib27e"
)

add_custom_target(30_day_harib27e_sys
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -T ${30_day_harib27e_LDS} ${30_day_harib27e_CCS} ${30_day_harib27e_WILDOBJ} -o ${30_day_harib27e_BOOTB}
  COMMAND cat ${30_day_harib27e_HEADB} ${30_day_harib27e_BOOTB} > ${30_day_harib27e_SYS}
  DEPENDS ${30_day_harib27e_HEADB} 30_day_harib27e_ipl
  DEPENDS ${30_day_harib27e_FUNCO}
  DEPENDS ${30_day_harib27e_FONTO} ${30_day_harib27e_LIBGC}
)
add_custom_target(30_day_harib27e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${30_day_harib27e_IPLB} -i ${30_day_harib27e_OS}
  COMMAND mcopy -i ${30_day_harib27e_OS} ${30_day_harib27e_SYS} ::
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/a.hrb ::a.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/gview.hrb ::gview.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/mmlplay.hrb ::mmlplay.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/tview.hrb ::tview.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/calc.hrb ::calc.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/invader.hrb ::invader.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/bball.hrb ::bball.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/notrec.hrb ::notrec.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/chklang.hrb ::chklang.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/iroha.hrb ::iroha.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/type.hrb ::type.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/color.hrb ::color.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_BINARY_DIR}/projects/30_day/harib27e/hello5.hrb ::hello5.hrb
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_SOURCE_DIR}/projects/30_day/harib27e/euc.txt ::euc.txt
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_SOURCE_DIR}/projects/30_day/harib27e/pictdata/night.bmp ::night.bmp
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_SOURCE_DIR}/projects/30_day/harib27e/pictdata/fujisan.jpg ::fujisan.jpg
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_SOURCE_DIR}/projects/30_day/harib27e/mmldata/kirakira.mml ::kirakira.mml
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_SOURCE_DIR}/projects/30_day/harib27e/mmldata/fujisan.mml ::fujisan.mml
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_SOURCE_DIR}/projects/30_day/harib27e/mmldata/daiku.mml ::daiku.mml
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_SOURCE_DIR}/projects/30_day/harib27e/mmldata/daigo.mml ::daigo.mml
  COMMAND mcopy -i ${30_day_harib27e_OS} ${root_SOURCE_DIR}/projects/30_day/harib27e/nihongo/nihongo.fnt ::nihongo.fnt
  DEPENDS 30_day_harib27e_sys ${root_BINARY_DIR}/projects/30_day/harib27e/a.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/hello3.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/hello4.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/winhelo.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/winhelo2.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/winhelo3.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/star1.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/stars.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/stars2.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/lines.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/walk.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/gview.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/mmlplay.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/tview.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/calc.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/invader.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/bball.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/notrec.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/chklang.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/iroha.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/type.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/sosu3.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/sosu2.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/sosu.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/noodle.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/beepdown.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/color.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/color2.hrb ${root_BINARY_DIR}/projects/30_day/harib27e/hello5.hrb ${root_SOURCE_DIR}/projects/30_day/harib27e/nihongo/nihongo.fnt ${root_SOURCE_DIR}/projects/30_day/harib27e/euc.txt ${root_SOURCE_DIR}/projects/30_day/harib27e/mmldata/daigo.mml ${root_SOURCE_DIR}/projects/30_day/harib27e/mmldata/daiku.mml ${root_SOURCE_DIR}/projects/30_day/harib27e/mmldata/fujisan.mml ${root_SOURCE_DIR}/projects/30_day/harib27e/mmldata/kirakira.mml ${root_SOURCE_DIR}/projects/30_day/harib27e/pictdata/fujisan.jpg ${root_SOURCE_DIR}/projects/30_day/harib27e/pictdata/night.bmp
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(30_day_harib27e_APIO
  ${root_BINARY_DIR}/projects/30_day/harib27e/api001.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api002.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api003.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api004.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api005.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api006.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api007.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api008.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api009.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api010.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api011.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api012.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api013.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api014.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api015.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api016.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api017.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api018.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api019.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api020.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api021.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api022.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api023.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api024.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api025.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api026.o
  ${root_BINARY_DIR}/projects/30_day/harib27e/api027.o)
find_program(NASM_EXECUTABLE nasm)
if(NOT NASM_EXECUTABLE)
  message(FATAL_ERROR "nasm is required to build gview (bmp.nasm) in 30_day/harib27e")
endif()
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/bmp.o
  COMMAND ${NASM_EXECUTABLE} -f coff -o ${root_BINARY_DIR}/projects/30_day/harib27e/bmp.o ${root_SOURCE_DIR}/projects/30_day/harib27e/gview/bmp.nasm
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/gview/bmp.nasm
  COMMENT "Assembling bmp.nasm for 30_day_harib27e (nasm)")
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/gview.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -I ${root_SOURCE_DIR}/projects/30_day/harib27e/haribote -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/gview/gview.c ${root_SOURCE_DIR}/projects/30_day/harib27e/gview/jpeg.c ${30_day_harib27e_APIO} ${root_BINARY_DIR}/projects/30_day/harib27e/bmp.o ${root_BINARY_DIR}/projects/30_day/harib27e/applib/strtol.o ${root_BINARY_DIR}/projects/30_day/harib27e/libgo.o ${root_BINARY_DIR}/projects/30_day/harib27e/rand.o -o ${root_BINARY_DIR}/projects/30_day/harib27e/gview.hrb
  DEPENDS ${root_BINARY_DIR}/projects/30_day/harib27e/applib/strtol.o ${root_SOURCE_DIR}/projects/30_day/harib27e/gview/gview.c ${root_SOURCE_DIR}/projects/30_day/harib27e/gview/jpeg.c ${30_day_harib27e_APPLDS} ${30_day_harib27e_APIO} ${root_BINARY_DIR}/projects/30_day/harib27e/bmp.o ${30_day_harib27e_LIBGC}
  COMMENT "Generating gview.hrb for 30_day_harib27e")
set(30_day_harib27e_RANDO ${root_BINARY_DIR}/projects/30_day/harib27e/rand.o)
set(30_day_harib27e_STRTOLO ${root_BINARY_DIR}/projects/30_day/harib27e/applib/strtol.o)
add_custom_command(
  OUTPUT ${30_day_harib27e_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${30_day_harib27e_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/applib/strtol.o
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -I ${root_SOURCE_DIR}/golibc -x c -c ${root_SOURCE_DIR}/golibc/strtol.c -o ${root_BINARY_DIR}/projects/30_day/harib27e/applib/strtol.o
  DEPENDS ${root_SOURCE_DIR}/golibc/strtol.c
  COMMENT "Generating strtol.o for 30_day_harib27e")
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api001.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api001.nas
  COMMENT "Generating api001.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api002.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api002.nas
  COMMENT "Generating api002.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api003.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api003.nas
  COMMENT "Generating api003.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api004.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api004.nas
  COMMENT "Generating api004.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api005.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api005.nas
  COMMENT "Generating api005.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api006.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api006.nas
  COMMENT "Generating api006.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api007.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api007.nas
  COMMENT "Generating api007.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api008.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api008.nas
  COMMENT "Generating api008.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api009.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api009.nas
  COMMENT "Generating api009.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api010.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api010.nas
  COMMENT "Generating api010.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api011.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api011.nas
  COMMENT "Generating api011.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api012.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api012.nas
  COMMENT "Generating api012.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api013.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api013.nas
  COMMENT "Generating api013.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api014.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api014.nas
  COMMENT "Generating api014.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api015.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api015.nas
  COMMENT "Generating api015.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api016.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api016.nas
  COMMENT "Generating api016.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api017.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api017.nas
  COMMENT "Generating api017.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api018.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api018.nas
  COMMENT "Generating api018.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api019.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api019.nas
  COMMENT "Generating api019.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api020.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api020.nas
  COMMENT "Generating api020.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api021.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api021.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api021.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api021.nas
  COMMENT "Generating api021.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api022.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api022.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api022.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api022.nas
  COMMENT "Generating api022.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api023.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api023.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api023.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api023.nas
  COMMENT "Generating api023.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api024.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api024.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api024.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api024.nas
  COMMENT "Generating api024.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api025.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api025.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api025.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api025.nas
  COMMENT "Generating api025.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api026.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api026.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api026.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api026.nas
  COMMENT "Generating api026.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/api027.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api027.nas ${root_BINARY_DIR}/projects/30_day/harib27e/api027.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/apilib/api027.nas
  COMMENT "Generating api027.o for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/a/a.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating a.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/hello3/hello3.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating hello3.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/hello4/hello4.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating hello4.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/winhelo/winhelo.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating winhelo.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/winhelo2/winhelo2.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating winhelo2.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/winhelo3/winhelo3.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating winhelo3.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/star1/star1.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating star1.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/stars/stars.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating stars.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/stars2/stars2.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating stars2.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/lines/lines.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating lines.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/walk/walk.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating walk.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/mmlplay.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/mmlplay/mmlplay.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/mmlplay.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/mmlplay/mmlplay.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating mmlplay.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/tview.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/tview/tview.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/tview.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/tview/tview.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating tview.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/calc.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/calc/calc.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/calc.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/calc/calc.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating calc.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/invader.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/invader/invader.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/invader.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/invader/invader.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating invader.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/bball.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/bball/bball.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/bball.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/bball/bball.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating bball.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/notrec.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/notrec/notrec.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/notrec.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/notrec/notrec.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating notrec.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/chklang.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/chklang/chklang.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/chklang.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/chklang/chklang.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating chklang.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/iroha.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/iroha/iroha.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/iroha.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/iroha/iroha.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating iroha.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/type.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/type/type.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/type.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/type/type.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating type.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/sosu3/sosu3.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating sosu3.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/sosu2/sosu2.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating sosu2.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/sosu/sosu.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating sosu.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/color2/color2.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating color2.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/color/color.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating color.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/beepdown/beepdown.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating beepdown.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27e -T ${30_day_harib27e_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27e/noodle/noodle.c ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27e/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27e_APIO} ${30_day_harib27e_STRTOLO} ${30_day_harib27e_LIBGC} ${30_day_harib27e_RANDO}
  COMMENT "Generating noodle.hrb for 30_day_harib27e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27e/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27e/hello5/hello5.nas ${root_BINARY_DIR}/projects/30_day/harib27e/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27e/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 30_day_harib27e"
)
