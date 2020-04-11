
#include "ZoneBase.h"
#include "IUpdater.h"
#include "Rendering/IRenderer.h"
#include "Zone/IEntity.h"
#include "Util/Algorithm.h"
#include "Math/Quad.h"
#include "Math/MathFunctions.h"
#include "System/System.h"

#include <cstdio>

using namespace mono;

namespace
{
    bool DrawableSortFunc(
        const std::pair<int, const mono::IDrawable*>& first, const std::pair<int, const mono::IDrawable*>& second)
    {
        return first.first < second.first;
    }
}

ZoneBase::~ZoneBase()
{
    std::vector<mono::IEntity*> entities = m_entities;


    // TODO, delete everything that's left here. 
}

void ZoneBase::Accept(IRenderer& renderer)
{
    for(auto& pair : m_drawables)
        renderer.AddDrawable(pair.second);
}

void ZoneBase::Accept(mono::IUpdater& updater)
{
    DoPreAccept();

    for(mono::IUpdatable* updatable : m_updatables)
        updater.AddUpdatable(updatable);
}

void ZoneBase::DoPreAccept()
{
    for(mono::IEntity* entity : m_entities_remove)
    {
        const bool result = mono::remove(m_entities, entity);
        if(!result)
            System::Log("ZoneBase|Unable to remove entity with id %u\n", entity->Id());
    }

    for(mono::IUpdatable* updatable : m_updatables_remove)
    {
        const bool result = mono::remove(m_updatables, updatable);
        if(!result)
            System::Log("ZoneBase|Unable to remove updatable\n");
    }

    for(mono::IDrawable* drawable : m_drawables_remove)
    {
        const auto func = [drawable](const std::pair<int, const mono::IDrawable*>& pair) {
            return pair.second == drawable;
        };

        const bool removed = mono::remove_if(m_drawables, func);
        if(!removed)
            System::Log("ZoneBase|Unable to remove drawable\n");
    }

    m_entities_remove.clear();
    m_updatables_remove.clear();
    m_drawables_remove.clear();
}

void ZoneBase::AddEntity(IEntity* entity, int layer)
{
    AddDrawable(entity, layer);
    AddUpdatable(entity);

    m_entities.push_back(entity);
}

void ZoneBase::RemoveEntity(IEntity* entity)
{
    m_entities_remove.push_back(entity);
    m_updatables_remove.push_back(entity);
    m_drawables_remove.push_back(entity);
}

void ZoneBase::AddUpdatable(IUpdatable* updatable)
{
    m_updatables.push_back(updatable);
}

void ZoneBase::RemoveUpdatable(IUpdatable* updatable)
{
    m_updatables_remove.push_back(updatable);
}

void ZoneBase::AddDrawable(mono::IDrawable* drawable, int layer)
{
    m_drawables.push_back(std::make_pair(layer, drawable));

    // Keep the drawable vector sorted so that we draw everything
    // in the correct order according to layers
    std::sort(m_drawables.begin(), m_drawables.end(), DrawableSortFunc);
}

void ZoneBase::RemoveDrawable(mono::IDrawable* drawable)
{
    m_drawables_remove.push_back(drawable);
}

void ZoneBase::SetDrawableLayer(const mono::IDrawable* drawable, int new_layer)
{
    const auto func = [drawable](const std::pair<int, const mono::IDrawable*>& pair) {
        return pair.second == drawable;
    };

    auto it = std::find_if(m_drawables.begin(), m_drawables.end(), func);
    if(it != m_drawables.end())
    {
        it->first = new_layer;
        std::sort(m_drawables.begin(), m_drawables.end(), DrawableSortFunc);
    }
}

mono::IEntity* ZoneBase::FindEntityFromId(uint32_t id) const
{
    const auto find_func = [id](const mono::IEntity* entity) {
        return id == entity->Id();
    };

    const auto& it = std::find_if(m_entities.begin(), m_entities.end(), find_func);
    if(it != m_entities.end())
        return *it;

    return nullptr;
}

mono::IEntity* ZoneBase::FindEntityFromPoint(const math::Vector& point) const
{
    const auto find_func = [&point](const mono::IEntity* entity) {
        const math::Quad& bb = entity->BoundingBox();
        return math::PointInsideQuad(point, bb);
    };

    const auto& it = std::find_if(m_entities.begin(), m_entities.end(), find_func);
    if(it != m_entities.end())
        return *it;

    return nullptr;
}

const std::vector<mono::IEntity*>& ZoneBase::GetEntities() const
{
    return m_entities;
}
