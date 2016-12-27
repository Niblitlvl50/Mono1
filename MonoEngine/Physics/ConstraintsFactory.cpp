
#include "ConstraintsFactory.h"
#include "IConstraint.h"
#include "CMIBody.h"

#include "chipmunk/chipmunk.h"

namespace
{
    class Constraint : public mono::IConstraint
    {
    public:

        Constraint(const mono::IBodyPtr& a, const mono::IBodyPtr& b)
        {
            m_constraint = cpPivotJointNew(a->Handle(), b->Handle(), cpvzero);
        }

        ~Constraint()
        {
            cpConstraintFree(m_constraint);
        }

        void SetMaxForce(float force) override
        {
            cpConstraintSetMaxForce(m_constraint, force);
        }

        float GetMaxForce() const override
        {
            return cpConstraintGetMaxForce(m_constraint);
        }

        void SetMaxBias(float bias) override
        {
            cpConstraintSetMaxBias(m_constraint, bias);
        }

        float GetMaxBias() const override
        {
            return cpConstraintGetMaxBias(m_constraint);
        }

        cpConstraint* Handle() override
        {
            return m_constraint;
        }

        cpConstraint* m_constraint;
    };
}

mono::IConstraintPtr mono::ConstraintsFactory::CreatePivot(const mono::IBodyPtr& a, const mono::IBodyPtr& b)
{
    return std::make_shared<Constraint>(a, b);
}
