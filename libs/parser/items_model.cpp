#include <items_model.h>
#include <QTextStream>

const int total_width = 30;
const int total_height = 15;

Item TypesContainer::createItem(const QString &type_name, const QList<QString> &values){
	Item result;
	auto need_it = std::find_if(types_list.begin(),
								types_list.end(),
								[type_name](const ItemType& type)
	{
		return type.name() == type_name;
	}
	);
	if (need_it != types_list.end())
	{
		result = need_it->CreateItem(values);
		if (result.type_name().isEmpty() == false){
			counters[type_name]++;
		}
	}
	return result;
}


Level::Level(const QString& level_name):
	m_items(total_height,
            std::vector<Item>(total_width, Item())),
    m_name(level_name)
{
	assert(m_items.size() == total_height);
	for (const auto& vector: m_items)
        assert(vector.size() == total_width);
}

Level::Level(const Level &level):
    m_width(level.m_width),
    m_height(level.m_height),
    m_src_path(level.m_src_path),
    m_items(level.m_items),
    m_name(level.m_name)
{

}

Level &Level::operator=(const Level &level)
{
    m_width = level.m_width;
    m_height = level.m_height;
    m_src_path = level.m_src_path;
    m_items = level.m_items;
    m_name = level.m_name;
}

bool Level::load(const QString &level_path)
{
	clear();
	bool success_flag = false;
	QDomDocument doc;
	QFile file(level_path);
	if (openFile(file)){
		if (doc.setContent(&file)){
			m_src_path = level_path;
			success_flag = load(doc);
		}
	}
	return success_flag;
}

bool Level::save(){
	bool success = false;
	if (!m_src_path.isEmpty()){
		QFile file(m_src_path);
		if (file.open(QIODevice::WriteOnly) == true)
		{
			QDomDocument level = toDomDocument();
			QTextStream writer(&file);
			level.save(writer, 4);
			success = true;
		}
	}
	return success;
}

bool Level::save(const QString& path)
{
	m_src_path = path;
	return save();
}

void Level::create()
{
	clear();
	m_src_path.clear();
}

void Level::insert(int row, int column, const Item &item)
{
	m_items[row][column] = item;
}

void Level::remove(int row, int column)
{
	m_items[row][column] = Item();
}

size_t Level::columnCount() const
{
	return total_width;
}

size_t Level::rowCount() const
{
	return total_height;
}

Item Level::get(int row, int column){
	return m_items[row][column];
}

void Level::clear()
{
	for (auto& row: m_items){
		for (auto& column: row){
			column = Item();
		}
	}
}

QString Level::getName() const
{
    return m_name;
}

void Level::setName(const QString &name)
{
    m_name = name;
}

QString Level::getSrc_path() const
{
    return m_src_path;
}

bool Level::load(const QDomDocument &doc)
{
	bool success_flag = false;
	auto root_elem = doc.firstChildElement();
	if (root_elem.isNull() == false){
        success_flag = load(root_elem);
	}
	return success_flag;

}

bool Level::load(const QDomElement &element)
{
    bool success_flag = false;
    auto curr_elem = element.firstChildElement();
    while(curr_elem.isNull() == false)
    {
        LevelItem level_item(curr_elem);
        if (level_item.isValid()){
            insert(level_item.m_y, level_item.m_x, level_item.m_item);
            success_flag = true;
        } else{
            clear();
            return false;
        }
        curr_elem = curr_elem.nextSiblingElement();
    }
    return success_flag;
}

std::vector<std::vector<Item> > Level::getItems() const
{
	return m_items;
}

QDomDocument Level::toDomDocument()
{
	QDomDocument doc;
    auto element = toElement();
    element.setTagName("root");
    doc.appendChild(element);
    return doc;
}

QDomElement Level::toElement() const
{
    QDomElement result;
    for (size_t row = 0; row < m_items.size(); row++)
    {
        for (size_t column = 0; column < m_items[row].size(); column++)
        {
            const auto& item = m_items[row][column];
            if (item.type_name().isEmpty() == false){
                LevelItem level_item(column, row, item);
                result.appendChild(level_item.toElement());
            }
        }
    }
    return result;
}

QDomDocument createEmpty()
{
	QDomDocument result;
	QByteArray content = init_string.toUtf8() + "<root></root>";
	assert(result.setContent(content) == true);
	assert(result.firstChildElement().firstChildElement().isNull());
	return result;
}

LevelItem::LevelItem(const QDomElement &level_element)
{
	bool success;
	int x, y;
	x = level_element.firstChildElement("x").text().toInt(&success);
	y = level_element.firstChildElement("y").text().toInt(&success);
	QDomElement item_element = level_element.lastChildElement();
	Item item(item_element );
	if (success == true && item.type_name().isNull() == false){
		m_x = x;
		m_y = y;
		m_item = item;
	}
}

LevelItem::LevelItem(int column, int row, const Item &item):
	m_x(column), m_y(row), m_item(item)
{}

QDomElement LevelItem::toElement()
{
	QDomElement result;
	QDomDocument doc;
	if (isValid() == true
			&& m_x >= 0 && m_y >= 0)
	{
		result = doc.createElement("level_item");
		QDomElement x = doc.createElement("x");
		x.appendChild(doc.createTextNode(QString::number(m_x)));
		QDomElement y = doc.createElement("y");
		y.appendChild(doc.createTextNode(QString::number(m_y)));
		result.appendChild(x);
		result.appendChild(y);
		result.appendChild(m_item.toElement());
	}
	return result;
}

bool LevelItem::isValid() const
{
	return m_item.type_name().isEmpty() == false;
}


//Levels ILevelsIO::parse(const ILevelsIO::TargetType &target) noexcept
//{
//    Levels result;
//    bool valid = setTarget(target);
//    if ( valid == true ) {
//        result = parse();
//    }
//    return result;
//}

//bool ILevelsIO::serrialize(const ILevelsIO::TargetType &target, const Levels &levels) noexcept
//{
//    bool success = setTarget(target);
//    if ( success == true ){
//        success = serrialize(levels);
//    }
//    return success;
//}

//AFileLevelsIO::AFileLevelsIO(const QString &target) noexcept
//{
//    setTarget(target);
//}

//bool AFileLevelsIO::isValid() const noexcept override
//{
//    return QFile::exists(m_filePath) &&
//            isValidFormat();
//}

//bool AFileLevelsIO::setTarget(const QString &target) noexcept override
//{
//    m_filePath = target;
//    if ( !isValid() ){
//        m_filePath = QString();
//        return false;
//    }
//    return true;
//}

//QString AFileLevelsIO::target() const noexcept override
//{
//    return m_filePath;
//}

//Levels AFileLevelsIO::parse() noexcept override
//{
//    QFile file(m_filePath);
//    Levels result;
//    if ( file.open( QIODevice::ReadOnly ) )
//    {
//        result = parseLevels(file.readAll());
//    }
//    return result;
//}

//bool AFileLevelsIO::serrialize(const Levels &levels) noexcept override
//{
//    bool result = false;
//    QFile file(m_filePath);
//    if ( file.open(QIODevice::WriteOnly)){
//        auto data = serrializeLevels(levels);
//        file.write(data);
//        result = !data.isEmpty();
//    }
//    return result;
//}

bool LevelsFromXml::isValid(const ILevelsIO::DataType &data) noexcept
{
    return QDomDocument(data).isNull();
}

Levels LevelsFromXml::parse(const ILevelsIO::DataType &data) noexcept
{
    Levels result;
    if ( isValid(data) ){
        QDomDocument root(data);
        auto root_elem = root.firstChildElement();
    }
    return result;
}

ILevelsIO::DataType LevelsFromXml::serrialize(const Levels &levels) noexcept
{

}
