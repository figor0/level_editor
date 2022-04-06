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

class Level
{
public:
    Level(const QString& level_name = "");
    Level(const Level& level);
    Level& operator=(const Level& level);
	bool load(const QString& level_path);
	bool load(const QDomDocument& doc);
    bool load(const QDomElement& element);
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
    QDomElement toElement() const;
	QString getSrc_path() const;
	void clear();
    QString getName() const;

    void setName(const QString &name);

    bool operator==(const Level& other);
    bool operator!=(const Level& other);
private:
    QDomElement m_width;
    QDomElement m_height;
	QString m_src_path;
	std::vector<std::vector<Item>> m_items;
    QString m_name;
};

bool operator==(const Level& left, const Level& right);

using Levels = std::vector<Level>;

class ILevelsIO
{
public:
    using DataType = QByteArray;
    virtual bool isValid(const DataType& data) noexcept = 0;
    virtual Levels parse(const DataType& target) noexcept = 0;
    virtual DataType serrialize(const Levels& levels) noexcept = 0;
};

class LevelsFromXml: public ILevelsIO
{
public:
    LevelsFromXml(const QString& target = {}) noexcept;
    bool isValid(const DataType& data) noexcept override;
    Levels parse(const DataType &data) noexcept override;
    DataType serrialize(const Levels &levels) noexcept override;
};
