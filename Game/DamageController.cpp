//
//  DamageController.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 11/06/16.
//
//

#include "DamageController.h"


using namespace game;

DamageRecord& DamageController::CreateRecord(uint record_id)
{
    const auto& pair = m_DamageRecords.insert(std::make_pair(record_id, DamageRecord()));

    pair.first->second.health = 100;
    pair.first->second.strong_against = 0;
    pair.first->second.weak_against = 0;
    pair.first->second.multipier = 1;

    return pair.first->second;
}

void DamageController::RemoveRecord(uint record_id)
{
    m_DamageRecords.erase(record_id);
}

DamageResult DamageController::ApplyDamage(uint id, int damage)
{
    DamageResult result = { false, 0 };

    auto it = m_DamageRecords.find(id);
    if(it != m_DamageRecords.end())
    {
        DamageRecord& record = it->second;
        record.health -= damage * record.multipier;

        result.success = true;
        result.health_left = record.health;
    }

    return result;
}
