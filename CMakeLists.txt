# Need CMake 3.28.0
cmake_minimum_required (VERSION 3.28.0)

# This is the name of the repository
project ("rogue")

# Set the correct paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set debug mode
# TODO: Remove this before release
add_compile_definitions(NDEBUG)

# Add lots of compiler warnings
add_compile_options(-gdwarf-4 -Wall -Wextra -Wpedantic -Wpointer-arith -Wstrict-prototypes -Wformat-security -Wfloat-equal -Wshadow -Wconversion -Wlogical-not-parentheses -Wnull-dereference)

# Find the array module
if ( NOT "${HAS_ARRAY}")

    # Add the array directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/array ${CMAKE_SOURCE_DIR}/build/array)

    # Let any parent project know that this project has array
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_ARRAY true PARENT_SCOPE)
        set(ARRAY_INCLUDE_DIR ${ARRAY_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the dict module
if ( NOT "${HAS_DICT}")

    # Add the dict directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/dict ${CMAKE_SOURCE_DIR}/build/dict)

    # Let any parent project know that this project has dict
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_DICT true PARENT_SCOPE)
        set(DICT_INCLUDE_DIR ${DICT_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the crypto module
if ( NOT "${HAS_CRYPTO}")

    # Add the crypto directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/crypto ${CMAKE_SOURCE_DIR}/build/crypto)

    # Let any parent project know that this project has crypto
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_CRYPTO true PARENT_SCOPE)
        set(CRYPTO_INCLUDE_DIR ${CRYPTO_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the tuple module
if ( NOT "${HAS_TUPLE}")
    
    # Add the tuple directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/tuple ${CMAKE_SOURCE_DIR}/build/tuple)

    # Let any parent project know that this project has tuple
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_TUPLE true PARENT_SCOPE)
        set(TUPLE_INCLUDE_DIR ${TUPLE_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()
 
# Find the set module
if ( NOT "${HAS_SET}")
    
    # Add the set directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/set ${CMAKE_SOURCE_DIR}/build/set)

    # Let any parent project know that this project has set
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_SET true PARENT_SCOPE)
        set(SET_INCLUDE_DIR ${SET_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the stack module
if ( NOT "${HAS_STACK}")

    # Add the stack directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/stack ${CMAKE_SOURCE_DIR}/build/stack)

    # Let any parent project know that this project has stack
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_STACK true PARENT_SCOPE)
        set(STACK_INCLUDE_DIR ${STACK_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the queue module
if ( NOT "${HAS_QUEUE}")
    
    # Add the queue directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/queue ${CMAKE_SOURCE_DIR}/build/queue)

    # Let any parent project know that this project has queue
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_QUEUE true PARENT_SCOPE)
        set(QUEUE_INCLUDE_DIR ${QUEUE_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the json module
if ( NOT "${HAS_JSON}")
    
    # Add the json directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/json ${CMAKE_SOURCE_DIR}/build/json)

    # Let any parent project know that this project has json
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_JSON true PARENT_SCOPE)
        set(JSON_INCLUDE_DIR ${JSON_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Add the rogue project
if ( NOT "${HAS_ROGUE}")
    
    # Log
    message("[rogue] Adding rogue project")

    # Set the g10 include
    set(ROGUE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

    # Let any parent project know that it has the g10 project
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_ROGUE true PARENT_SCOPE)
        set(ROGUE_INCLUDE_DIR ${ROGUE_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# rogue executable
add_executable (rogue "main.c" "stamp.c" "wav.c") 
add_dependencies(rogue log sync crypto array dict tuple stack queue json)
target_include_directories(rogue PUBLIC ${ROGUE_INCLUDE_DIR} ${ARRAY_INCLUDE_DIR} ${CRYPTO_INCLUDE_DIR} ${DICT_INCLUDE_DIR} ${JSON_INCLUDE_DIR} ${LOG_INCLUDE_DIR} ${QUEUE_INCLUDE_DIR} ${STACK_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${TUPLE_INCLUDE_DIR}) 
target_link_libraries(rogue PUBLIC log sync crypto array dict tuple stack queue json m)
