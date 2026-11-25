#----------------------------------------------------------
message(STATUS "Entering directory projects/30_day/harib27a/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(30_day_harib27a_OS    ${root_BINARY_DIR}/projects/30_day/harib27a/os.img)
set(30_day_harib27a_SYS	  ${root_BINARY_DIR}/projects/30_day/harib27a/os.sys)
set(30_day_harib27a_IPLB  ${root_BINARY_DIR}/projects/30_day/harib27a/ipl.bin)
set(30_day_harib27a_IPLS  ${root_SOURCE_DIR}/projects/30_day/harib27a/haribote/ipl20.nas)
set(30_day_harib27a_HEADB ${root_BINARY_DIR}/projects/30_day/harib27a/asmhead.bin)
set(30_day_harib27a_HEADS ${root_SOURCE_DIR}/projects/30_day/harib27a/haribote/asmhead.nas)
set(30_day_harib27a_BOOTB ${root_BINARY_DIR}/projects/30_day/harib27a/boot.bin)
set(30_day_harib27a_CCO	  ${root_BINARY_DIR}/projects/30_day/harib27a/boot.o)
file(GLOB 30_day_harib27a_CCS ${root_SOURCE_DIR}/projects/30_day/harib27a/haribote/*.c)
set(30_day_harib27a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(30_day_harib27a_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(30_day_harib27a_FUNCS ${root_SOURCE_DIR}/projects/30_day/harib27a/haribote/naskfunc.nas)
set(30_day_harib27a_FUNCO ${root_BINARY_DIR}/projects/30_day/harib27a/naskfunc.o)
set(30_day_harib27a_FONTS ${root_SOURCE_DIR}/projects/30_day/harib27a/haribote/hankaku.txt)
set(30_day_harib27a_FONTB ${root_BINARY_DIR}/projects/30_day/harib27a/hankaku.bin)
set(30_day_harib27a_FONTO ${root_BINARY_DIR}/projects/30_day/harib27a/hankaku.o)
set(30_day_harib27a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(30_day_harib27a_LIBGC ${root_BINARY_DIR}/projects/30_day/harib27a/libgo.o)
set(30_day_harib27a_WILDOBJ ${root_BINARY_DIR}/projects/30_day/harib27a/*.o)


add_custom_target(30_day_harib27a_run
  COMMAND ${QEMU} ${QEMUOPT} ${30_day_harib27a_OS}
  DEPENDS 30_day_harib27a_img
)
add_custom_target(30_day_harib27a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${30_day_harib27a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 30_day_harib27a_img
)
add_custom_target(30_day_harib27a_clean
  COMMAND rm -f ${30_day_harib27a_OS}
  COMMAND rm -f ${30_day_harib27a_SYS}
  COMMAND rm -f ${30_day_harib27a_IPLB}
  COMMAND rm -f ${30_day_harib27a_HEADB}
  COMMAND rm -f ${30_day_harib27a_BOOTB}
  COMMAND rm -f ${30_day_harib27a_WILDOBJ}
)
add_custom_target(30_day_harib27a_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27a_IPLS} ${30_day_harib27a_IPLB}
  DEPENDS ${30_day_harib27a_IPLS}
)
add_custom_command(
  OUTPUT ${30_day_harib27a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27a_HEADS} ${30_day_harib27a_HEADB}
  DEPENDS ${30_day_harib27a_HEADS}
  COMMENT "Generating asmhead.bin for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${30_day_harib27a_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${30_day_harib27a_FUNCS} ${30_day_harib27a_FUNCO}
  DEPENDS ${30_day_harib27a_FUNCS}
  COMMENT "Generating naskfunc.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${30_day_harib27a_FONTO}
  COMMAND ${FONT} ${30_day_harib27a_FONTS} ${30_day_harib27a_FONTB}
  COMMAND ${B2O}  ${30_day_harib27a_FONTB} ${30_day_harib27a_FONTO} _hankaku
  DEPENDS ${30_day_harib27a_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${30_day_harib27a_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${30_day_harib27a_LIBGE} ${30_day_harib27a_LIBGC}
  DEPENDS ${30_day_harib27a_LIBGE}
  COMMENT "Converting golibc for 30_day_harib27a"
)

add_custom_target(30_day_harib27a_sys
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -T ${30_day_harib27a_LDS} ${30_day_harib27a_CCS} ${30_day_harib27a_WILDOBJ} -o ${30_day_harib27a_BOOTB}
  COMMAND cat ${30_day_harib27a_HEADB} ${30_day_harib27a_BOOTB} > ${30_day_harib27a_SYS}
  DEPENDS ${30_day_harib27a_HEADB} 30_day_harib27a_ipl
  DEPENDS ${30_day_harib27a_FUNCO}
  DEPENDS ${30_day_harib27a_FONTO} ${30_day_harib27a_LIBGC}
)
add_custom_target(30_day_harib27a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${30_day_harib27a_IPLB} -i ${30_day_harib27a_OS}
  COMMAND mcopy -i ${30_day_harib27a_OS} ${30_day_harib27a_SYS} ::
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/a.hrb ::a.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/calc.hrb ::calc.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/invader.hrb ::invader.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/bball.hrb ::bball.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/notrec.hrb ::notrec.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/chklang.hrb ::chklang.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/iroha.hrb ::iroha.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/type.hrb ::type.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/color.hrb ::color.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_BINARY_DIR}/projects/30_day/harib27a/hello5.hrb ::hello5.hrb
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_SOURCE_DIR}/projects/30_day/harib27a/euc.txt ::euc.txt
  COMMAND mcopy -i ${30_day_harib27a_OS} ${root_SOURCE_DIR}/projects/30_day/harib27a/nihongo/nihongo.fnt ::nihongo.fnt
  DEPENDS 30_day_harib27a_sys ${root_BINARY_DIR}/projects/30_day/harib27a/a.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/hello3.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/hello4.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/winhelo.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/winhelo2.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/winhelo3.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/star1.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/stars.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/stars2.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/lines.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/walk.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/calc.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/invader.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/bball.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/notrec.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/chklang.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/iroha.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/type.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/sosu3.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/sosu2.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/sosu.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/noodle.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/beepdown.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/color.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/color2.hrb ${root_BINARY_DIR}/projects/30_day/harib27a/hello5.hrb ${root_SOURCE_DIR}/projects/30_day/harib27a/nihongo/nihongo.fnt ${root_SOURCE_DIR}/projects/30_day/harib27a/euc.txt
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(30_day_harib27a_APIO
  ${root_BINARY_DIR}/projects/30_day/harib27a/api001.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api002.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api003.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api004.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api005.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api006.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api007.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api008.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api009.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api010.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api011.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api012.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api013.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api014.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api015.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api016.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api017.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api018.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api019.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api020.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api021.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api022.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api023.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api024.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api025.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api026.o
  ${root_BINARY_DIR}/projects/30_day/harib27a/api027.o)
set(30_day_harib27a_RANDO ${root_BINARY_DIR}/projects/30_day/harib27a/rand.o)
set(30_day_harib27a_STRTOLO ${root_BINARY_DIR}/projects/30_day/harib27a/applib/strtol.o)
add_custom_command(
  OUTPUT ${30_day_harib27a_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${30_day_harib27a_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/applib/strtol.o
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -I ${root_SOURCE_DIR}/golibc -x c -c ${root_SOURCE_DIR}/golibc/strtol.c -o ${root_BINARY_DIR}/projects/30_day/harib27a/applib/strtol.o
  DEPENDS ${root_SOURCE_DIR}/golibc/strtol.c
  COMMENT "Generating strtol.o for 30_day_harib27a")
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api001.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api001.nas
  COMMENT "Generating api001.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api002.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api002.nas
  COMMENT "Generating api002.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api003.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api003.nas
  COMMENT "Generating api003.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api004.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api004.nas
  COMMENT "Generating api004.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api005.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api005.nas
  COMMENT "Generating api005.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api006.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api006.nas
  COMMENT "Generating api006.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api007.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api007.nas
  COMMENT "Generating api007.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api008.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api008.nas
  COMMENT "Generating api008.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api009.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api009.nas
  COMMENT "Generating api009.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api010.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api010.nas
  COMMENT "Generating api010.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api011.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api011.nas
  COMMENT "Generating api011.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api012.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api012.nas
  COMMENT "Generating api012.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api013.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api013.nas
  COMMENT "Generating api013.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api014.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api014.nas
  COMMENT "Generating api014.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api015.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api015.nas
  COMMENT "Generating api015.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api016.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api016.nas
  COMMENT "Generating api016.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api017.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api017.nas
  COMMENT "Generating api017.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api018.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api018.nas
  COMMENT "Generating api018.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api019.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api019.nas
  COMMENT "Generating api019.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api020.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api020.nas
  COMMENT "Generating api020.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api021.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api021.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api021.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api021.nas
  COMMENT "Generating api021.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api022.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api022.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api022.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api022.nas
  COMMENT "Generating api022.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api023.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api023.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api023.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api023.nas
  COMMENT "Generating api023.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api024.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api024.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api024.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api024.nas
  COMMENT "Generating api024.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api025.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api025.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api025.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api025.nas
  COMMENT "Generating api025.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api026.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api026.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api026.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api026.nas
  COMMENT "Generating api026.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/api027.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api027.nas ${root_BINARY_DIR}/projects/30_day/harib27a/api027.o
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/apilib/api027.nas
  COMMENT "Generating api027.o for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/a/a.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating a.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/hello3/hello3.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating hello3.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/hello4/hello4.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating hello4.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/winhelo/winhelo.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating winhelo.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/winhelo2/winhelo2.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating winhelo2.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/winhelo3/winhelo3.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating winhelo3.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/star1/star1.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating star1.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/stars/stars.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating stars.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/stars2/stars2.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating stars2.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/lines/lines.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating lines.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/walk/walk.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating walk.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/calc.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/calc/calc.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/calc.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/calc/calc.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating calc.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/invader.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/invader/invader.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/invader.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/invader/invader.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating invader.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/bball.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/bball/bball.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/bball.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/bball/bball.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating bball.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/notrec.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/notrec/notrec.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/notrec.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/notrec/notrec.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating notrec.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/chklang.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/chklang/chklang.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/chklang.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/chklang/chklang.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating chklang.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/iroha.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/iroha/iroha.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/iroha.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/iroha/iroha.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating iroha.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/type.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/type/type.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/type.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/type/type.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating type.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/sosu3/sosu3.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating sosu3.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/sosu2/sosu2.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating sosu2.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/sosu/sosu.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating sosu.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/color2/color2.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating color2.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/color/color.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating color.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/beepdown/beepdown.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating beepdown.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/30_day/harib27a -T ${30_day_harib27a_APPLDS} ${root_SOURCE_DIR}/projects/30_day/harib27a/noodle/noodle.c ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO} -o ${root_BINARY_DIR}/projects/30_day/harib27a/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${30_day_harib27a_APIO} ${30_day_harib27a_STRTOLO} ${30_day_harib27a_LIBGC} ${30_day_harib27a_RANDO}
  COMMENT "Generating noodle.hrb for 30_day_harib27a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/30_day/harib27a/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/30_day/harib27a/hello5/hello5.nas ${root_BINARY_DIR}/projects/30_day/harib27a/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/30_day/harib27a/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 30_day_harib27a"
)
