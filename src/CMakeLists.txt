## 生成动态库 供他人时候
set(SRC_LIST ./Log.cpp ./TimeStamp.cpp ./Buffer.cpp ./Channel.cpp

./Poller.cpp ./DefaultPoller.cpp ./EPoller.cpp ./EventLoop.cpp

./CurrentThread.cpp)

add_library(mymuduo SHARED ${SRC_LIST})

target_include_directories(mymuduo PUBLIC ../include)

target_compile_options(mymuduo PUBLIC -Wall -Werror -std=c++17)

target_compile_definitions(mymuduo PUBLIC -DDB)