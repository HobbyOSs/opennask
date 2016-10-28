#----------------------------------------------------------
message(STATUS "Entering directory projects/02_day/helloos5/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(02_day_helloos5_OS    ${root_BINARY_DIR}/projects/02_day/helloos5/os.img)
set(02_day_helloos5_SYS	  ${root_BINARY_DIR}/projects/02_day/helloos5/os.sys)
set(02_day_helloos5_IPLB  ${root_BINARY_DIR}/projects/02_day/helloos5/ipl.bin)
set(02_day_helloos5_IPLS  ${root_SOURCE_DIR}/projects/02_day/helloos5/ipl10.nas)
set(02_day_helloos5_HEADB ${root_BINARY_DIR}/projects/02_day/helloos5/asmhead.bin)
set(02_day_helloos5_HEADS ${root_SOURCE_DIR}/projects/02_day/helloos5/asmhead.nas)
set(02_day_helloos5_BOOTB ${root_BINARY_DIR}/projects/02_day/helloos5/boot.bin)
set(02_day_helloos5_CCO	  ${root_BINARY_DIR}/projects/02_day/helloos5/boot.o)
set(02_day_helloos5_CCS	  ${root_SOURCE_DIR}/projects/02_day/helloos5/*.c)

add_custom_target(02_day_helloos5_ipl_run                                           
  COMMAND ${QEMU} ${QEMUOPT} ${02_day_helloos5_OS}				
)											
add_custom_target(02_day_helloos5_ipl_debug						
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${02_day_helloos5_OS} -redir tcp:5555:127.0.0.1:1234 &
)											
add_custom_target(02_day_helloos5_ipl_ipl						
  COMMAND ${NASK} ${02_day_helloos5_IPLS} ${02_day_helloos5_IPLB}		
)											
add_custom_target(02_day_helloos5_ipl_sys						
  COMMAND ${NASK} ${02_day_helloos5_HEADS} ${02_day_helloos5_HEADB}		
  COMMAND gcc ${02_day_helloos5_CCS}  -c -o ${02_day_helloos5_CCO}	
  COMMAND ld -o ${02_day_helloos5_BOOTB} -e _HariMain ${02_day_helloos5_CCO} ${02_day_helloos5_FUNCO}	
  COMMAND cat ${02_day_helloos5_HEADB} ${02_day_helloos5_BOOTB} > ${02_day_helloos5_SYS}			
)											
add_custom_target(02_day_helloos5_ipl_img						
  COMMAND mformat -f 1440 -C -B ${02_day_helloos5_IPLB} -i ${02_day_helloos5_OS}	
  COMMAND mcopy ${02_day_helloos5_SYS} -i ${02_day_helloos5_OS}			
)											
########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/02_day/helloos5/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/02_day/helloos5/ipl.img)
set(ipl_wine.img_OUTS ${root_BINARY_DIR}/projects/02_day/helloos5/ipl_wine.img)

add_custom_target(02_day_helloos5_ipl
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl.img_SRCS} ${ipl.img_OUTS}
)
add_custom_target(02_day_helloos5_ipl_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl.img_SRCS} ${ipl_wine.img_OUTS}
)
add_custom_target(02_day_helloos5_ipl_od
  COMMAND ${OD} -t x1 ${ipl.img_OUTS}      > ${ipl.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl_wine.img_OUTS} > ${ipl_wine.img_OUTS}_t.txt
)

add_dependencies(images 02_day_helloos5_ipl)
add_dependencies(wine 02_day_helloos5_ipl_wine)
add_dependencies(od 02_day_helloos5_ipl_od)

#----------------------------------------------------------
