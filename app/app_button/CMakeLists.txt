project(APP_BUTTON)

add_library(lib_app_button STATIC)
target_sources(lib_app_button 
    PRIVATE
    ./src/app_button.c
)
target_include_directories(lib_app_button
    PUBLIC   
    ./inc
)
target_link_libraries(lib_app_button
    PRIVATE
    lib_lwbtn
    lib_shell  
    lib_threadx
    lib_bsp
)