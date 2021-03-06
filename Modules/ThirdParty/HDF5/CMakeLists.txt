project(ITKHDF5)
set(ITKHDF5_THIRD_PARTY 1)

option(ITK_USE_SYSTEM_HDF5 "Use an outside build of HDF5." OFF)
mark_as_advanced(ITK_USE_SYSTEM_HDF5)

if(ITK_USE_SYSTEM_HDF5)
  find_package(HDF5 REQUIRED C CXX)
  set(ITKHDF5_SYSTEM_INCLUDE_DIRS
    ${ITKHDF5_BINARY_DIR}/src # itk_hdf5.h and itk_H5Cpp.h
    ${HDF5_INCLUDE_DIRS}
    )
  set(ITKHDF5_LIBRARIES ${HDF5_LIBRARIES})
  set(ITKHDF5_NO_SRC 1)
else()
  set(ITKHDF5_INCLUDE_DIRS
    ${ITKHDF5_SOURCE_DIR}/src
    ${ITKHDF5_BINARY_DIR}/src
    )
  set(ITKHDF5_LIBRARIES itkhdf5_cpp itkhdf5)
endif()

configure_file(src/itk_hdf5.h.in src/itk_hdf5.h)
configure_file(src/itk_H5Cpp.h.in src/itk_H5Cpp.h)

itk_module_impl()

install(FILES ${ITKHDF5_BINARY_DIR}/src/itk_hdf5.h
              ${ITKHDF5_BINARY_DIR}/src/itk_H5Cpp.h
  DESTINATION ${ITKHDF5_INSTALL_INCLUDE_DIR})
