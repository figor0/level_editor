#pragma once
#include <general.h>
#include <vector>
#include <QString>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>

QDomDocument createEmpty();

struct TypesContainer
{
	bool parseFromFile(const QString& file_path);
	Item createItem(const QString& type_name, const QList<QString>& values);
	std::list<ItemType> types_list;
	using type_name = QString;
	std::map<type_name, int> counters;
};

struct LevelItem
{
	LevelItem() = default;
	LevelItem(const QDomElement& level_element);
	LevelItem(int column, int row, const Item& item);
	QDomElement toElement();
	bool isValid() const;
	int m_x = -1, m_y = -1;
	Item m_item;
};

class ItemsArea
{
public:
	ItemsArea();
	bool load(const QString& level_path);
	bool load(const QDomDocument& doc);
	bool save();
	bool save(const QString& path);
	void create();
	void insert(int row, int column, const Item& item);
	void remove(int row, int column);
	size_t columnCount() const;
	size_t rowCount() const;
	Item get(int row, int column);
	std::vector<std::vector<Item> > getItems() const;
	QDomDocument toDomDocument();
	QString getSrc_path() const;
	void clear();
private:
	QDomElement m_width;
	QDomElement m_height;
	QString m_src_path;
	std::vector<std::vector<Item>> m_items;
};
