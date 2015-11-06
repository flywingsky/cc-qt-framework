#include "qtpropertyextension.h"


VariantPropertyManagerExt::VariantPropertyManagerExt(QObject *parent)
    : QtVariantPropertyManager(parent)
{
}

VariantPropertyManagerExt::~VariantPropertyManagerExt()
{

}

void VariantPropertyManagerExt::registerPropertyCreator(int type, VariantPropertyValueCreator creator)
{
    m_creators[type] = creator;
}

bool VariantPropertyManagerExt::isPropertyTypeSupported(int propertyType) const
{
    CreatorMap::const_iterator it = m_creators.constFind(propertyType);
    if(it != m_creators.constEnd())
    {
        return true;
    }

    return QtVariantPropertyManager::isPropertyTypeSupported(propertyType);
}

int VariantPropertyManagerExt::valueType(int propertyType) const
{
    CreatorMap::const_iterator it = m_creators.constFind(propertyType);
    if(it != m_creators.constEnd())
    {
        return propertyType;
    }

    return QtVariantPropertyManager::valueType(propertyType);
}

QVariant VariantPropertyManagerExt::value(const QtProperty *property) const
{
    DataMap::const_iterator it = m_propertyToData.constFind(property);
    if(it != m_propertyToData.constEnd())
    {
        return (*it)->getValue();
    }

    return QtVariantPropertyManager::value(property);
}

QString VariantPropertyManagerExt::valueText(const QtProperty *property) const
{
    DataMap::const_iterator it = m_propertyToData.constFind(property);
    if(it != m_propertyToData.constEnd())
    {
        return (*it)->valueText();
    }

    return QtVariantPropertyManager::valueText(property);
}

void VariantPropertyManagerExt::setValue(QtProperty *property, const QVariant &value)
{
    DataMap::iterator it = m_propertyToData.find(property);
    if(it != m_propertyToData.end())
    {
        (*it)->setValue(value);

        emit propertyChanged(property);
        emit valueChanged(property, value);
        return;
    }

    QtVariantPropertyManager::setValue(property, value);
}

void VariantPropertyManagerExt::initializeProperty(QtProperty *property)
{
    int type = propertyType(property);
    CreatorMap::iterator it = m_creators.find(type);
    if(it != m_creators.end())
    {
        VariantPropertyValue *value = (*it)(this, property);
        m_propertyToData[property] = value;
        return;
    }

    QtVariantPropertyManager::initializeProperty(property);
}

void VariantPropertyManagerExt::uninitializeProperty(QtProperty *property)
{
    DataMap::iterator it = m_propertyToData.find(property);
    if(it != m_propertyToData.end())
    {
        VariantPropertyValue *value = *it;
        value->uninitialize();
        delete value;

        m_propertyToData.erase(it);
        return;
    }

    QtVariantPropertyManager::uninitializeProperty(property);
}
