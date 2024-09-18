cmake_minimum_required(VERSION 3.5)

# Устанавливаем имя проекта для подпроекта
set(PARENT_PROJECT_NAME ${PROJECT_NAME}.Common)

###############################################
add_subdirectory(hash_creator)

add_subdirectory(db)