//
//  CacoDemonController.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 22/11/15.
//
//

#pragma once

#include "CMIBody.h"

namespace game
{
    class CacoDemon;

    class CacoDemonController : public mono::ICollisionHandler
    {
    public:

        CacoDemonController(CacoDemon* demon);

        virtual void OnCollideWith(const mono::IBodyPtr& body);
        virtual void OnPostStep();

    private:

        CacoDemon* m_demon;
    };
}
