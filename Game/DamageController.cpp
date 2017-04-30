
#include "DamageController.h"
#include "System/System.h"

#include <limits>

using namespace game;

DamageRecord& DamageController::CreateRecord(unsigned int record_id)
{
    const auto& pair = m_DamageRecords.insert(std::make_pair(record_id, DamageRecord()));

    pair.first->second.health = 100;
    pair.first->second.strong_against = 0;
    pair.first->second.weak_against = 0;
    pair.first->second.multipier = 1;
    pair.first->second.last_damaged_timestamp = std::numeric_limits<unsigned int>::max();

    return pair.first->second;
}

void DamageController::RemoveRecord(unsigned int record_id)
{
    m_DamageRecords.erase(record_id);
}

DamageResult DamageController::ApplyDamage(unsigned int record_id, int damage)
{
    DamageResult result = { false, 0 };

    auto it = m_DamageRecords.find(record_id);
    if(it != m_DamageRecords.end())
    {
        DamageRecord& record = it->second;
        record.health -= damage * record.multipier;
        record.last_damaged_timestamp = System::GetMilliseconds();

        result.success = true;
        result.health_left = record.health;
    }

    return result;
}

const std::unordered_map<unsigned int, DamageRecord>& DamageController::GetDamageRecords() const
{
    return m_DamageRecords;
}
