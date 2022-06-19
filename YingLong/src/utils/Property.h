#pragma once

#include <functional>

namespace YingLong {

    /**
    * A Property template class for easy to write viarables with Lambda Setter & Getter functions implicit.
    */
    template<typename PropertyType>
    class Property
    {
    public:
        using SetterType = std::function<void (const PropertyType&, PropertyType&)>;
        using GetterType = std::function<PropertyType (const PropertyType&)>;

        Property(const PropertyType& InitValue)
            : m_Value(InitValue)
            , m_Setter([](const PropertyType& NewValue, PropertyType& InnerValue) { InnerValue = NewValue; })
            , m_Getter([](const PropertyType& InnerValue) { return InnerValue; })
        {}

        Property(const PropertyType& InitValue, const SetterType& Setter)
            : m_Value(InitValue)
            , m_Setter(Setter)
            , m_Getter([](const PropertyType& InnerValue) { return InnerValue; })
        {}

        Property(const PropertyType& InitValue, const GetterType& Getter)
            : m_Value(InitValue)
            , m_Setter([](const PropertyType& NewValue, PropertyType& InnerValue) { InnerValue = NewValue; })
            , m_Getter(Getter)
        {}

        Property(const PropertyType& InitValue, const SetterType& Setter, const GetterType& Getter)
            : m_Value(InitValue)
            , m_Setter(Setter)
            , m_Getter(Getter)
        {}

        PropertyType Value() const
        {
            return m_Getter(m_Value);
        }

        operator PropertyType() const
        {
            return m_Getter(m_Value);
        }

        Property<PropertyType>& operator = (const Property<PropertyType>& Value)
        {
            if (&Value == this)
            {
                return *this;
            }

            m_Setter(Value.m_Value, m_Value);
            return *this;
        }

    private:
        PropertyType m_Value;
        const SetterType m_Setter;
        const GetterType m_Getter;
    };
}
