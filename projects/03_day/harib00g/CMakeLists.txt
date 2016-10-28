#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00g/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(03_day_harib00g_OS    ${root_BINARY_DIR}/projects/03_day/harib00g/os.img)
set(03_day_harib00g_SYS	  ${root_BINARY_DIR}/projects/03_day/harib00g/os.sys)
set(03_day_harib00g_IPLB  ${root_BINARY_DIR}/projects/03_day/harib00g/ipl.bin)
set(03_day_harib00g_IPLS  ${root_SOURCE_DIR}/projects/03_day/harib00g/ipl10.nas)
set(03_day_harib00g_HEADB ${root_BINARY_DIR}/projects/03_day/harib00g/asmhead.bin)
set(03_day_harib00g_HEADS ${root_SOURCE_DIR}/projects/03_day/harib00g/asmhead.nas)
set(03_day_harib00g_BOOTB ${root_BINARY_DIR}/projects/03_day/harib00g/boot.bin)
set(03_day_harib00g_CCO	  ${root_BINARY_DIR}/projects/03_day/harib00g/boot.o)
set(03_day_harib00g_CCS	  ${root_SOURCE_DIR}/projects/03_day/harib00g/*.c)

add_custom_target(03_day_harib00g_haribote_run                                           
  COMMAND ${QEMU} ${QEMUOPT} ${03_day_harib00g_OS}				
)											
add_custom_target(03_day_harib00g_haribote_debug						
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${03_day_harib00g_OS} -redir tcp:5555:127.0.0.1:1234 &
)											
add_custom_target(03_day_harib00g_haribote_ipl						
  COMMAND ${NASK} ${03_day_harib00g_IPLS} ${03_day_harib00g_IPLB}		
)											
add_custom_target(03_day_harib00g_haribote_sys						
  COMMAND ${NASK} ${03_day_harib00g_HEADS} ${03_day_harib00g_HEADB}		
  COMMAND gcc ${03_day_harib00g_CCS}  -c -o ${03_day_harib00g_CCO}	
  COMMAND ld -o ${03_day_harib00g_BOOTB} -e _HariMain ${03_day_harib00g_CCO} ${03_day_harib00g_FUNCO}	
  COMMAND cat ${03_day_harib00g_HEADB} ${03_day_harib00g_BOOTB} > ${03_day_harib00g_SYS}			
)											
add_custom_target(03_day_harib00g_haribote_img						
  COMMAND mformat -f 1440 -C -B ${03_day_harib00g_IPLB} -i ${03_day_harib00g_OS}	
  COMMAND mcopy ${03_day_harib00g_SYS} -i ${03_day_harib00g_OS}			
)											
########### next target ###############
set(haribote.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00g/haribote.nas)
set(haribote.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00g/haribote.img)
set(haribote_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00g/haribote_wine.img)

add_custom_target(03_day_harib00g_haribote
  COMMAND ${root_BINARY_DIR}/src/opennask ${haribote.img_SRCS} ${haribote.img_OUTS}
)
add_custom_target(03_day_harib00g_haribote_wine
  COMMAND ${WINE} ${WINE_NASK} ${haribote.img_SRCS} ${haribote_wine.img_OUTS}
)
add_custom_target(03_day_harib00g_haribote_od
  COMMAND ${OD} -t x1 ${haribote.img_OUTS}      > ${haribote.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${haribote_wine.img_OUTS} > ${haribote_wine.img_OUTS}_t.txt
)

add_dependencies(images 03_day_harib00g_haribote)
add_dependencies(wine 03_day_harib00g_haribote_wine)
add_dependencies(od 03_day_harib00g_haribote_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00g/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00g/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00g/ipl10_wine.img)

add_custom_target(03_day_harib00g_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(03_day_harib00g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(03_day_harib00g_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 03_day_harib00g_ipl10)
add_dependencies(wine 03_day_harib00g_ipl10_wine)
add_dependencies(od 03_day_harib00g_ipl10_od)

#----------------------------------------------------------
