
#include "ConstraintsFactory.h"
#include "IConstraint.h"
#include "IBody.h"

#include "chipmunk/chipmunk.h"

namespace
{
    class Constraint : public mono::IConstraint
    {
    public:

        Constraint(cpConstraint* constraint)
            : m_constraint(constraint)
        { }

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

        void SetErrorBias(float bias) override
        {
            cpConstraintSetErrorBias(m_constraint, bias);
        }

        float GetErrorBias() const override
        {
            return cpConstraintGetErrorBias(m_constraint);
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
    cpConstraint* constraint = cpPivotJointNew(a->Handle(), b->Handle(), cpvzero);
    return std::make_shared<Constraint>(constraint);
}

mono::IConstraintPtr mono::ConstraintsFactory::CreateGear(const mono::IBodyPtr& a, const mono::IBodyPtr& b, float phase, float ratio)
{
    cpConstraint* constraint = cpGearJointNew(a->Handle(), b->Handle(), phase, ratio);
    return std::make_shared<Constraint>(constraint);
}

mono::IConstraintPtr mono::ConstraintsFactory::CreateSpring(const mono::IBodyPtr& a, const mono::IBodyPtr& b, float restLength, float stiffness, float damping)
{
    cpConstraint* constraint = cpDampedSpringNew(a->Handle(), b->Handle(), cpvzero, cpvzero, restLength, stiffness, damping);
    return std::make_shared<Constraint>(constraint);
}