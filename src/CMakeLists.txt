cmake_minimum_required(VERSION 3.9)

add_library(p2pcrypto STATIC
        crypto/sha256.cpp
        )

add_library(p2putil SHARED
        util/uuid.cpp
        )

add_library(p2pmain SHARED
        peer_node.cpp
        record_table.cpp
        record.cpp
        )