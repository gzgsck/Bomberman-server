cmake_minimum_required(VERSION 3.8)
project(PROJECTBOOM)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp sources/Map.cpp json_spirit/json_spirit_writer.cpp json_spirit/json_spirit_writer.h headers/Map.h sources/Bomb.cpp headers/Bomb.h sources/Point.cpp headers/Point.h sources/Cell.cpp headers/Cell.h sources/Player.cpp headers/Player.h sources/Obstacle.cpp headers/Obstacle.h sources/DestroyableObstacle.cpp headers/DestroyableObstacle.h sources/UndestroyableObstacle.cpp headers/UndestroyableObstacle.h utils/Configuration.h utils/Serializer.cpp utils/Serializer.h)
add_executable(PROJECTBOOM ${SOURCE_FILES})

