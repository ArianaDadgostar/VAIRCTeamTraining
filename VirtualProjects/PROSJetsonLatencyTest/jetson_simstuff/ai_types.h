#pragma once
#include <stdint.h>
#include <stddef.h>

struct Detection {
    int32_t classId;
    float   confidence;
    float   distance;
    int32_t bbox[4];
    float   position[3];
};

struct PosRecord {
    int32_t framecnt;
    int32_t status;
    float   x, y, z;
    float   az, el, rot;
};

struct AI_RECORD {
    PosRecord pos;
    int32_t   detectionCount;
    Detection detections[8];
};

struct MapPacket {
    uint8_t   sync[4];
    uint16_t  length;
    uint16_t  type;
    uint32_t  crc;
    AI_RECORD map;
};

struct AckPacket {
    uint8_t  sync[4];
    int32_t  framecnt;
    int64_t  v5_rx_us;
    uint32_t crc;
};

inline uint32_t calc_crc(const uint8_t* data, size_t len) {
    uint32_t c = 0xFFFFFFFF;
    for (size_t i = 0; i < len; i++) {
        c ^= data[i];
        for (int b = 0; b < 8; b++)
            c = (c >> 1) ^ (0xEDB88320u & -(c & 1u));
    }
    return ~c;
}