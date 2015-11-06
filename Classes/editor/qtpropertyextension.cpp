#include "qtpropertyextension.h"
#include <qtpropertymanager.h>

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif

VariantPropertyValue::VariantPropertyValue(QObject *parent)
    : QObject(parent)
    , m_manager(nullptr)
    , m_property(nullptr)
{}

void VariantPropertyValue::bind(VariantPropertyManagerExt *mgr, QtProperty* property)
{
    m_manager = mgr;
    m_property = property;
}

//////////////////////////////////////////////////////////////////////////

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
        VariantPropertyValue *value = (*it)(this);
        value->bind(this, property);
        value->initialize();
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
    }

    QtVariantPropertyManager::uninitializeProperty(property);
}

//////////////////////////////////////////////////////////////////////////
Vec3PropertyValue::Vec3PropertyValue(QObject *parent)
    : VariantPropertyValue(parent)
    , m_value(0.0f, 0.0f, 0.0f)
    , m_xProperty(nullptr)
    , m_yProperty(nullptr)
    , m_zProperty(nullptr)
{
    m_subPropertyManager = new QtDoublePropertyManager(this);
    connect(m_subPropertyManager, SIGNAL(valueChanged(QtProperty*,double)), this, SLOT(slotValueChanged(QtProperty*,double)));
}

QVariant Vec3PropertyValue::getValue() const
{
    return QVariant::fromValue(m_value);
}

void Vec3PropertyValue::setValue(const QVariant &val)
{
    QVector3D v = val.value<QVector3D>();
    if(v != m_value)
    {
        m_value = v;

        m_subPropertyManager->setValue(m_xProperty, m_value.x());
        m_subPropertyManager->setValue(m_yProperty, m_value.y());
        m_subPropertyManager->setValue(m_zProperty, m_value.z());
    }
}

QString Vec3PropertyValue::valueText() const
{
    return QString(tr("(%1, %2, %3)")
                   .arg(QString::number(m_value.x()))
                   .arg(QString::number(m_value.y()))
                   .arg(QString::number(m_value.z())));
}

void Vec3PropertyValue::initialize()
{
    m_xProperty = m_subPropertyManager->addProperty("x");
    m_property->addSubProperty(m_xProperty);

    m_yProperty = m_subPropertyManager->addProperty("y");
    m_property->addSubProperty(m_yProperty);

    m_zProperty = m_subPropertyManager->addProperty("z");
    m_property->addSubProperty(m_zProperty);
}

#define SAFE_DELETE_POINTER(p) if(p != nullptr){ delete p; p = nullptr; }

void Vec3PropertyValue::uninitialize()
{
    SAFE_DELETE_POINTER(m_xProperty)
    SAFE_DELETE_POINTER(m_yProperty)
    SAFE_DELETE_POINTER(m_zProperty)
}

void Vec3PropertyValue::slotValueChanged(QtProperty *property, double val)
{
    if(property == m_xProperty)
    {
        m_value.setX(val);
    }
    else if(property == m_yProperty)
    {
        m_value.setY(val);
    }
    else if(property == m_zProperty)
    {
        m_value.setZ(val);
    }

    m_manager->setValue(property, QVariant::fromValue(m_value));
}

#if QT_VERSION >= 0x040400
QT_END_NAMESPACE
#endif
