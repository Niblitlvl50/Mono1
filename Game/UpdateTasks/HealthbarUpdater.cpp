
#include "HealthbarUpdater.h"
#include "IPhysicsZone.h"
#include "IEntity.h"
#include "IPhysicsEntity.h"
#include "DamageController.h"
#include "Hud/Healthbar.h"
#include "System/SysTime.h"

using namespace game;

HealthbarUpdater::HealthbarUpdater(std::vector<Healthbar>& healthbars, const DamageController& damage_controller, const mono::IPhysicsZone& zone)
    : m_healthbars(healthbars),
        m_damageController(damage_controller),
        m_zone(zone)
{ }

void HealthbarUpdater::doUpdate(unsigned int delta)
{
    m_healthbars.clear();

    const std::unordered_map<uint, DamageRecord>& records = m_damageController.GetDamageRecords();
    const uint now = Time::GetMilliseconds();

    for(auto& record : records)
    {
        const int delta = now - record.second.last_damaged_timestamp;
        if(delta > 5000)
            continue;

        mono::IEntityPtr entity = m_zone.FindEntityFromId(record.first);
        if(!entity)
        {
            entity = m_zone.FindPhysicsEntityFromId(record.first);
            if(!entity)
                continue;
        }

        const math::Vector& position = entity->Position();
        const math::Vector& scale = entity->Scale();

        Healthbar bar;
        bar.position = position - math::Vector(0.0f, scale.y / 2.0f + 5.0f);
        bar.width = scale.x;
        bar.health_percentage = float(record.second.health) / 100.0f;
        m_healthbars.push_back(bar);
    }

}