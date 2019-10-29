cmake_minimum_required(VERSION 3.9)

add_library(p2pcrypto STATIC
        crypto/sha256.cpp
        )

add_library(p2putil SHARED
        util/uuid.cpp
        util/key.cpp
        )

add_library(p2pnetworking SHARED
        networking/listener.cpp
        )

add_library(p2pmain SHARED
        util/uuid.cpp
        crypto/sha256.cpp
        node.cpp
        record.cpp
        )
