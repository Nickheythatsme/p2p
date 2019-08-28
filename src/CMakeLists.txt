cmake_minimum_required(VERSION 3.9)

add_library(p2pcrypto STATIC
        crypto/sha256.cpp
        )

add_library(p2putil SHARED
        util/uuid.cpp
        util/logging.cpp
        )

add_library(p2pmain SHARED
        util/uuid.cpp
        crypto/sha256.cpp
        peer_node.cpp
        record_table.cpp
        record.cpp
        )