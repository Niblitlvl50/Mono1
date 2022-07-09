
#include "Triangulate.h"
#include "delaunay/delaunay.h"

mono::TriangulatedPoints mono::Triangulate(const std::vector<math::Vector>& points)
{
    std::vector<del_point2d_t> delaunay_points;
    delaunay_points.reserve(points.size());

    for(const math::Vector& vertex : points)
        delaunay_points.push_back({vertex.x, vertex.y});

    delaunay2d_t* delaunay_triangulation = delaunay2d_from(delaunay_points.data(), delaunay_points.size());
    tri_delaunay2d_t* triangles = tri_delaunay2d_from(delaunay_triangulation);

    TriangulatedPoints out_triangulated_points;
    
    // Copy back the triangulated data to known data structures.
    {
        out_triangulated_points.vertices.reserve(triangles->num_points);
        out_triangulated_points.triangles.reserve(triangles->num_triangles * 3);

        for(uint32_t index = 0; index < triangles->num_points; ++index)
        {
            del_point2d_t& del_point = triangles->points[index];
            out_triangulated_points.vertices.push_back(math::Vector(del_point.x, del_point.y));
        }

        for(uint32_t index = 0; index < triangles->num_triangles * 3; ++index)
            out_triangulated_points.triangles.push_back(triangles->tris[index]);
    }

    tri_delaunay2d_release(triangles);
    delaunay2d_release(delaunay_triangulation);

    return out_triangulated_points;
}
