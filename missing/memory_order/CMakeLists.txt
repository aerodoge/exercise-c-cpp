cmake_minimum_required(VERSION 4.0) project(memory_order)

    set(CMAKE_CXX_STANDARD 20)

        add_executable(memory_order_relaxed relaxed.cpp)
            add_executable(memory_order_acquire acquire.cpp)
                add_executable(memory_order_release release.cpp)
                    add_executable(memory_order_acq_rel acq_rel.cpp)
                        add_executable(memory_order_seq_cst seq_cst.cpp)
                            add_executable(memory_order_consume consume.cpp)
