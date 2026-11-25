#----------------------------------------------------------
message(STATUS "Entering directory projects/30_day/harib27f/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(30_day_harib27f_OS    ${root_BINARY_DIR}/projects/30_day/harib27f/os.img)
set(30_day_harib27f_SYS	  ${root_BINARY_DIR}/projects/30_day/harib27f/os.sys)
set(30_day_harib27f_IPLB  ${root_BINARY_DIR}/projects/30_day/harib27f/ipl.bin)
set(30_day_harib27f_IPLS  ${root_SOURCE_DIR}/projects/30_day/harib27f/haribote/ipl09.nas)
set(30_day_harib27f_HEADB ${root_BINARY_DIR}/projects/30_day/harib27f/asmhead.bin)
set(30_day_harib27f_HEADS ${root_SOURCE_DIR}/projects/30_day/harib27f/haribote/asmhead.nas)
set(30_day_harib27f_BOOTB ${root_BINARY_DIR}/projects/30_day/harib27f/boot.bin)
set(30_day_harib27f_CCO	  ${root_BINARY_DIR}/projects/30_day/harib27f/boot.o)
file(GLOB 30_day_harib27f_CCS ${root_SOURCE_DIR}/projects/30_day/harib27f/haribote/*.c)
set(30_day_harib27f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(30_day_harib27f_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(30_day_harib27f_FUNCS ${root_SOURCE_DIR}/projects/30_day/harib27f/haribote/naskfunc.nas)
set(30_day_harib27f_FUNCO ${root_BINARY_DIR}/projects/30_day/harib27f/naskfunc.o)
set(30_day_harib27f_FONTS ${root_SOURCE_DIR}/projects/30_day/harib27f/haribote/hankaku.txt)
set(30_day_harib27f_FONTB ${root_BINARY_DIR}/projects/30_day/harib27f/hankaku.bin)
set(30_day_harib27f_FONTO ${root_BINARY_DIR}/projects/30_day/harib27f/hankaku.o)
set(30_day_harib27f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(30_day_harib27f_LIBGC ${root_BINARY_DIR}/projects/30_day/harib27f/libgo.o)
set(30_day_harib27f_WILDOBJ ${root_BINARY_DIR}/projects/30_day/harib27f/*.o)


add_custom_target(30_day_harib27f_run
  COMMAND ${QEMU} ${QEMUOPT} ${30_day_harib27f_OS}
  DEPENDS 30_day_harib27f_img
)
add_custom_target(30_day_harib27f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${30_day_harib27f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 30_day_harib27f_img
)
add_custom_target(30_day_harib27f_clean
  COMMAND rm -f ${30_day_harib27f_OS}
  COMMAND rm -f ${30_day_harib27f_SYS}
  COMMAND rm -f ${30_day_harib27f_IPLB}
  COMMAND rm -f ${30_day_harib27f_HEADB}
  COMMAND rm -f ${30_day_harib27f_BOOTB}
  COMMAND rm -f ${30_day_harib27f_WILDOBJ}
)
add_custom_target(30_day_harib27f_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27f_IPLS} ${30_day_harib27f_IPLB}
  DEPENDS ${30_day_harib27f_IPLS}
)
add_custom_command(
  OUTPUT ${30_day_harib27f_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27f_HEADS} ${30_day_harib27f_HEADB}
  DEPENDS ${30_day_harib27f_HEADS}
  COMMENT "Generating asmhead.bin for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${30_day_harib27f_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27f_FUNCS} ${30_day_harib27f_FUNCO}
  DEPENDS ${30_day_harib27f_FUNCS}
  COMMENT "Generating naskfunc.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${30_day_harib27f_FONTO}
  COMMAND ${FONT} ${30_day_harib27f_FONTS} ${30_day_harib27f_FONTB}
  COMMAND ${B2O}  ${30_day_harib27f_FONTB} ${30_day_harib27f_FONTO} _hankaku
  DEPENDS ${30_day_harib27f_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${30_day_harib27f_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${30_day_harib27f_LIBGE} ${30_day_harib27f_LIBGC}
  DEPENDS ${30_day_harib27f_LIBGE}
  COMMENT "Converting golibc for 30_day_harib27f"
)

add_custom_target(30_day_harib27f_sys
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -T ${30_day_harib27f_LDS} ${30_day_harib27f_CCS} ${30_day_harib27f_WILDOBJ} -o ${30_day_harib27f_BOOTB}
  COMMAND cat ${30_day_harib27f_HEADB} ${30_day_harib27f_BOOTB} > ${30_day_harib27f_SYS}
  DEPENDS ${30_day_harib27f_HEADB} 30_day_harib27f_ipl
  DEPENDS ${30_day_harib27f_FUNCO}
  DEPENDS ${30_day_harib27f_FONTO} ${30_day_harib27f_LIBGC}
)
add_custom_target(30_day_harib27f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${30_day_harib27f_IPLB} -i ${30_day_harib27f_OS}
  COMMAND mcopy -i ${30_day_harib27f_OS} ${30_day_harib27f_SYS} ::
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/a.hrb ::a.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/gview.hrb ::gview.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/mmlplay.hrb ::mmlplay.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/tview.hrb ::tview.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/calc.hrb ::calc.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/invader.hrb ::invader.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/bball.hrb ::bball.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/notrec.hrb ::notrec.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/chklang.hrb ::chklang.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/iroha.hrb ::iroha.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/type.hrb ::type.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/color.hrb ::color.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_BINARY_DIR}/projects/30_day/harib27f/hello5.hrb ::hello5.hrb
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_SOURCE_DIR}/projects/30_day/harib27f/euc.txt ::euc.txt
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_SOURCE_DIR}/projects/30_day/harib27f/pictdata/night.bmp ::night.bmp
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_SOURCE_DIR}/projects/30_day/harib27f/pictdata/fujisan.jpg ::fujisan.jpg
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_SOURCE_DIR}/projects/30_day/harib27f/mmldata/kirakira.mml ::kirakira.mml
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_SOURCE_DIR}/projects/30_day/harib27f/mmldata/fujisan.mml ::fujisan.mml
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_SOURCE_DIR}/projects/30_day/harib27f/mmldata/daiku.mml ::daiku.mml
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_SOURCE_DIR}/projects/30_day/harib27f/mmldata/daigo.mml ::daigo.mml
  COMMAND mcopy -i ${30_day_harib27f_OS} ${root_SOURCE_DIR}/projects/30_day/harib27f/nihongo/nihongo.fnt ::nihongo.fnt
  DEPENDS 30_day_harib27f_sys ${root_BINARY_DIR}/projects/30_day/harib27f/a.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/hello3.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/hello4.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/winhelo.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/winhelo2.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/winhelo3.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/star1.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/stars.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/stars2.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/lines.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/walk.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/gview.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/mmlplay.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/tview.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/calc.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/invader.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/bball.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/notrec.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/chklang.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/iroha.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/type.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/sosu3.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/sosu2.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/sosu.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/noodle.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/beepdown.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/color.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/color2.hrb ${root_BINARY_DIR}/projects/30_day/harib27f/hello5.hrb ${root_SOURCE_DIR}/projects/30_day/harib27f/nihongo/nihongo.fnt ${root_SOURCE_DIR}/projects/30_day/harib27f/euc.txt ${root_SOURCE_DIR}/projects/30_day/harib27f/mmldata/daigo.mml ${root_SOURCE_DIR}/projects/30_day/harib27f/mmldata/daiku.mml ${root_SOURCE_DIR}/projects/30_day/harib27f/mmldata/fujisan.mml ${root_SOURCE_DIR}/projects/30_day/harib27f/mmldata/kirakira.mml ${root_SOURCE_DIR}/projects/30_day/harib27f/pictdata/fujisan.jpg ${root_SOURCE_DIR}/projects/30_day/harib27f/pictdata/night.bmp
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(30_day_harib27f_APIO
  ${root_BINARY_DIR}/projects/30_day/harib27f/api001.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api002.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api003.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api004.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api005.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api006.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api007.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api008.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api009.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api010.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api011.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api012.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api013.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api014.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api015.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api016.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api017.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api018.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api019.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api020.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api021.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api022.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api023.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api024.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api025.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api026.o
  ${root_BINARY_DIR}/projects/30_day/harib27f/api027.o)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/bmp.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/gview/bmp.nasm ${root_BINARY_DIR}/projects/30_day/harib27f/bmp.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/gview/bmp.nasm
  COMMENT "Assembling bmp.nasm for 30_day/harib27f")
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/gview.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -I ${root_SOURCE_DIR}/projects/30_day/harib27f/haribote -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/gview/gview.c ${root_SOURCE_DIR}/projects/30_day/harib27f/gview/jpeg.c ${30_day_harib27f_APIO} ${root_BINARY_DIR}/projects/30_day/harib27f/bmp.o ${root_BINARY_DIR}/projects/30_day/harib27f/applib/strtol.o ${root_BINARY_DIR}/projects/30_day/harib27f/libgo.o ${root_BINARY_DIR}/projects/30_day/harib27f/rand.o -o ${root_BINARY_DIR}/projects/30_day/harib27f/gview.hrb
  DEPENDS ${root_BINARY_DIR}/projects/30_day/harib27f/applib/strtol.o ${root_SOURCE_DIR}/projects/30_day/harib27f/gview/gview.c ${root_SOURCE_DIR}/projects/30_day/harib27f/gview/jpeg.c ${30_day_harib27f_APPLDS} ${30_day_harib27f_APIO} ${root_BINARY_DIR}/projects/30_day/harib27f/bmp.o ${30_day_harib27f_LIBGC}
  COMMENT "Generating gview.hrb for 30_day_harib27f")
set(30_day_harib27f_RANDO ${root_BINARY_DIR}/projects/30_day/harib27f/rand.o)
set(30_day_harib27f_STRTOLO ${root_BINARY_DIR}/projects/30_day/harib27f/applib/strtol.o)
add_custom_command(
  OUTPUT ${30_day_harib27f_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${30_day_harib27f_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/applib/strtol.o
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -I ${root_SOURCE_DIR}/golibc -x c -c ${root_SOURCE_DIR}/golibc/strtol.c -o ${root_BINARY_DIR}/projects/30_day/harib27f/applib/strtol.o
  DEPENDS ${root_SOURCE_DIR}/golibc/strtol.c
  COMMENT "Generating strtol.o for 30_day_harib27f")
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api001.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api001.nas
  COMMENT "Generating api001.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api002.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api002.nas
  COMMENT "Generating api002.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api003.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api003.nas
  COMMENT "Generating api003.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api004.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api004.nas
  COMMENT "Generating api004.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api005.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api005.nas
  COMMENT "Generating api005.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api006.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api006.nas
  COMMENT "Generating api006.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api007.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api007.nas
  COMMENT "Generating api007.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api008.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api008.nas
  COMMENT "Generating api008.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api009.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api009.nas
  COMMENT "Generating api009.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api010.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api010.nas
  COMMENT "Generating api010.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api011.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api011.nas
  COMMENT "Generating api011.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api012.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api012.nas
  COMMENT "Generating api012.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api013.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api013.nas
  COMMENT "Generating api013.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api014.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api014.nas
  COMMENT "Generating api014.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api015.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api015.nas
  COMMENT "Generating api015.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api016.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api016.nas
  COMMENT "Generating api016.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api017.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api017.nas
  COMMENT "Generating api017.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api018.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api018.nas
  COMMENT "Generating api018.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api019.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api019.nas
  COMMENT "Generating api019.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api020.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api020.nas
  COMMENT "Generating api020.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api021.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api021.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api021.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api021.nas
  COMMENT "Generating api021.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api022.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api022.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api022.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api022.nas
  COMMENT "Generating api022.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api023.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api023.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api023.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api023.nas
  COMMENT "Generating api023.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api024.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api024.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api024.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api024.nas
  COMMENT "Generating api024.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api025.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api025.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api025.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api025.nas
  COMMENT "Generating api025.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api026.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api026.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api026.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api026.nas
  COMMENT "Generating api026.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/api027.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api027.nas ${root_BINARY_DIR}/projects/30_day/harib27f/api027.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/apilib/api027.nas
  COMMENT "Generating api027.o for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/a/a.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating a.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/hello3/hello3.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating hello3.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/hello4/hello4.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating hello4.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/winhelo/winhelo.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating winhelo.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/winhelo2/winhelo2.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating winhelo2.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/winhelo3/winhelo3.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating winhelo3.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/star1/star1.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating star1.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/stars/stars.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating stars.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/stars2/stars2.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating stars2.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/lines/lines.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating lines.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/walk/walk.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating walk.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/mmlplay.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/mmlplay/mmlplay.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/mmlplay.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/mmlplay/mmlplay.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating mmlplay.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/tview.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/tview/tview.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/tview.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/tview/tview.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating tview.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/calc.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/calc/calc.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/calc.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/calc/calc.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating calc.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/invader.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/invader/invader.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/invader.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/invader/invader.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating invader.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/bball.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/bball/bball.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/bball.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/bball/bball.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating bball.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/notrec.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/notrec/notrec.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/notrec.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/notrec/notrec.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating notrec.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/chklang.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/chklang/chklang.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/chklang.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/chklang/chklang.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating chklang.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/iroha.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/iroha/iroha.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/iroha.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/iroha/iroha.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating iroha.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/type.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/type/type.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/type.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/type/type.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating type.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/sosu3/sosu3.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating sosu3.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/sosu2/sosu2.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating sosu2.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/sosu/sosu.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating sosu.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/color2/color2.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating color2.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/color/color.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating color.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/beepdown/beepdown.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating beepdown.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27f -T ${30_day_harib27f_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27f/noodle/noodle.c ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27f/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27f_APIO} ${30_day_harib27f_STRTOLO} ${30_day_harib27f_LIBGC} ${30_day_harib27f_RANDO}
  COMMENT "Generating noodle.hrb for 30_day_harib27f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27f/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27f/hello5/hello5.nas ${root_BINARY_DIR}/projects/30_day/harib27f/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27f/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 30_day_harib27f"
)
