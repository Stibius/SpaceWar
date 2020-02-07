cmake_minimum_required (VERSION 3.8)

# solution
project(SpaceWar)
# project
set(APP_NAME "SpaceWar") 

find_path(SDL_INCLUDE_DIR SDL.h)
find_path(SDL_mixer_INCLUDE_DIR SDL_mixer.h)
find_path(SDL_ttf_INCLUDE_DIR SDL_ttf.h)

find_library(SDL_LIBRARY_RELEASE NAMES SDL) 
find_library(SDLmain_LIBRARY_RELEASE NAMES SDLmain) 
find_library(SDL_mixer_LIBRARY_RELEASE NAMES SDL_mixer) 
find_library(SDL_ttf_LIBRARY_RELEASE NAMES SDL_ttf) 

find_library(SDL_LIBRARY_DEBUG NAMES SDL) 
find_library(SDLmain_LIBRARY_DEBUG NAMES SDLmain) 
find_library(SDL_mixer_LIBRARY_DEBUG NAMES SDL_mixer) 
find_library(SDL_ttf_LIBRARY_DEBUG NAMES SDL_ttf) 

set(HEADER_FILES
   src/Circle.h
   src/Collision_System.h
   src/Color.h 
   src/Configurable.h
   src/Drawable.h
   src/Events.h
   src/Font.h 
   src/Game.h
   src/Game_Interface.h 
   src/Math.h
   src/Menu.h
   src/Moving_Object.h
   src/Object.h 
   src/Player.h 
   src/Point.h 
   src/Powerup_System.h
   src/Projectile.h 
   src/Settings.h 
   src/Sound.h
   src/Star.h
   src/Texture.h   
   src/Updatable.h  
)

set(SOURCE_FILES
   src/Circle.cpp
   src/Collision_System.cpp
   src/Color.cpp 
   src/Font.cpp
   src/Game.cpp
   src/Game_Interface.cpp
   src/main.cpp 
   src/Math.cpp
   src/Menu.cpp
   src/Moving_Object.cpp
   src/Object.cpp 
   src/Player.cpp 
   src/Powerup_System.cpp
   src/Projectile.cpp 
   src/Settings.cpp
   src/Sound.cpp
   src/Star.cpp
   src/Texture.cpp   
)

# Define a grouping for source files in IDE project generation
source_group("Source Files" FILES ${SOURCE_FILES})

# Define a grouping for source files in IDE project generation
source_group("Header Files" FILES ${HEADER_FILES})

add_executable(${APP_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${APP_NAME} PUBLIC ${SDL_INCLUDE_DIR}
                                       PUBLIC ${SDL_mixer_INCLUDE_DIR}
                                       PUBLIC ${SDL_ttf_INCLUDE_DIR})                                     

target_link_libraries(${APP_NAME} debug ${SDL_LIBRARY_DEBUG}
                                  debug ${SDLmain_LIBRARY_DEBUG}
                                  debug ${SDL_mixer_LIBRARY_DEBUG}
                                  debug ${SDL_ttf_LIBRARY_DEBUG}
                                  optimized ${SDL_LIBRARY_RELEASE}
                                  optimized ${SDLmain_LIBRARY_RELEASE}
                                  optimized ${SDL_mixer_LIBRARY_RELEASE}
                                  optimized ${SDL_ttf_LIBRARY_RELEASE}
                                  opengl32
                                  )