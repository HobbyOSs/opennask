#----------------------------------------------------------
message(STATUS "Entering directory projects/02_day/helloos4/")

########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/02_day/helloos4/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/02_day/helloos4/ipl.img)
set(ipl_wine.img_OUTS ${root_BINARY_DIR}/projects/02_day/helloos4/ipl_wine.img)

add_custom_target(02_day_helloos4_ipl
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl.img_SRCS} ${ipl.img_OUTS}
)
add_custom_target(02_day_helloos4_ipl_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl.img_SRCS} ${ipl_wine.img_OUTS}
  COMMAND ${OD} -t x1 ${ipl.img_OUTS}      > ${ipl.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl_wine.img_OUTS} > ${ipl_wine.img_OUTS}_t.txt
)

add_dependencies(images 02_day_helloos4_ipl)
add_dependencies(wine 02_day_helloos4_ipl_wine)

