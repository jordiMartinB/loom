file(GLOB_RECURSE transitmap_SRC *.cpp)

set(transitmap_main TransitMap.cpp)

list(REMOVE_ITEM transitmap_SRC ${transitmap_main})
list(REMOVE_ITEM transitmap_SRC TestMain.cpp)

include_directories(
	${LOOM_INCLUDE_DIR}
)

add_subdirectory(output/protobuf)

configure_file (
  "_config.h.in"
  "_config.h"
)

# Build object library so sources compile to object files that can be linked into
# a single library later.
add_library(transitmap_objects OBJECT ${transitmap_SRC})
set_target_properties(transitmap_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(transitmap_objects PUBLIC ${LOOM_INCLUDE_DIR})

# If protobuf is used, ensure proto is built before the object compilation
if (Protobuf_FOUND AND Protobuf_PROTOC_EXECUTABLE)
    add_dependencies(transitmap_objects proto)
endif()
