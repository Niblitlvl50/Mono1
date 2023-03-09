
#include "PathFactory.h"
#include "IPath.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/MathFunctions.h"
#include "Rendering/RenderBuffer/BufferFactory.h"

#include "par_streamlines/par_streamlines.h"

namespace
{
    class PathImpl : public mono::IPath
    {
    public:

        PathImpl(const std::vector<math::Vector>& coords)
            : m_points(coords)
        {
            m_length_table.reserve(coords.size());

            float m_total_length = 0.0f;

            for(int index = 0; index < static_cast<int>(coords.size()); ++index)
            {
                const int previous_index = std::max(0, index -1);

                const math::Vector& previous_point = coords[previous_index];
                const math::Vector& current_point = coords[index];
                
                const math::Vector& diff = current_point - previous_point;

                m_total_length += math::Length(diff);
                m_length_table.push_back(m_total_length);
            }
        }

        float Length() const override
        {
            return m_length_table.empty() ? 0.0f : m_length_table.back();
        }

        const std::vector<math::Vector>& GetPathPoints() const override
        {
            return m_points;
        }

        math::Vector GetPositionByLength(float length) const override
        {
            if(m_length_table.empty())
                return math::ZeroVec;

            const auto find_func = [length](float start_length) {
                return length <= start_length;
            };

            auto it = std::find_if(m_length_table.begin(), m_length_table.end(), find_func);
            if(it == m_length_table.end())
                return math::ZeroVec;

            if(it != m_length_table.begin())
                --it;

            const size_t point_index = std::distance(m_length_table.begin(), it);

            const math::Vector& previous_point = m_points[point_index];
            const math::Vector& current_point = m_points[point_index +1];

            const math::Vector& diff = current_point - previous_point;
            const float diff_length = math::Length(diff);

            const float local_length = length - (*it);
            const float percent = local_length / diff_length;
            
            const math::Vector& pos = previous_point + (diff * percent);
            return pos;
        }

        float GetLengthFromPosition(const math::Vector& position) const override
        {
            if(m_points.empty())
                return 0.0f;

            math::Vector best_point;
            uint32_t first_index = -1;
            float shortest_distance = math::INF;

            for(uint32_t index = 0; index < m_points.size() - 1; ++index)
            {
                const math::PointOnLineResult result = math::ClosestPointOnLine(m_points[index], m_points[index +1], position);
                if(result.t < 0.0f || result.t > 1.0f)
                    continue; // Not on line

                const float distance = math::DistanceBetween(result.point, position);
                if(distance < shortest_distance)
                {
                    best_point = result.point;
                    shortest_distance = distance;
                    first_index = index;
                }
            }

            if(shortest_distance == math::INF)
                return 0.0f;
    
            const float distance_to_first = m_length_table[first_index];
            const float distance_to_point = math::DistanceBetween(m_points[first_index], best_point);

            return distance_to_first + distance_to_point;
        }

        const std::vector<math::Vector> m_points;
        std::vector<float> m_length_table;
    };
}

mono::IPathPtr mono::CreatePath(const std::vector<math::Vector>& coords)
{
    return std::make_unique<PathImpl>(coords);
}

mono::IPathPtr mono::CreatePath(const std::vector<math::Vector>& coords, const math::Matrix& transform)
{
    std::vector<math::Vector> world_points;
    world_points.reserve(coords.size());

    for(const math::Vector& local_point : coords)
        world_points.push_back(math::Transformed(transform, local_point));

    return mono::CreatePath(world_points);
}


mono::PathDrawBuffer mono::BuildPathDrawBuffers(PathType type, const std::vector<math::Vector>& points, const PathOptions& options)
{
    if(points.empty())
        return {};

    parsl_config config;
    config.thickness = options.width;
    config.flags = PARSL_FLAG_ANNOTATIONS;
    config.curves_max_flatness = 0.1f;
    config.streamlines_seed_spacing = 0.0f;
    config.streamlines_seed_viewport = {};
    config.miter_limit = 0.0f;
    config.u_mode = PAR_U_MODE_NORMALIZED_DISTANCE;

    if(options.uv_mode & UVMode::DISTANCE)
        config.u_mode = PAR_U_MODE_DISTANCE;
    else if(options.uv_mode & UVMode::NORMALIZED_DISTANCE)
        config.u_mode = PAR_U_MODE_NORMALIZED_DISTANCE;

    parsl_context* ctx = parsl_create_context(config);

    uint16_t spine_length[] = { (uint16_t)points.size() };

    parsl_spine_list spine_list;
    spine_list.num_vertices = points.size();
    spine_list.num_spines = 1;
    spine_list.vertices = (parsl_position*)points.data();
    spine_list.spine_lengths = spine_length;
    spine_list.closed = options.closed;

    parsl_mesh* generated_mesh = nullptr;

    switch(type)
    {
    case mono::PathType::REGULAR:
        generated_mesh = parsl_mesh_from_lines(ctx, spine_list);
        break;
    case mono::PathType::BEZIER_QUADRATIC:
        generated_mesh = parsl_mesh_from_curves_quadratic(ctx, spine_list);
        break;
    case mono::PathType::BEZIER_CUBIC:
        generated_mesh = parsl_mesh_from_curves_cubic(ctx, spine_list);
        break;
    }

    if(options.uv_mode & UVMode::NORMALIZED_WIDTH)
    {
        for(uint32_t index = 0; index < generated_mesh->num_vertices; ++index)
        {
            parsl_annotation& an = generated_mesh->annotations[index];
            an.v_across_curve = std::clamp(an.v_across_curve, 0.0f, 1.0f);
        }
    }

    const uint32_t num_indices = generated_mesh->num_triangles * 3;

    const std::vector<mono::Color::RGBA> colors(generated_mesh->num_vertices, options.color);

    std::vector<uint16_t> indices;
    indices.resize(num_indices);

    for(uint32_t index = 0; index < num_indices; ++index)
        indices[index] = generated_mesh->triangle_indices[index];

    mono::PathDrawBuffer buffers;
    buffers.vertices = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, generated_mesh->num_vertices, generated_mesh->positions);
    buffers.colors = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 4, generated_mesh->num_vertices, colors.data());
    buffers.anotations = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 4, generated_mesh->num_vertices, generated_mesh->annotations);
    buffers.indices = CreateElementBuffer(BufferType::STATIC, indices.size(), indices.data());

    parsl_destroy_context(ctx);

    return buffers;
}
