
#include "ZoneBase.h"
#include "IUpdatable.h"
#include "IUpdater.h"
#include "Rendering/IDrawable.h"
#include "Rendering/IRenderer.h"
#include "Util/Algorithm.h"
#include "Math/Quad.h"
#include "Math/MathFunctions.h"
#include "System/System.h"

#include <set>
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
    DoDeferredDelete();

    std::set<mono::IDrawable*> deleted_drawables;
    std::set<mono::IUpdatable*> deleted_updatables;

    for(std::pair<int, mono::IDrawable*>& drawable_pair : m_drawables)
    {
        const bool not_deleted = (deleted_drawables.find(drawable_pair.second) == deleted_drawables.end());
        if(not_deleted)
        {
            delete drawable_pair.second;
            deleted_drawables.insert(drawable_pair.second);
        }
    }

    for(mono::IUpdatable* updatable : m_updatables)
    {
        const bool not_deleted = (deleted_updatables.find(updatable) == deleted_updatables.end());
        if(not_deleted)
        {
            delete updatable;
            deleted_updatables.insert(updatable);
        }
    }
}

void ZoneBase::Accept(IRenderer& renderer)
{
    for(const auto& pair : m_drawables)
        renderer.AddDrawable(pair.second);
}

void ZoneBase::Accept(mono::IUpdater& updater)
{
    DoDeferredDelete();

    for(mono::IUpdatable* updatable : m_updatables)
        updater.AddUpdatable(updatable);
}

void ZoneBase::DoDeferredDelete()
{
    const auto remove_updatable_func = [this](const mono::IUpdatable* updatable) {
        return std::find(m_updatables_remove.begin(), m_updatables_remove.end(), updatable) != m_updatables_remove.end();
    };
    mono::remove_if(m_updatables, remove_updatable_func);

    const auto remove_drawable_func = [this](const std::pair<int, mono::IDrawable*>& drawable_pair) {
        return std::find(m_drawables_remove.begin(), m_drawables_remove.end(), drawable_pair.second) != m_drawables_remove.end();
    };
    mono::remove_if(m_drawables, remove_drawable_func);

    m_updatables_remove.clear();
    m_drawables_remove.clear();
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
