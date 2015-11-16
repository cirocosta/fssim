include_directories(${fssim_INCLUDE_DIRS})

file(GLOB_RECURSE exps "*.c")
foreach(file ${exps})

  get_filename_component(expname ${file} NAME_WE)
  add_executable(${expname} ${file})
  target_link_libraries(${expname} ${fssim_LIBRARIES})
endforeach()
