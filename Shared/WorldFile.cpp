//
//  WorldFile.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 21/07/16.
//
//

#include "WorldFile.h"
#include <cstdio>


bool world::WriteWorld(File::FilePtr& file, const LevelFileHeader& level)
{
    std::fwrite(&level.version, sizeof(int), 1, file.get());

    const int n_polygons = static_cast<int>(level.polygons.size());
    std::fwrite(&n_polygons, sizeof(int), 1, file.get());

    for(int index = 0; index < n_polygons; ++index)
    {
        const world::PolygonData& polygon = level.polygons[index];
        const int n_vertices = static_cast<int>(polygon.vertices.size());

        std::fwrite(&polygon.position,          sizeof(math::Vector2f), 1,          file.get());
        std::fwrite(&polygon.local_offset,      sizeof(math::Vector2f), 1,          file.get());
        std::fwrite(&polygon.rotation,          sizeof(float),          1,          file.get());
        std::fwrite(&n_vertices,                sizeof(int),            1,          file.get());
        std::fwrite(polygon.vertices.data(),    sizeof(math::Vector2f), n_vertices, file.get());
    }

    return true;
}

bool world::ReadWorld(File::FilePtr& file, LevelFileHeader& level)
{
    std::vector<byte> bytes;
    File::FileRead(file, bytes);

    int offset = 0;

    std::memcpy(&level.version,     bytes.data() + offset, sizeof(int));
    offset += sizeof(int);

    int n_polygons = 0;
    std::memcpy(&n_polygons,        bytes.data() + offset, sizeof(int));
    offset += sizeof(int);

    level.polygons.resize(n_polygons);

    for(int index = 0; index < n_polygons; ++index)
    {
        world::PolygonData& polygon = level.polygons[index];

        std::memcpy(&polygon.position, bytes.data() + offset, sizeof(math::Vector2f));
        offset += sizeof(math::Vector2f);

        std::memcpy(&polygon.local_offset, bytes.data() + offset, sizeof(math::Vector2f));
        offset += sizeof(math::Vector2f);

        std::memcpy(&polygon.rotation, bytes.data() + offset, sizeof(float));
        offset += sizeof(float);

        int n_vertices = 0;
        std::memcpy(&n_vertices,        bytes.data() + offset, sizeof(int));
        offset += sizeof(int);

        polygon.vertices.resize(n_vertices);

        std::memcpy(polygon.vertices.data(), bytes.data() + offset, sizeof(math::Vector2f) * n_vertices);
        offset += sizeof(math::Vector2f) * n_vertices;
    }

    return true;
}
