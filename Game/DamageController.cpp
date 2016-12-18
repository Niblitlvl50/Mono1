//
//  DamageController.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 11/06/16.
//
//

#include "DamageController.h"
#include "System/SysTime.h"

#include <limits>

using namespace game;

DamageRecord& DamageController::CreateRecord(uint record_id)
{
    const auto& pair = m_DamageRecords.insert(std::make_pair(record_id, DamageRecord()));

    pair.first->second.health = 100;
    pair.first->second.strong_against = 0;
    pair.first->second.weak_against = 0;
    pair.first->second.multipier = 1;
    //pair.first->second.last_damaged_timestamp = 10000000;
    pair.first->second.last_damaged_timestamp = std::numeric_limits<uint>::max();

    return pair.first->second;
}

void DamageController::RemoveRecord(uint record_id)
{
    m_DamageRecords.erase(record_id);
}

DamageResult DamageController::ApplyDamage(uint record_id, int damage)
{
    DamageResult result = { false, 0 };

    auto it = m_DamageRecords.find(record_id);
    if(it != m_DamageRecords.end())
    {
        DamageRecord& record = it->second;
        record.health -= damage * record.multipier;
        record.last_damaged_timestamp = Time::GetMilliseconds();

        result.success = true;
        result.health_left = record.health;
    }

    return result;
}

const std::unordered_map<uint, DamageRecord>& DamageController::GetDamageRecords() const
{
    return m_DamageRecords;
}
