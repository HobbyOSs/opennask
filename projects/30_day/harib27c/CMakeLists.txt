#----------------------------------------------------------
message(STATUS "Entering directory projects/30_day/harib27c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(30_day_harib27c_OS    ${root_BINARY_DIR}/projects/30_day/harib27c/os.img)
set(30_day_harib27c_SYS	  ${root_BINARY_DIR}/projects/30_day/harib27c/os.sys)
set(30_day_harib27c_IPLB  ${root_BINARY_DIR}/projects/30_day/harib27c/ipl.bin)
set(30_day_harib27c_IPLS  ${root_SOURCE_DIR}/projects/30_day/harib27c/haribote/ipl20.nas)
set(30_day_harib27c_HEADB ${root_BINARY_DIR}/projects/30_day/harib27c/asmhead.bin)
set(30_day_harib27c_HEADS ${root_SOURCE_DIR}/projects/30_day/harib27c/haribote/asmhead.nas)
set(30_day_harib27c_BOOTB ${root_BINARY_DIR}/projects/30_day/harib27c/boot.bin)
set(30_day_harib27c_CCO	  ${root_BINARY_DIR}/projects/30_day/harib27c/boot.o)
file(GLOB 30_day_harib27c_CCS ${root_SOURCE_DIR}/projects/30_day/harib27c/haribote/*.c)
set(30_day_harib27c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(30_day_harib27c_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(30_day_harib27c_FUNCS ${root_SOURCE_DIR}/projects/30_day/harib27c/haribote/naskfunc.nas)
set(30_day_harib27c_FUNCO ${root_BINARY_DIR}/projects/30_day/harib27c/naskfunc.o)
set(30_day_harib27c_FONTS ${root_SOURCE_DIR}/projects/30_day/harib27c/haribote/hankaku.txt)
set(30_day_harib27c_FONTB ${root_BINARY_DIR}/projects/30_day/harib27c/hankaku.bin)
set(30_day_harib27c_FONTO ${root_BINARY_DIR}/projects/30_day/harib27c/hankaku.o)
set(30_day_harib27c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(30_day_harib27c_LIBGC ${root_BINARY_DIR}/projects/30_day/harib27c/libgo.o)
set(30_day_harib27c_WILDOBJ ${root_BINARY_DIR}/projects/30_day/harib27c/*.o)


add_custom_target(30_day_harib27c_run
  COMMAND ${QEMU} ${QEMUOPT} ${30_day_harib27c_OS}
  DEPENDS 30_day_harib27c_img
)
add_custom_target(30_day_harib27c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${30_day_harib27c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 30_day_harib27c_img
)
add_custom_target(30_day_harib27c_clean
  COMMAND rm -f ${30_day_harib27c_OS}
  COMMAND rm -f ${30_day_harib27c_SYS}
  COMMAND rm -f ${30_day_harib27c_IPLB}
  COMMAND rm -f ${30_day_harib27c_HEADB}
  COMMAND rm -f ${30_day_harib27c_BOOTB}
  COMMAND rm -f ${30_day_harib27c_WILDOBJ}
)
add_custom_target(30_day_harib27c_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27c_IPLS} ${30_day_harib27c_IPLB}
  DEPENDS ${30_day_harib27c_IPLS}
)
add_custom_command(
  OUTPUT ${30_day_harib27c_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27c_HEADS} ${30_day_harib27c_HEADB}
  DEPENDS ${30_day_harib27c_HEADS}
  COMMENT "Generating asmhead.bin for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${30_day_harib27c_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27c_FUNCS} ${30_day_harib27c_FUNCO}
  DEPENDS ${30_day_harib27c_FUNCS}
  COMMENT "Generating naskfunc.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${30_day_harib27c_FONTO}
  COMMAND ${FONT} ${30_day_harib27c_FONTS} ${30_day_harib27c_FONTB}
  COMMAND ${B2O}  ${30_day_harib27c_FONTB} ${30_day_harib27c_FONTO} _hankaku
  DEPENDS ${30_day_harib27c_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${30_day_harib27c_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${30_day_harib27c_LIBGE} ${30_day_harib27c_LIBGC}
  DEPENDS ${30_day_harib27c_LIBGE}
  COMMENT "Converting golibc for 30_day_harib27c"
)

add_custom_target(30_day_harib27c_sys
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -T ${30_day_harib27c_LDS} ${30_day_harib27c_CCS} ${30_day_harib27c_WILDOBJ} -o ${30_day_harib27c_BOOTB}
  COMMAND cat ${30_day_harib27c_HEADB} ${30_day_harib27c_BOOTB} > ${30_day_harib27c_SYS}
  DEPENDS ${30_day_harib27c_HEADB} 30_day_harib27c_ipl
  DEPENDS ${30_day_harib27c_FUNCO}
  DEPENDS ${30_day_harib27c_FONTO} ${30_day_harib27c_LIBGC}
)
add_custom_target(30_day_harib27c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${30_day_harib27c_IPLB} -i ${30_day_harib27c_OS}
  COMMAND mcopy -i ${30_day_harib27c_OS} ${30_day_harib27c_SYS} ::
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/a.hrb ::a.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/mmlplay.hrb ::mmlplay.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/tview.hrb ::tview.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/calc.hrb ::calc.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/invader.hrb ::invader.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/bball.hrb ::bball.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/notrec.hrb ::notrec.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/chklang.hrb ::chklang.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/iroha.hrb ::iroha.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/type.hrb ::type.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/color.hrb ::color.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_BINARY_DIR}/projects/30_day/harib27c/hello5.hrb ::hello5.hrb
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_SOURCE_DIR}/projects/30_day/harib27c/euc.txt ::euc.txt
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_SOURCE_DIR}/projects/30_day/harib27c/mmldata/kirakira.mml ::kirakira.mml
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_SOURCE_DIR}/projects/30_day/harib27c/mmldata/fujisan.mml ::fujisan.mml
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_SOURCE_DIR}/projects/30_day/harib27c/mmldata/daiku.mml ::daiku.mml
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_SOURCE_DIR}/projects/30_day/harib27c/mmldata/daigo.mml ::daigo.mml
  COMMAND mcopy -i ${30_day_harib27c_OS} ${root_SOURCE_DIR}/projects/30_day/harib27c/nihongo/nihongo.fnt ::nihongo.fnt
  DEPENDS 30_day_harib27c_sys ${root_BINARY_DIR}/projects/30_day/harib27c/a.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/hello3.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/hello4.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/winhelo.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/winhelo2.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/winhelo3.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/star1.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/stars.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/stars2.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/lines.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/walk.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/mmlplay.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/tview.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/calc.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/invader.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/bball.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/notrec.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/chklang.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/iroha.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/type.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/sosu3.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/sosu2.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/sosu.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/noodle.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/beepdown.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/color.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/color2.hrb ${root_BINARY_DIR}/projects/30_day/harib27c/hello5.hrb ${root_SOURCE_DIR}/projects/30_day/harib27c/nihongo/nihongo.fnt ${root_SOURCE_DIR}/projects/30_day/harib27c/euc.txt ${root_SOURCE_DIR}/projects/30_day/harib27c/mmldata/daigo.mml ${root_SOURCE_DIR}/projects/30_day/harib27c/mmldata/daiku.mml ${root_SOURCE_DIR}/projects/30_day/harib27c/mmldata/fujisan.mml ${root_SOURCE_DIR}/projects/30_day/harib27c/mmldata/kirakira.mml
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(30_day_harib27c_APIO
  ${root_BINARY_DIR}/projects/30_day/harib27c/api001.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api002.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api003.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api004.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api005.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api006.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api007.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api008.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api009.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api010.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api011.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api012.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api013.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api014.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api015.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api016.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api017.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api018.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api019.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api020.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api021.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api022.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api023.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api024.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api025.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api026.o
  ${root_BINARY_DIR}/projects/30_day/harib27c/api027.o)
set(30_day_harib27c_RANDO ${root_BINARY_DIR}/projects/30_day/harib27c/rand.o)
set(30_day_harib27c_STRTOLO ${root_BINARY_DIR}/projects/30_day/harib27c/applib/strtol.o)
add_custom_command(
  OUTPUT ${30_day_harib27c_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${30_day_harib27c_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/applib/strtol.o
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -I ${root_SOURCE_DIR}/golibc -x c -c ${root_SOURCE_DIR}/golibc/strtol.c -o ${root_BINARY_DIR}/projects/30_day/harib27c/applib/strtol.o
  DEPENDS ${root_SOURCE_DIR}/golibc/strtol.c
  COMMENT "Generating strtol.o for 30_day_harib27c")
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api001.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api001.nas
  COMMENT "Generating api001.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api002.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api002.nas
  COMMENT "Generating api002.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api003.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api003.nas
  COMMENT "Generating api003.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api004.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api004.nas
  COMMENT "Generating api004.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api005.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api005.nas
  COMMENT "Generating api005.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api006.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api006.nas
  COMMENT "Generating api006.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api007.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api007.nas
  COMMENT "Generating api007.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api008.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api008.nas
  COMMENT "Generating api008.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api009.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api009.nas
  COMMENT "Generating api009.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api010.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api010.nas
  COMMENT "Generating api010.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api011.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api011.nas
  COMMENT "Generating api011.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api012.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api012.nas
  COMMENT "Generating api012.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api013.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api013.nas
  COMMENT "Generating api013.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api014.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api014.nas
  COMMENT "Generating api014.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api015.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api015.nas
  COMMENT "Generating api015.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api016.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api016.nas
  COMMENT "Generating api016.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api017.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api017.nas
  COMMENT "Generating api017.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api018.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api018.nas
  COMMENT "Generating api018.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api019.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api019.nas
  COMMENT "Generating api019.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api020.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api020.nas
  COMMENT "Generating api020.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api021.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api021.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api021.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api021.nas
  COMMENT "Generating api021.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api022.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api022.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api022.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api022.nas
  COMMENT "Generating api022.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api023.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api023.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api023.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api023.nas
  COMMENT "Generating api023.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api024.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api024.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api024.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api024.nas
  COMMENT "Generating api024.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api025.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api025.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api025.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api025.nas
  COMMENT "Generating api025.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api026.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api026.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api026.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api026.nas
  COMMENT "Generating api026.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/api027.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api027.nas ${root_BINARY_DIR}/projects/30_day/harib27c/api027.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/apilib/api027.nas
  COMMENT "Generating api027.o for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/a/a.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating a.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/hello3/hello3.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating hello3.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/hello4/hello4.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating hello4.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/winhelo/winhelo.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating winhelo.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/winhelo2/winhelo2.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating winhelo2.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/winhelo3/winhelo3.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating winhelo3.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/star1/star1.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating star1.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/stars/stars.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating stars.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/stars2/stars2.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating stars2.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/lines/lines.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating lines.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/walk/walk.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating walk.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/mmlplay.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/mmlplay/mmlplay.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/mmlplay.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/mmlplay/mmlplay.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating mmlplay.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/tview.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/tview/tview.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/tview.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/tview/tview.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating tview.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/calc.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/calc/calc.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/calc.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/calc/calc.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating calc.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/invader.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/invader/invader.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/invader.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/invader/invader.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating invader.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/bball.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/bball/bball.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/bball.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/bball/bball.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating bball.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/notrec.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/notrec/notrec.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/notrec.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/notrec/notrec.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating notrec.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/chklang.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/chklang/chklang.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/chklang.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/chklang/chklang.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating chklang.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/iroha.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/iroha/iroha.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/iroha.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/iroha/iroha.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating iroha.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/type.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/type/type.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/type.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/type/type.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating type.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/sosu3/sosu3.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating sosu3.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/sosu2/sosu2.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating sosu2.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/sosu/sosu.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating sosu.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/color2/color2.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating color2.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/color/color.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating color.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/beepdown/beepdown.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating beepdown.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27c -T ${30_day_harib27c_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27c/noodle/noodle.c ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27c/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27c_APIO} ${30_day_harib27c_STRTOLO} ${30_day_harib27c_LIBGC} ${30_day_harib27c_RANDO}
  COMMENT "Generating noodle.hrb for 30_day_harib27c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27c/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27c/hello5/hello5.nas ${root_BINARY_DIR}/projects/30_day/harib27c/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27c/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 30_day_harib27c"
)
