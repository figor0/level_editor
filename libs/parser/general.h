#pragma once
#include <QDomDocument>
#include <QString>
#include <QtXmlPatterns/QXmlSchemaValidator>
#include <QtXmlPatterns/QXmlSchema>
#include <QFile>
#include <QDebug>
#include <QDomElement>
#include <iostream>
#include <list>
#include <QList>
#include <map>

const QString init_string = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";

bool openFile(QFile& file);
QByteArray buildPartAsGeneral(const QDomElement& element);

class Item{
public:
	Item() = default;
	Item(const QDomElement& doc);
	Item& operator=(const Item& right);
	Item(const Item& item);
	QDomElement toElement();
	QString type_name() const;
	std::list<std::pair<QString, QString>> members() const;
    bool operator==(const Item& other) const noexcept;
    bool operator!=(const Item& other) const noexcept;
private:
	friend class ItemType;
	QString m_type_name;
	std::list<std::pair<QString, QString>> m_members;
};

QDomDocument prepareXml(const QString& root_tag,
						const QList<QString>& tags,
						const QList<QString>& values);
class ItemType
{
public:
	ItemType(QDomElement xml_mapping);
	ItemType(const ItemType& type);
	Item CreateItem(const QList<QString>& values);
	bool unique_flag() const;
	QString name() const;
	QString image_path() const;
	QXmlSchema validator() const;
	QString root_tag() const;
	QList<QString> tags() const;
	bool isValid() const;
private:
	bool m_unique_flag;
	QString m_name;
	QString m_image_path;
	QXmlSchema m_schema;
	QList<QString> m_tags;
};

std::list<ItemType> parse_config(const QString& file_path);













