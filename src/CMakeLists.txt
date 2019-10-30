cmake_minimum_required(VERSION 3.9)

add_library(p2putil SHARED
        util/uuid.cpp
        util/key.cpp
        crypto/sha256.cpp
        networking/listener.cpp
        )

add_library(p2pmain SHARED
        util/uuid.cpp
        util/key.cpp
        crypto/sha256.cpp
        record.cpp
        node.cpp
        )
