#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00d/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(03_day_harib00d_OS    ${root_BINARY_DIR}/projects/03_day/harib00d/os.img)
set(03_day_harib00d_SYS	  ${root_BINARY_DIR}/projects/03_day/harib00d/os.sys)
set(03_day_harib00d_IPLB  ${root_BINARY_DIR}/projects/03_day/harib00d/ipl.bin)
set(03_day_harib00d_IPLS  ${root_SOURCE_DIR}/projects/03_day/harib00d/ipl10.nas)
set(03_day_harib00d_HEADB ${root_BINARY_DIR}/projects/03_day/harib00d/asmhead.bin)
set(03_day_harib00d_HEADS ${root_SOURCE_DIR}/projects/03_day/harib00d/asmhead.nas)
set(03_day_harib00d_BOOTB ${root_BINARY_DIR}/projects/03_day/harib00d/boot.bin)
set(03_day_harib00d_CCO	  ${root_BINARY_DIR}/projects/03_day/harib00d/boot.o)
set(03_day_harib00d_CCS	  ${root_SOURCE_DIR}/projects/03_day/harib00d/*.c)

add_custom_target(03_day_harib00d_ipl_run                                           
  COMMAND ${QEMU} ${QEMUOPT} ${03_day_harib00d_OS}				
)											
add_custom_target(03_day_harib00d_ipl_debug						
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${03_day_harib00d_OS} -redir tcp:5555:127.0.0.1:1234 &
)											
add_custom_target(03_day_harib00d_ipl_ipl						
  COMMAND ${NASK} ${03_day_harib00d_IPLS} ${03_day_harib00d_IPLB}		
)											
add_custom_target(03_day_harib00d_ipl_sys						
  COMMAND ${NASK} ${03_day_harib00d_HEADS} ${03_day_harib00d_HEADB}		
  COMMAND gcc ${03_day_harib00d_CCS}  -c -o ${03_day_harib00d_CCO}	
  COMMAND ld -o ${03_day_harib00d_BOOTB} -e _HariMain ${03_day_harib00d_CCO} ${03_day_harib00d_FUNCO}	
  COMMAND cat ${03_day_harib00d_HEADB} ${03_day_harib00d_BOOTB} > ${03_day_harib00d_SYS}			
)											
add_custom_target(03_day_harib00d_ipl_img						
  COMMAND mformat -f 1440 -C -B ${03_day_harib00d_IPLB} -i ${03_day_harib00d_OS}	
  COMMAND mcopy ${03_day_harib00d_SYS} -i ${03_day_harib00d_OS}			
)											
########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00d/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00d/ipl.img)
set(ipl_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00d/ipl_wine.img)

add_custom_target(03_day_harib00d_ipl
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl.img_SRCS} ${ipl.img_OUTS}
)
add_custom_target(03_day_harib00d_ipl_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl.img_SRCS} ${ipl_wine.img_OUTS}
)
add_custom_target(03_day_harib00d_ipl_od
  COMMAND ${OD} -t x1 ${ipl.img_OUTS}      > ${ipl.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl_wine.img_OUTS} > ${ipl_wine.img_OUTS}_t.txt
)

add_dependencies(images 03_day_harib00d_ipl)
add_dependencies(wine 03_day_harib00d_ipl_wine)
add_dependencies(od 03_day_harib00d_ipl_od)

#----------------------------------------------------------
