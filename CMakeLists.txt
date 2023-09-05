cmake_minimum_required(VERSION 3.7)

if (ANDROID)
    set(OGG_DIR dependencies/android/libogg)
    set(THEORA_DIR dependencies/android/libtheora)
    add_library(
        libogg
        STATIC
        ${OGG_DIR}/src/bitwise.c
        ${OGG_DIR}/src/framing.c
    )

    add_library(libtheora STATIC
        ${THEORA_DIR}/lib/analyze.c
        ${THEORA_DIR}/lib/apiwrapper.c
        ${THEORA_DIR}/lib/bitpack.c
        ${THEORA_DIR}/lib/cpu.c
        ${THEORA_DIR}/lib/decapiwrapper.c
        ${THEORA_DIR}/lib/decinfo.c
        ${THEORA_DIR}/lib/decode.c
        ${THEORA_DIR}/lib/dequant.c
        ${THEORA_DIR}/lib/encapiwrapper.c
        ${THEORA_DIR}/lib/encfrag.c
        ${THEORA_DIR}/lib/encinfo.c
        ${THEORA_DIR}/lib/encode.c
        ${THEORA_DIR}/lib/encoder_disabled.c
        ${THEORA_DIR}/lib/enquant.c
        ${THEORA_DIR}/lib/fdct.c
        ${THEORA_DIR}/lib/fragment.c
        ${THEORA_DIR}/lib/huffdec.c
        ${THEORA_DIR}/lib/huffenc.c
        ${THEORA_DIR}/lib/idct.c
        ${THEORA_DIR}/lib/info.c
        ${THEORA_DIR}/lib/internal.c
        ${THEORA_DIR}/lib/mathops.c
        ${THEORA_DIR}/lib/mcenc.c
        ${THEORA_DIR}/lib/quant.c
        ${THEORA_DIR}/lib/rate.c
        ${THEORA_DIR}/lib/state.c
        ${THEORA_DIR}/lib/tokenize.c
    )

    target_include_directories(libogg PRIVATE dependencies/android/libogg/include)
    target_include_directories(libtheora PRIVATE dependencies/android/libogg/include dependencies/android/libtheora/include)

    target_compile_options(libogg PRIVATE -ffast-math -fsigned-char -O2 -fPIC -DPIC -DBYTE_ORDER=LITTLE_ENDIAN -D_ARM_ASSEM_ -w)
    target_compile_options(libtheora PRIVATE -ffast-math -fsigned-char -O2 -fPIC -DPIC -DBYTE_ORDER=LITTLE_ENDIAN -D_ARM_ASSEM_ -w)

    add_library(RetroEngine SHARED
        RSDKv5/main.cpp
        RSDKv5/RSDK/Core/RetroEngine.cpp
        RSDKv5/RSDK/Core/Math.cpp
        RSDKv5/RSDK/Core/Reader.cpp
        RSDKv5/RSDK/Core/Link.cpp
        RSDKv5/RSDK/Core/ModAPI.cpp
        RSDKv5/RSDK/Dev/Debug.cpp
        RSDKv5/RSDK/Storage/Storage.cpp
        RSDKv5/RSDK/Storage/Text.cpp
        RSDKv5/RSDK/Graphics/Drawing.cpp
        RSDKv5/RSDK/Graphics/Scene3D.cpp
        RSDKv5/RSDK/Graphics/Animation.cpp
        RSDKv5/RSDK/Graphics/Sprite.cpp
        RSDKv5/RSDK/Graphics/Palette.cpp
        RSDKv5/RSDK/Graphics/Video.cpp
        RSDKv5/RSDK/Audio/Audio.cpp
        RSDKv5/RSDK/Input/Input.cpp
        RSDKv5/RSDK/Scene/Scene.cpp
        RSDKv5/RSDK/Scene/Collision.cpp
        RSDKv5/RSDK/Scene/Object.cpp
        RSDKv5/RSDK/Scene/Objects/DefaultObject.cpp
        RSDKv5/RSDK/Scene/Objects/DevOutput.cpp
        RSDKv5/RSDK/User/Core/UserAchievements.cpp
        RSDKv5/RSDK/User/Core/UserCore.cpp
        RSDKv5/RSDK/User/Core/UserLeaderboards.cpp
        RSDKv5/RSDK/User/Core/UserPresence.cpp
        RSDKv5/RSDK/User/Core/UserStats.cpp
        RSDKv5/RSDK/User/Core/UserStorage.cpp
        dependencies/all/tinyxml2/tinyxml2.cpp
        dependencies/all/iniparser/iniparser.cpp
        dependencies/all/iniparser/dictionary.cpp
        dependencies/all/miniz/miniz.c
        dependencies/android/androidHelpers.cpp
    )

    target_include_directories(RetroEngine PRIVATE
        RSDKv5/
        dependencies/android/libogg/include/
        dependencies/android/libtheora/include/
        dependencies/android/
        dependencies/all/
        dependencies/all/tinyxml2/
        dependencies/all/iniparser/
    )

    target_compile_options(RetroEngine PRIVATE -fexceptions -frtti -DRSDK_USE_GL3 -DRETRO_REVISION=3)

    find_package(games-controller REQUIRED CONFIG)
    find_package(game-activity REQUIRED CONFIG)
    find_package(oboe REQUIRED CONFIG)

    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u GameActivity_onCreate")

    target_link_libraries(
        RetroEngine
        android
        libogg
        libtheora
        EGL
        GLESv3
        log
        z
        jnigraphics
        games-controller::paddleboat_static
        game-activity::game-activity
        oboe::oboe
        OpenSLES
    )
elseif(VITA)
    set(VITASDK_CMAKE_FILE "$ENV{VITASDK}/share/vita.cmake" CACHE PATH "VitaSDK CMake functions file")
    include("${VITASDK_CMAKE_FILE}" REQUIRED)
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffast-math -O3 -fsigned-char -Wall")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math -O3 -fsigned-char -fpermissive -std=c++17")
    set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1 -d ATTRIBUTE2=12")

    add_executable(RetroEngine
      RSDKv5/main.cpp
      RSDKv5/RSDK/Core/RetroEngine.cpp
      RSDKv5/RSDK/Core/Math.cpp
      RSDKv5/RSDK/Core/Reader.cpp
      RSDKv5/RSDK/Core/Link.cpp
      RSDKv5/RSDK/Core/ModAPI.cpp
      RSDKv5/RSDK/Dev/Debug.cpp
      RSDKv5/RSDK/Storage/Storage.cpp
      RSDKv5/RSDK/Storage/Text.cpp
      RSDKv5/RSDK/Graphics/Drawing.cpp
      RSDKv5/RSDK/Graphics/Scene3D.cpp
      RSDKv5/RSDK/Graphics/Animation.cpp
      RSDKv5/RSDK/Graphics/Sprite.cpp
      RSDKv5/RSDK/Graphics/Palette.cpp
      RSDKv5/RSDK/Graphics/Video.cpp
      RSDKv5/RSDK/Audio/Audio.cpp
      RSDKv5/RSDK/Input/Input.cpp
      RSDKv5/RSDK/Scene/Scene.cpp
      RSDKv5/RSDK/Scene/Collision.cpp
      RSDKv5/RSDK/Scene/Object.cpp
      RSDKv5/RSDK/Scene/Objects/DefaultObject.cpp
      RSDKv5/RSDK/Scene/Objects/DevOutput.cpp
      RSDKv5/RSDK/User/Core/UserAchievements.cpp
      RSDKv5/RSDK/User/Core/UserCore.cpp
      RSDKv5/RSDK/User/Core/UserLeaderboards.cpp
      RSDKv5/RSDK/User/Core/UserPresence.cpp
      RSDKv5/RSDK/User/Core/UserStats.cpp
      RSDKv5/RSDK/User/Core/UserStorage.cpp
      dependencies/all/tinyxml2/tinyxml2.cpp
      dependencies/all/iniparser/iniparser.cpp
      dependencies/all/iniparser/dictionary.cpp
      dependencies/all/miniz/miniz.c
    )

    target_include_directories(RetroEngine PRIVATE
      RSDKv5/
      dependencies/android/libogg/include/
      dependencies/android/libtheora/include/
      dependencies/android/
      dependencies/all/
      dependencies/all/tinyxml2/
      dependencies/all/iniparser/
      dependencies/all/thread-pool
      "$ENV{VITASDK}/arm-vita-eabi/include/SDL2"
      "$ENV{VITASDK}/arm-vita-eabi/include/freetype2"
    )

    target_compile_options(RetroEngine PRIVATE -ffast-math -O3 -fsigned-char -fpermissive -std=c++17)
    target_compile_definitions(RetroEngine PUBLIC -DBASE_PATH="ux0:data/Mania/" -DRETRO_REVISION=3 -DRETRO_DISABLE_LOG=1 -D__VITA__)

    target_link_libraries(RetroEngine
      SDL2
      SDL2_image
      vorbisfile
      vorbis
      ogg
      mpg123
      FLAC
      mikmod
      opus
      freetype
      jpeg
      png
      theora
      SceMotion_stub
      SceCommonDialog_stub
      SceHid_stub
      SceGxm_stub
      SceAudio_stub
      SceAudioIn_stub
      ScePower_stub
      SceDisplay_stub
      SceCtrl_stub
      SceTouch_stub
      SceIofilemgr_stub
      SceSsl_stub
      SceSysmodule_stub
      SceNet_stub
      SceNetCtl_stub
      SceLibKernel_stub
      SceNpTrophy_stub
      dl
      taihen_stub
      SceKernelModulemgr_stub
      SceSblSsMgr_stub
      -Wl,--whole-archive pthread -Wl,--no-whole-archive
      ${SDL2_LIBRARIES}
      gcc
      g
      m
    )

    vita_create_self(eboot.bin
      RetroEngine
      UNSAFE
      NOASLR
    )

    set(VPK "SonicMania.vpk")
    set(TITLE_NAME "Sonic Mania")
    set(TITLE_ID "MANIA0001")
    set(TITLE_VER "01.10")

    vita_create_vpk("${VPK}" "${TITLE_ID}" eboot.bin
      NAME "${TITLE_NAME}"
      VERSION "${TITLE_VER}"
      FILE vita/mania_livearea sce_sys
    )

endif()
