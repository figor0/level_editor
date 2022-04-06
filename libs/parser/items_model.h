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
    using TargetType = QString;
    virtual bool isValid() const noexcept = 0;
    virtual bool setTarget(const TargetType& target) noexcept = 0;
    virtual QString target() const noexcept = 0;
    virtual Levels parse() noexcept = 0;
    virtual Levels parse(const TargetType& target) noexcept
    {
        Levels result;
        bool valid = setTarget(target);
        if ( valid == true ) {
            result = parse();
        }
        return result;
    }
    virtual bool serrialize(const TargetType& target,
                            const Levels& levels) noexcept
    {
        bool success = setTarget(target);
        if ( success == true ){
            success = serrialize(levels);
        }
        return success;
    }
    virtual bool serrialize(const Levels& levels) noexcept = 0;
};

class AFileLevelsIO: public ILevelsIO
{
public:
    AFileLevelsIO(const QString& target = {} ) noexcept
    {
        setTarget(target);
    }
    bool isValid() const noexcept override
    {
        return QFile::exists(m_filePath) &&
               isValidFormat();
    }
    bool setTarget(const QString &target) noexcept override
    {
        m_filePath = target;
        if ( !isValid() ){
            m_filePath = QString();
            return false;
        }
        return true;
    }
    QString target() const noexcept override
    {
        return m_filePath;
    }
    Levels parse() noexcept override
    {
        QFile file(m_filePath);
        Levels result;
        if ( file.open( QIODevice::ReadOnly ) )
        {
            result = parseLevels(file.readAll());
        }
        return result;
    }
    bool serrialize(const Levels &levels) noexcept override
    {
        bool result = false;
        QFile file(m_filePath);
        if ( file.open(QIODevice::WriteOnly)){
            auto data = serrializeLevels(levels);
            file.write(data);
            result = !data.isEmpty();
        }
        return result;
    }
protected:
    virtual bool isValidFormat() const noexcept = 0;
    virtual Levels parseLevels(const QByteArray& data) const noexcept = 0;
    virtual QByteArray serrializeLevels(const Levels& levels) noexcept = 0;
    QString m_filePath;
    QByteArray m_data;
};

class LevelsFromXml: public AFileLevelsIO
{
public:
    LevelsFromXml(const QString& target = {}) noexcept;
private:
    bool isValidFormat() const noexcept override;
    Levels parseLevels(const QByteArray& data) const noexcept override;
    QByteArray serrializeLevels(const Levels &levels) noexcept override;
};
