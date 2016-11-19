
#pragma once

#include <vector>
#include <memory>

namespace editor
{
    class PolygonEntity;
    class PathEntity;

    std::vector<std::shared_ptr<editor::PolygonEntity>> LoadPolygons(const char* file_name);
    void SavePolygons(const char* file_name, const std::vector<std::shared_ptr<editor::PolygonEntity>>& polygons);

    std::vector<std::shared_ptr<editor::PathEntity>> LoadPaths(const char* file_name);
    void SavePaths(const std::vector<std::shared_ptr<editor::PathEntity>>& paths);
}
