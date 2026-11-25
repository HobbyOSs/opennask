#----------------------------------------------------------
message(STATUS "Entering directory projects/30_day/harib27b/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(30_day_harib27b_OS    ${root_BINARY_DIR}/projects/30_day/harib27b/os.img)
set(30_day_harib27b_SYS	  ${root_BINARY_DIR}/projects/30_day/harib27b/os.sys)
set(30_day_harib27b_IPLB  ${root_BINARY_DIR}/projects/30_day/harib27b/ipl.bin)
set(30_day_harib27b_IPLS  ${root_SOURCE_DIR}/projects/30_day/harib27b/haribote/ipl20.nas)
set(30_day_harib27b_HEADB ${root_BINARY_DIR}/projects/30_day/harib27b/asmhead.bin)
set(30_day_harib27b_HEADS ${root_SOURCE_DIR}/projects/30_day/harib27b/haribote/asmhead.nas)
set(30_day_harib27b_BOOTB ${root_BINARY_DIR}/projects/30_day/harib27b/boot.bin)
set(30_day_harib27b_CCO	  ${root_BINARY_DIR}/projects/30_day/harib27b/boot.o)
file(GLOB 30_day_harib27b_CCS ${root_SOURCE_DIR}/projects/30_day/harib27b/haribote/*.c)
set(30_day_harib27b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(30_day_harib27b_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(30_day_harib27b_FUNCS ${root_SOURCE_DIR}/projects/30_day/harib27b/haribote/naskfunc.nas)
set(30_day_harib27b_FUNCO ${root_BINARY_DIR}/projects/30_day/harib27b/naskfunc.o)
set(30_day_harib27b_FONTS ${root_SOURCE_DIR}/projects/30_day/harib27b/haribote/hankaku.txt)
set(30_day_harib27b_FONTB ${root_BINARY_DIR}/projects/30_day/harib27b/hankaku.bin)
set(30_day_harib27b_FONTO ${root_BINARY_DIR}/projects/30_day/harib27b/hankaku.o)
set(30_day_harib27b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(30_day_harib27b_LIBGC ${root_BINARY_DIR}/projects/30_day/harib27b/libgo.o)
set(30_day_harib27b_WILDOBJ ${root_BINARY_DIR}/projects/30_day/harib27b/*.o)


add_custom_target(30_day_harib27b_run
  COMMAND ${QEMU} ${QEMUOPT} ${30_day_harib27b_OS}
  DEPENDS 30_day_harib27b_img
)
add_custom_target(30_day_harib27b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${30_day_harib27b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 30_day_harib27b_img
)
add_custom_target(30_day_harib27b_clean
  COMMAND rm -f ${30_day_harib27b_OS}
  COMMAND rm -f ${30_day_harib27b_SYS}
  COMMAND rm -f ${30_day_harib27b_IPLB}
  COMMAND rm -f ${30_day_harib27b_HEADB}
  COMMAND rm -f ${30_day_harib27b_BOOTB}
  COMMAND rm -f ${30_day_harib27b_WILDOBJ}
)
add_custom_target(30_day_harib27b_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27b_IPLS} ${30_day_harib27b_IPLB}
  DEPENDS ${30_day_harib27b_IPLS}
)
add_custom_command(
  OUTPUT ${30_day_harib27b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27b_HEADS} ${30_day_harib27b_HEADB}
  DEPENDS ${30_day_harib27b_HEADS}
  COMMENT "Generating asmhead.bin for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${30_day_harib27b_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27b_FUNCS} ${30_day_harib27b_FUNCO}
  DEPENDS ${30_day_harib27b_FUNCS}
  COMMENT "Generating naskfunc.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${30_day_harib27b_FONTO}
  COMMAND ${FONT} ${30_day_harib27b_FONTS} ${30_day_harib27b_FONTB}
  COMMAND ${B2O}  ${30_day_harib27b_FONTB} ${30_day_harib27b_FONTO} _hankaku
  DEPENDS ${30_day_harib27b_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${30_day_harib27b_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${30_day_harib27b_LIBGE} ${30_day_harib27b_LIBGC}
  DEPENDS ${30_day_harib27b_LIBGE}
  COMMENT "Converting golibc for 30_day_harib27b"
)

add_custom_target(30_day_harib27b_sys
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -T ${30_day_harib27b_LDS} ${30_day_harib27b_CCS} ${30_day_harib27b_WILDOBJ} -o ${30_day_harib27b_BOOTB}
  COMMAND cat ${30_day_harib27b_HEADB} ${30_day_harib27b_BOOTB} > ${30_day_harib27b_SYS}
  DEPENDS ${30_day_harib27b_HEADB} 30_day_harib27b_ipl
  DEPENDS ${30_day_harib27b_FUNCO}
  DEPENDS ${30_day_harib27b_FONTO} ${30_day_harib27b_LIBGC}
)
add_custom_target(30_day_harib27b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${30_day_harib27b_IPLB} -i ${30_day_harib27b_OS}
  COMMAND mcopy -i ${30_day_harib27b_OS} ${30_day_harib27b_SYS} ::
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/a.hrb ::a.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/tview.hrb ::tview.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/calc.hrb ::calc.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/invader.hrb ::invader.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/bball.hrb ::bball.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/notrec.hrb ::notrec.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/chklang.hrb ::chklang.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/iroha.hrb ::iroha.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/type.hrb ::type.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/color.hrb ::color.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_BINARY_DIR}/projects/30_day/harib27b/hello5.hrb ::hello5.hrb
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_SOURCE_DIR}/projects/30_day/harib27b/euc.txt ::euc.txt
  COMMAND mcopy -i ${30_day_harib27b_OS} ${root_SOURCE_DIR}/projects/30_day/harib27b/nihongo/nihongo.fnt ::nihongo.fnt
  DEPENDS 30_day_harib27b_sys ${root_BINARY_DIR}/projects/30_day/harib27b/a.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/hello3.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/hello4.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/winhelo.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/winhelo2.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/winhelo3.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/star1.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/stars.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/stars2.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/lines.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/walk.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/tview.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/calc.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/invader.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/bball.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/notrec.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/chklang.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/iroha.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/type.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/sosu3.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/sosu2.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/sosu.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/noodle.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/beepdown.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/color.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/color2.hrb ${root_BINARY_DIR}/projects/30_day/harib27b/hello5.hrb ${root_SOURCE_DIR}/projects/30_day/harib27b/nihongo/nihongo.fnt ${root_SOURCE_DIR}/projects/30_day/harib27b/euc.txt
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(30_day_harib27b_APIO
  ${root_BINARY_DIR}/projects/30_day/harib27b/api001.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api002.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api003.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api004.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api005.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api006.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api007.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api008.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api009.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api010.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api011.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api012.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api013.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api014.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api015.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api016.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api017.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api018.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api019.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api020.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api021.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api022.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api023.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api024.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api025.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api026.o
  ${root_BINARY_DIR}/projects/30_day/harib27b/api027.o)
set(30_day_harib27b_RANDO ${root_BINARY_DIR}/projects/30_day/harib27b/rand.o)
set(30_day_harib27b_STRTOLO ${root_BINARY_DIR}/projects/30_day/harib27b/applib/strtol.o)
add_custom_command(
  OUTPUT ${30_day_harib27b_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${30_day_harib27b_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/applib/strtol.o
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -I ${root_SOURCE_DIR}/golibc -x c -c ${root_SOURCE_DIR}/golibc/strtol.c -o ${root_BINARY_DIR}/projects/30_day/harib27b/applib/strtol.o
  DEPENDS ${root_SOURCE_DIR}/golibc/strtol.c
  COMMENT "Generating strtol.o for 30_day_harib27b")
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api001.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api001.nas
  COMMENT "Generating api001.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api002.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api002.nas
  COMMENT "Generating api002.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api003.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api003.nas
  COMMENT "Generating api003.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api004.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api004.nas
  COMMENT "Generating api004.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api005.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api005.nas
  COMMENT "Generating api005.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api006.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api006.nas
  COMMENT "Generating api006.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api007.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api007.nas
  COMMENT "Generating api007.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api008.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api008.nas
  COMMENT "Generating api008.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api009.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api009.nas
  COMMENT "Generating api009.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api010.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api010.nas
  COMMENT "Generating api010.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api011.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api011.nas
  COMMENT "Generating api011.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api012.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api012.nas
  COMMENT "Generating api012.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api013.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api013.nas
  COMMENT "Generating api013.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api014.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api014.nas
  COMMENT "Generating api014.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api015.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api015.nas
  COMMENT "Generating api015.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api016.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api016.nas
  COMMENT "Generating api016.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api017.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api017.nas
  COMMENT "Generating api017.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api018.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api018.nas
  COMMENT "Generating api018.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api019.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api019.nas
  COMMENT "Generating api019.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api020.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api020.nas
  COMMENT "Generating api020.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api021.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api021.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api021.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api021.nas
  COMMENT "Generating api021.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api022.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api022.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api022.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api022.nas
  COMMENT "Generating api022.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api023.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api023.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api023.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api023.nas
  COMMENT "Generating api023.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api024.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api024.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api024.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api024.nas
  COMMENT "Generating api024.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api025.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api025.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api025.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api025.nas
  COMMENT "Generating api025.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api026.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api026.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api026.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api026.nas
  COMMENT "Generating api026.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/api027.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api027.nas ${root_BINARY_DIR}/projects/30_day/harib27b/api027.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/apilib/api027.nas
  COMMENT "Generating api027.o for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/a/a.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating a.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/hello3/hello3.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating hello3.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/hello4/hello4.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating hello4.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/winhelo/winhelo.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating winhelo.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/winhelo2/winhelo2.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating winhelo2.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/winhelo3/winhelo3.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating winhelo3.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/star1/star1.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating star1.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/stars/stars.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating stars.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/stars2/stars2.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating stars2.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/lines/lines.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating lines.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/walk/walk.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating walk.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/tview.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/tview/tview.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/tview.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/tview/tview.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating tview.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/calc.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/calc/calc.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/calc.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/calc/calc.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating calc.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/invader.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/invader/invader.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/invader.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/invader/invader.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating invader.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/bball.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/bball/bball.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/bball.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/bball/bball.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating bball.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/notrec.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/notrec/notrec.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/notrec.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/notrec/notrec.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating notrec.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/chklang.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/chklang/chklang.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/chklang.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/chklang/chklang.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating chklang.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/iroha.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/iroha/iroha.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/iroha.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/iroha/iroha.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating iroha.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/type.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/type/type.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/type.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/type/type.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating type.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/sosu3/sosu3.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating sosu3.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/sosu2/sosu2.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating sosu2.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/sosu/sosu.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating sosu.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/color2/color2.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating color2.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/color/color.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating color.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/beepdown/beepdown.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating beepdown.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27b -T ${30_day_harib27b_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27b/noodle/noodle.c ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27b/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27b_APIO} ${30_day_harib27b_STRTOLO} ${30_day_harib27b_LIBGC} ${30_day_harib27b_RANDO}
  COMMENT "Generating noodle.hrb for 30_day_harib27b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27b/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27b/hello5/hello5.nas ${root_BINARY_DIR}/projects/30_day/harib27b/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27b/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 30_day_harib27b"
)
