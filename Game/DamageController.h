//
//  DamageController.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 11/06/16.
//
//

#pragma once

#include "System/SysTypes.h"
#include <unordered_map>

namespace game
{
    struct DamageRecord
    {
        int strong_against;
        int weak_against;
        int multipier;
        int health;
        uint last_damaged_timestamp;
    };

    struct DamageResult
    {
        bool success;
        int health_left;
    };

    class DamageController
    {
    public:

        DamageRecord& CreateRecord(uint record_id);
        void RemoveRecord(uint record_id);
        DamageResult ApplyDamage(uint record_id, int damage);
        const std::unordered_map<uint, DamageRecord>& GetDamageRecords() const;

    private:

        std::unordered_map<uint, DamageRecord> m_DamageRecords;
    };
}