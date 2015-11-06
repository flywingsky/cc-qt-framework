#ifndef QTPROPERTYEXTENSION_H
#define QTPROPERTYEXTENSION_H

#include <QtVariantProperty>

class VariantPropertyManagerExt;

class VariantPropertyValue
{
public:
    VariantPropertyValue() : m_property(nullptr) {}
    virtual ~VariantPropertyValue(){}

    virtual QVariant getValue() const = 0;
    virtual void setValue(const QVariant &val) = 0;

    virtual QString valueText() const = 0;

    virtual void initialize() = 0;
    virtual void uninitialize() = 0;

protected:
    QtProperty*     m_property;
};

typedef VariantPropertyValue* (*VariantPropertyValueCreator)(VariantPropertyManagerExt* /*mgr*/, QtProperty* /*property*/);

class VariantPropertyManagerExt : public QtVariantPropertyManager
{
    Q_OBJECT
public:
    VariantPropertyManagerExt(QObject *parent = 0);
    ~VariantPropertyManagerExt();

    void registerPropertyCreator(int type, VariantPropertyValueCreator creator);

    virtual QVariant value(const QtProperty *property) const override;
    virtual int valueType(int propertyType) const override;
    virtual bool isPropertyTypeSupported(int propertyType) const override;

    virtual QString valueText(const QtProperty *property) const override;

public slots:
    virtual void setValue(QtProperty *property, const QVariant &val) override;

protected:
    virtual void initializeProperty(QtProperty *property) override;
    virtual void uninitializeProperty(QtProperty *property) override;

    typedef QMap<const QtProperty *, VariantPropertyValue*> DataMap;
    DataMap         m_propertyToData;

    typedef QMap<int, VariantPropertyValueCreator> CreatorMap;
    CreatorMap      m_creators;
};

#endif // QTPROPERTYEXTENSION_H
