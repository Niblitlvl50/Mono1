
#pragma once

#include <unordered_map>

namespace game
{
    struct DamageRecord
    {
        int strong_against;
        int weak_against;
        int multipier;
        int health;
        unsigned int last_damaged_timestamp;
    };

    struct DamageResult
    {
        bool success;
        int health_left;
    };

    class DamageController
    {
    public:

        DamageRecord& CreateRecord(unsigned int record_id);
        void RemoveRecord(unsigned int record_id);
        DamageResult ApplyDamage(unsigned int record_id, int damage);
        const std::unordered_map<unsigned int, DamageRecord>& GetDamageRecords() const;

    private:

        std::unordered_map<unsigned int, DamageRecord> m_DamageRecords;
    };
}