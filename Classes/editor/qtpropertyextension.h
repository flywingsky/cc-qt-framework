#ifndef QTPROPERTYEXTENSION_H
#define QTPROPERTYEXTENSION_H

#include <QtVariantProperty>
#include <QVector3D>

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif

class VariantPropertyManagerExt;

class VariantPropertyValue : public QObject
{
    Q_OBJECT
public:
    VariantPropertyValue(QObject *parent);

    virtual void bind(VariantPropertyManagerExt *mgr, QtProperty* property);

    virtual QVariant getValue() const = 0;
    virtual void setValue(const QVariant &val) = 0;

    virtual QString valueText() const = 0;

    virtual void initialize() = 0;
    virtual void uninitialize() = 0;

protected:
    VariantPropertyManagerExt*  m_manager;
    QtProperty*                 m_property;
};

typedef VariantPropertyValue* (*VariantPropertyValueCreator)(QObject * /*parent*/);

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

class QtDoublePropertyManager;

class Vec3PropertyValue : public VariantPropertyValue
{
    Q_OBJECT
public:
    static Vec3PropertyValue* create(QObject *parent){ return new Vec3PropertyValue(parent); }

    Vec3PropertyValue(QObject *parent);

    virtual QVariant getValue() const;
    virtual void setValue(const QVariant &val);

    virtual QString valueText() const;

    virtual void initialize();
    virtual void uninitialize();

private slots:
    void slotValueChanged(QtProperty *property, double val);

private:
    QtDoublePropertyManager *m_subPropertyManager;

    QtProperty*     m_xProperty;
    QtProperty*     m_yProperty;
    QtProperty*     m_zProperty;

    QVector3D       m_value;
};

#if QT_VERSION >= 0x040400
QT_END_NAMESPACE
#endif

#endif // QTPROPERTYEXTENSION_H
