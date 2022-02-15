cmake_minimum_required(VERSION 3.10)

# set the project name
project(kinc)

function(fblobRecurse out pathTo)
    file(GLOB_RECURSE tempout
        ${PROJECT_SOURCE_DIR}/${pathTo}/*.hpp
        ${PROJECT_SOURCE_DIR}/${pathTo}/*.h
        ${PROJECT_SOURCE_DIR}/${pathTo}/*.cpp
        ${PROJECT_SOURCE_DIR}/${pathTo}/*.c
    )
    set(${out} ${tempout} PARENT_SCOPE)
endfunction(fblobRecurse out pathTo)

fblobRecurse(KINC_FILES Sources)

# WindowsApps aren't supposed to have static stuff
# for OS specifics.
set(KINC_LIB_TYPE "STATIC")


if(${KINC_PLATFORM} STREQUAL "WindowsApp")
# set(KINC_LIB_TYPE "SHARED")
endif(${KINC_PLATFORM} STREQUAL "WindowsApp")

add_library(Kinc ${KINC_LIB_TYPE} ${KINC_FILES})
target_compile_features(Kinc PUBLIC cxx_std_17)
target_compile_definitions(Kinc PUBLIC
    KORE_G1
    KORE_G2
    KORE_G3
    KORE_A1
    KORE_A2
    KORE_A3
)
target_include_directories(Kinc PUBLIC Sources)

set(KINC_PLATFORM "Windows" CACHE STRING "Target Platform")
set_property(CACHE KINC_PLATFORM PROPERTY STRINGS
    Windows
    WindowsApp
    iOS
    OSX
    Android
    Linux
    HTML5
    Tizen
    Pi
    tvOS
    PS4
    XboxOne
    Switch
    XboxScarlett
    PS5
    FreeBSD
)

set(KINC_GRAPHICS_API "Default" CACHE STRING "Graphics API")
set_property(CACHE KINC_GRAPHICS_API PROPERTY STRINGS
    Default
    OpenGL
    OpenGL1
    Direct3D9
    Direct3D11
    Direct3D12
    Metal
    Vulkan
    WebGPU
)

set(KINC_AUDIO_API "Default" CACHE STRING "Audio API")
set_property(CACHE KINC_AUDIO_API PROPERTY STRINGS
    Default
    DirectSound
    WASAPI
)

set(KINC_VR_API "None" CACHE STRING "VR API")
set_property(CACHE KINC_VR_API PROPERTY STRINGS
    GearVr
    Cardboard
    Oculus
    SteamVR
    HoloLens
    None
)

function(addBackend backendName)
    fblobRecurse(BACKENDFILES Backends/${backendName}/Sources)

    target_include_directories(Kinc PRIVATE 
        "${PROJECT_SOURCE_DIR}/Backends/${backendName}/Sources/"
    )

    target_sources(Kinc PRIVATE ${BACKENDFILES})
endfunction(addBackend backendName)

message("Targeting for ${KINC_PLATFORM}")
if(${KINC_PLATFORM} STREQUAL "Windows")
    target_compile_definitions(Kinc PUBLIC
        KORE_WINDOWS
        KORE_MICROSOFT
    )
    addBackend("System/Windows")
    addBackend("System/Microsoft")
    target_compile_definitions(Kinc PRIVATE
        _CRT_SECURE_NO_WARNINGS
        _WINSOCK_DEPRECATED_NO_WARNINGS
    )
    fblobRecurse(DIRECTSHOW_FILES Backends/System/Windows/Libraries/DirectShow)
    target_sources(Kinc PRIVATE ${DIRECTSHOW_FILES})
    target_include_directories(Kinc PUBLIC 
        "${PROJECT_SOURCE_DIR}/Backends/System/Windows/Libraries/DirectShow/BaseClasses"
    )
    target_link_libraries(Kinc
        PUBLIC
        dxguid
        dsound
        dinput8
        ws2_32
        Winhttp
        strmiids
        winmm
        wbemuuid
    )
    
    if(${KINC_GRAPHICS_API} STREQUAL "OpenGL1")
        addBackend("Graphics3/OpenGL1")
        target_compile_definitions(Kinc PUBLIC
            KORE_OPENGL1
            GLEW_STATIC
        )
    elseif(${KINC_GRAPHICS_API} STREQUAL "OpenGL")
        set(G4 TRUE)
        addBackend("Graphics4/OpenGL")
        target_compile_definitions(Kinc PUBLIC
            KORE_OPENGL
            GLEW_STATIC
        )
    elseif((${KINC_GRAPHICS_API} STREQUAL "Direct3D11") OR (${KINC_GRAPHICS_API} STREQUAL "Default"))
        set(G4 TRUE)
        addBackend("Graphics4/Direct3D11")
        target_compile_definitions(Kinc PUBLIC
            KORE_DIRECT3D
            KORE_DIRECT3D11
        )
        target_link_libraries(Kinc
            PUBLIC
            d3d11
        )
    elseif(${KINC_GRAPHICS_API} STREQUAL "Direct3D12")
        set(G4 TRUE)
        set(G5 TRUE)
        addBackend("Graphics5/Direct3D12")
        target_compile_definitions(Kinc PUBLIC
            KORE_DIRECT3D
            KORE_DIRECT3D12
        )
        target_link_libraries(Kinc
            PUBLIC
            dxgi
            d3d12
        )
    elseif(${KINC_GRAPHICS_API} STREQUAL "Vulkan")
        set(G4 TRUE)
        set(G5 TRUE)
        addBackend("Graphics5/Vulkan")
        target_compile_definitions(Kinc PUBLIC
            KORE_VULKAN
            VK_USE_PLATFORM_WIN32_KHR
        )
        find_package(Vulkan REQUIRED)
        if(NOT VULKAN_FOUND)
            message(FATAL_ERROR "Vulkan not found!")
        endif(NOT VULKAN_FOUND)
        
        target_link_libraries(Kinc
            PUBLIC
            ${Vulkan_LIBRARIES}
        )
        target_include_directories(Kinc PRIVATE 
            $Vulkan_INCLUDE_DIRS}
        )
    elseif(${KINC_GRAPHICS_API} STREQUAL "Direct3D9")
        set(G4 TRUE)
        addBackend("Graphics3/Direct3D9")
        target_compile_definitions(Kinc PUBLIC
            KORE_DIRECT3D
            KORE_DIRECT3D9
        )
        
        target_link_libraries(Kinc
            PUBLIC
            d3d9
        )
    else()
        message(FATAL_ERROR "Graphics API ${KINC_GRAPHICS_API} is not available on windows")
    endif(${KINC_GRAPHICS_API} STREQUAL "OpenGL1")
    
    if(${KINC_AUDIO_API} STREQUAL "DirectSound")
        addBackend("Audio2/DirectSound")
    elseif((${KINC_AUDIO_API} STREQUAL "WASAPI") OR (${KINC_AUDIO_API} STREQUAL "Default"))
        addBackend("Audio2/WASAPI")
    endif(${KINC_AUDIO_API} STREQUAL "DirectSound")
    
    if(${KINC_VR_API} STREQUAL "Oculus")
        target_compile_definitions(Kinc PUBLIC
            KORE_VR
            KORE_OCULUS
        )
        fblobRecurse(OCULUS_SDK_GL Backends/System/Windows/Libraries/OculusSDK/LibOVRKernel/Src/GL)
        target_sources(Kinc PRIVATE OCULUS_SDK_GL)
        target_include_directories(Kinc PRIVATE
            "${PROJECT_SOURCE_DIR}/Backends/System/Windows/Libraries/OculusSDK/LibOVR/Include/"
            "${PROJECT_SOURCE_DIR}/Backends/System/Windows/Libraries/OculusSDK/LibOVRKernel/Src/"
        )
        target_link_libraries(Kinc PUBLIC
            "${PROJECT_SOURCE_DIR}/Backends/System/Windows/Libraries/OculusSDK/LibOVR/Lib/Windows/x64/Release/VS2017/LibOVR"
        )
    elseif(${KINC_VR_API} STREQUAL "SteamVR")
        target_compile_definitions(Kinc PUBLIC
            KORE_VR
            KORE_STEAMVR
            VR_API_PUBLIC
        )
        fblobRecurse(STEAMVR_SRC Backends/System/Windows/Libraries/SteamVR/src)
        target_sources(Kinc PRIVATE STEAMVR_SRC)
        target_include_directories(Kinc PRIVATE
            "${PROJECT_SOURCE_DIR}/Backends/System/Windows/Libraries/SteamVR/src"
            "${PROJECT_SOURCE_DIR}/Backends/System/Windows/Libraries/SteamVR/src/vrcommon"
            "${PROJECT_SOURCE_DIR}/Backends/System/Windows/Libraries/SteamVR/headers"
        )
    elseif(${KINC_VR_API} STREQUAL "None")
    else()
        message(FATAL_ERROR "VR API ${KINC_VR_API} is not available on windows!")
    endif(${KINC_VR_API} STREQUAL "Oculus")
    
elseif(${KINC_PLATFORM} STREQUAL "WindowsApp")
    set(G4 TRUE)
    target_compile_definitions(Kinc PUBLIC
        KORE_WINDOWSAPP
        KORE_WINRT
        KORE_MICROSOFT
    )
    addBackend("System/WindowsApp")
    addBackend("System/Microsoft")
    addBackend("Graphics4/Direct3D11")
    addBackend("Audio2/WASAPI") 

    target_compile_definitions(Kinc PRIVATE
        _CRT_SECURE_NO_WARNINGS
    )
    target_compile_options(Kinc PRIVATE "/ZW")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -AI\"C:/Program Files/Microsoft Visual Studio/2022/Preview/Common7/IDE/VC/vcpackages\"" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -AI\"C:/Program Files (x86)/Windows Kits/10/UnionMetadata/10.0.19041.0\"" )
    add_link_options("/MD" "/WINMD" "/WHOLEARCHIVE")
    set(CMAKE_SYSTEM_NAME "WindowsStore")
    message("${CMAKE_SYSTEM_VERSION}")
else()
    set(KINC_PROP_PLUGIN TRUE)
    set(G4 TRUE)
    set(G5 TRUE)
endif(${KINC_PLATFORM} STREQUAL "Windows")

if(${G4})
    target_compile_definitions(Kinc PUBLIC KORE_G4)
else()
    
endif(${G4})

if(${G5})
    target_compile_definitions(Kinc PUBLIC KORE_G5 KORE_G4ONG5)
    addBackend("Graphics4/G4onG5")
else()
    target_compile_definitions(Kinc PUBLIC KORE_G5 KORE_G5ONG4)
    addBackend("Graphics5/G5onG4")
endif(${G5})

addBackend("Audio3/A3onA2")