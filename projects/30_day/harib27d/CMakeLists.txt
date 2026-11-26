#----------------------------------------------------------
message(STATUS "Entering directory projects/30_day/harib27d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(30_day_harib27d_OS    ${root_BINARY_DIR}/projects/30_day/harib27d/os.img)
set(30_day_harib27d_SYS	  ${root_BINARY_DIR}/projects/30_day/harib27d/os.sys)
set(30_day_harib27d_IPLB  ${root_BINARY_DIR}/projects/30_day/harib27d/ipl.bin)
set(30_day_harib27d_IPLS  ${root_SOURCE_DIR}/projects/30_day/harib27d/haribote/ipl20.nas)
set(30_day_harib27d_HEADB ${root_BINARY_DIR}/projects/30_day/harib27d/asmhead.bin)
set(30_day_harib27d_HEADS ${root_SOURCE_DIR}/projects/30_day/harib27d/haribote/asmhead.nas)
set(30_day_harib27d_BOOTB ${root_BINARY_DIR}/projects/30_day/harib27d/boot.bin)
set(30_day_harib27d_CCO	  ${root_BINARY_DIR}/projects/30_day/harib27d/boot.o)
file(GLOB 30_day_harib27d_CCS ${root_SOURCE_DIR}/projects/30_day/harib27d/haribote/*.c)
set(30_day_harib27d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(30_day_harib27d_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(30_day_harib27d_FUNCS ${root_SOURCE_DIR}/projects/30_day/harib27d/haribote/naskfunc.nas)
set(30_day_harib27d_FUNCO ${root_BINARY_DIR}/projects/30_day/harib27d/naskfunc.o)
set(30_day_harib27d_FONTS ${root_SOURCE_DIR}/projects/30_day/harib27d/haribote/hankaku.txt)
set(30_day_harib27d_FONTB ${root_BINARY_DIR}/projects/30_day/harib27d/hankaku.bin)
set(30_day_harib27d_FONTO ${root_BINARY_DIR}/projects/30_day/harib27d/hankaku.o)
set(30_day_harib27d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(30_day_harib27d_LIBGC ${root_BINARY_DIR}/projects/30_day/harib27d/libgo.o)
set(30_day_harib27d_WILDOBJ ${root_BINARY_DIR}/projects/30_day/harib27d/*.o)


add_custom_target(30_day_harib27d_run
  COMMAND ${QEMU} ${QEMUOPT} ${30_day_harib27d_OS}
  DEPENDS 30_day_harib27d_img
)
add_custom_target(30_day_harib27d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${30_day_harib27d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 30_day_harib27d_img
)
add_custom_target(30_day_harib27d_clean
  COMMAND rm -f ${30_day_harib27d_OS}
  COMMAND rm -f ${30_day_harib27d_SYS}
  COMMAND rm -f ${30_day_harib27d_IPLB}
  COMMAND rm -f ${30_day_harib27d_HEADB}
  COMMAND rm -f ${30_day_harib27d_BOOTB}
  COMMAND rm -f ${30_day_harib27d_WILDOBJ}
)
add_custom_target(30_day_harib27d_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27d_IPLS} ${30_day_harib27d_IPLB}
  DEPENDS ${30_day_harib27d_IPLS}
)
add_custom_command(
  OUTPUT ${30_day_harib27d_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27d_HEADS} ${30_day_harib27d_HEADB}
  DEPENDS ${30_day_harib27d_HEADS}
  COMMENT "Generating asmhead.bin for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${30_day_harib27d_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27d_FUNCS} ${30_day_harib27d_FUNCO}
  DEPENDS ${30_day_harib27d_FUNCS}
  COMMENT "Generating naskfunc.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${30_day_harib27d_FONTO}
  COMMAND ${FONT} ${30_day_harib27d_FONTS} ${30_day_harib27d_FONTB}
  COMMAND ${B2O}  ${30_day_harib27d_FONTB} ${30_day_harib27d_FONTO} _hankaku
  DEPENDS ${30_day_harib27d_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${30_day_harib27d_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${30_day_harib27d_LIBGE} ${30_day_harib27d_LIBGC}
  DEPENDS ${30_day_harib27d_LIBGE}
  COMMENT "Converting golibc for 30_day_harib27d"
)

add_custom_target(30_day_harib27d_sys
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -T ${30_day_harib27d_LDS} ${30_day_harib27d_CCS} ${30_day_harib27d_WILDOBJ} -o ${30_day_harib27d_BOOTB}
  COMMAND cat ${30_day_harib27d_HEADB} ${30_day_harib27d_BOOTB} > ${30_day_harib27d_SYS}
  DEPENDS ${30_day_harib27d_HEADB} 30_day_harib27d_ipl
  DEPENDS ${30_day_harib27d_FUNCO}
  DEPENDS ${30_day_harib27d_FONTO} ${30_day_harib27d_LIBGC}
)
add_custom_target(30_day_harib27d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${30_day_harib27d_IPLB} -i ${30_day_harib27d_OS}
  COMMAND mcopy -i ${30_day_harib27d_OS} ${30_day_harib27d_SYS} ::
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/a.hrb ::a.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/gview.hrb ::gview.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/mmlplay.hrb ::mmlplay.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/tview.hrb ::tview.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/calc.hrb ::calc.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/invader.hrb ::invader.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/bball.hrb ::bball.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/notrec.hrb ::notrec.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/chklang.hrb ::chklang.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/iroha.hrb ::iroha.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/type.hrb ::type.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/color.hrb ::color.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_BINARY_DIR}/projects/30_day/harib27d/hello5.hrb ::hello5.hrb
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_SOURCE_DIR}/projects/30_day/harib27d/euc.txt ::euc.txt
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_SOURCE_DIR}/projects/30_day/harib27d/pictdata/night.bmp ::night.bmp
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_SOURCE_DIR}/projects/30_day/harib27d/pictdata/fujisan.jpg ::fujisan.jpg
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_SOURCE_DIR}/projects/30_day/harib27d/mmldata/kirakira.mml ::kirakira.mml
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_SOURCE_DIR}/projects/30_day/harib27d/mmldata/fujisan.mml ::fujisan.mml
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_SOURCE_DIR}/projects/30_day/harib27d/mmldata/daiku.mml ::daiku.mml
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_SOURCE_DIR}/projects/30_day/harib27d/mmldata/daigo.mml ::daigo.mml
  COMMAND mcopy -i ${30_day_harib27d_OS} ${root_SOURCE_DIR}/projects/30_day/harib27d/nihongo/nihongo.fnt ::nihongo.fnt
  DEPENDS 30_day_harib27d_sys ${root_BINARY_DIR}/projects/30_day/harib27d/a.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/hello3.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/hello4.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/winhelo.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/winhelo2.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/winhelo3.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/star1.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/stars.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/stars2.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/lines.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/walk.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/gview.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/mmlplay.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/tview.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/calc.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/invader.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/bball.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/notrec.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/chklang.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/iroha.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/type.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/sosu3.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/sosu2.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/sosu.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/noodle.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/beepdown.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/color.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/color2.hrb ${root_BINARY_DIR}/projects/30_day/harib27d/hello5.hrb ${root_SOURCE_DIR}/projects/30_day/harib27d/nihongo/nihongo.fnt ${root_SOURCE_DIR}/projects/30_day/harib27d/euc.txt ${root_SOURCE_DIR}/projects/30_day/harib27d/mmldata/daigo.mml ${root_SOURCE_DIR}/projects/30_day/harib27d/mmldata/daiku.mml ${root_SOURCE_DIR}/projects/30_day/harib27d/mmldata/fujisan.mml ${root_SOURCE_DIR}/projects/30_day/harib27d/mmldata/kirakira.mml ${root_SOURCE_DIR}/projects/30_day/harib27d/pictdata/fujisan.jpg ${root_SOURCE_DIR}/projects/30_day/harib27d/pictdata/night.bmp
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(30_day_harib27d_APIO
  ${root_BINARY_DIR}/projects/30_day/harib27d/api001.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api002.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api003.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api004.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api005.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api006.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api007.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api008.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api009.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api010.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api011.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api012.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api013.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api014.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api015.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api016.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api017.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api018.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api019.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api020.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api021.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api022.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api023.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api024.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api025.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api026.o
  ${root_BINARY_DIR}/projects/30_day/harib27d/api027.o)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/bmp.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/gview/bmp.nasm ${root_BINARY_DIR}/projects/30_day/harib27d/bmp.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/gview/bmp.nasm
  COMMENT "Assembling bmp.nasm for 30_day/harib27d")
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/gview.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -I ${root_SOURCE_DIR}/projects/30_day/harib27d/haribote -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/gview/gview.c ${root_SOURCE_DIR}/projects/30_day/harib27d/gview/jpeg.c ${30_day_harib27d_APIO} ${root_BINARY_DIR}/projects/30_day/harib27d/bmp.o ${root_BINARY_DIR}/projects/30_day/harib27d/applib/strtol.o ${root_BINARY_DIR}/projects/30_day/harib27d/libgo.o ${root_BINARY_DIR}/projects/30_day/harib27d/rand.o -o ${root_BINARY_DIR}/projects/30_day/harib27d/gview.hrb
  DEPENDS ${root_BINARY_DIR}/projects/30_day/harib27d/applib/strtol.o ${root_SOURCE_DIR}/projects/30_day/harib27d/gview/gview.c ${root_SOURCE_DIR}/projects/30_day/harib27d/gview/jpeg.c ${30_day_harib27d_APPLDS} ${30_day_harib27d_APIO} ${root_BINARY_DIR}/projects/30_day/harib27d/bmp.o ${30_day_harib27d_LIBGC}
  COMMENT "Generating gview.hrb for 30_day_harib27d")
set(30_day_harib27d_RANDO ${root_BINARY_DIR}/projects/30_day/harib27d/rand.o)
set(30_day_harib27d_STRTOLO ${root_BINARY_DIR}/projects/30_day/harib27d/applib/strtol.o)
add_custom_command(
  OUTPUT ${30_day_harib27d_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${30_day_harib27d_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/applib/strtol.o
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -I ${root_SOURCE_DIR}/golibc -x c -c ${root_SOURCE_DIR}/golibc/strtol.c -o ${root_BINARY_DIR}/projects/30_day/harib27d/applib/strtol.o
  DEPENDS ${root_SOURCE_DIR}/golibc/strtol.c
  COMMENT "Generating strtol.o for 30_day_harib27d")
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api001.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api001.nas
  COMMENT "Generating api001.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api002.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api002.nas
  COMMENT "Generating api002.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api003.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api003.nas
  COMMENT "Generating api003.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api004.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api004.nas
  COMMENT "Generating api004.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api005.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api005.nas
  COMMENT "Generating api005.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api006.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api006.nas
  COMMENT "Generating api006.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api007.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api007.nas
  COMMENT "Generating api007.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api008.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api008.nas
  COMMENT "Generating api008.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api009.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api009.nas
  COMMENT "Generating api009.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api010.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api010.nas
  COMMENT "Generating api010.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api011.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api011.nas
  COMMENT "Generating api011.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api012.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api012.nas
  COMMENT "Generating api012.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api013.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api013.nas
  COMMENT "Generating api013.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api014.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api014.nas
  COMMENT "Generating api014.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api015.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api015.nas
  COMMENT "Generating api015.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api016.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api016.nas
  COMMENT "Generating api016.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api017.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api017.nas
  COMMENT "Generating api017.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api018.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api018.nas
  COMMENT "Generating api018.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api019.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api019.nas
  COMMENT "Generating api019.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api020.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api020.nas
  COMMENT "Generating api020.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api021.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api021.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api021.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api021.nas
  COMMENT "Generating api021.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api022.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api022.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api022.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api022.nas
  COMMENT "Generating api022.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api023.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api023.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api023.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api023.nas
  COMMENT "Generating api023.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api024.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api024.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api024.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api024.nas
  COMMENT "Generating api024.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api025.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api025.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api025.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api025.nas
  COMMENT "Generating api025.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api026.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api026.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api026.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api026.nas
  COMMENT "Generating api026.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/api027.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api027.nas ${root_BINARY_DIR}/projects/30_day/harib27d/api027.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/apilib/api027.nas
  COMMENT "Generating api027.o for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/a/a.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating a.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/hello3/hello3.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating hello3.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/hello4/hello4.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating hello4.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/winhelo/winhelo.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating winhelo.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/winhelo2/winhelo2.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating winhelo2.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/winhelo3/winhelo3.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating winhelo3.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/star1/star1.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating star1.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/stars/stars.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating stars.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/stars2/stars2.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating stars2.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/lines/lines.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating lines.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/walk/walk.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating walk.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/mmlplay.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/mmlplay/mmlplay.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/mmlplay.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/mmlplay/mmlplay.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating mmlplay.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/tview.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/tview/tview.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/tview.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/tview/tview.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating tview.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/calc.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/calc/calc.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/calc.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/calc/calc.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating calc.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/invader.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/invader/invader.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/invader.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/invader/invader.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating invader.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/bball.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/bball/bball.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/bball.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/bball/bball.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating bball.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/notrec.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/notrec/notrec.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/notrec.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/notrec/notrec.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating notrec.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/chklang.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/chklang/chklang.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/chklang.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/chklang/chklang.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating chklang.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/iroha.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/iroha/iroha.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/iroha.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/iroha/iroha.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating iroha.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/type.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/type/type.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/type.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/type/type.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating type.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/sosu3/sosu3.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating sosu3.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/sosu2/sosu2.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating sosu2.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/sosu/sosu.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating sosu.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/color2/color2.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating color2.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/color/color.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating color.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/beepdown/beepdown.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating beepdown.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27d -T ${30_day_harib27d_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27d/noodle/noodle.c ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27d/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27d_APIO} ${30_day_harib27d_STRTOLO} ${30_day_harib27d_LIBGC} ${30_day_harib27d_RANDO}
  COMMENT "Generating noodle.hrb for 30_day_harib27d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27d/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27d/hello5/hello5.nas ${root_BINARY_DIR}/projects/30_day/harib27d/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27d/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 30_day_harib27d"
)
