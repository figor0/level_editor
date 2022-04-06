#include <LevelModel.h>

QHash<int, QByteArray> LevelModel::roleNames() const {
	return {
		{ImagePath, "image_path"},
		{TypeName, "type_name"},
		{DataMembers, "data_members"},
		{DataTypes, "data_types"},
		{Row, "row"},
		{Column, "column"}
	};
}

LevelModel::LevelModel(std::shared_ptr<Level> items_ptr,
					   std::shared_ptr<TypesContainer> types_ptr,
					   QObject* parent): QAbstractTableModel(parent),
	m_items_ptr(items_ptr), m_types_ptr(types_ptr)
{}

int LevelModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	return m_items_ptr->rowCount();
}

int LevelModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	return m_items_ptr->columnCount();
}

QVariant LevelModel::data(const QModelIndex &index, int role) const
{
	QVariant result;
	if (index.isValid() == false)
		return QVariant();
	Item item = m_items_ptr->get(index.row(), index.column());
	if (role == ImagePath){
		result.setValue(QString());
		if (item.type_name().isEmpty() == false){
			QString type_name = item.type_name();
			for (const auto& item_type: m_types_ptr->types_list){
				if (item_type.name() == item.type_name()){
					result.setValue("file:types_config/" + item_type.image_path());
					break;
				}
			}
		}
	} else if(role == TypeName){
		result.setValue(item.type_name());
	}
	else if (role == Column){
		result.setValue(index.column());
	} else if (role == Row){
		result.setValue(index.row());
	} else if( role == DataMembers){
		QString members;
		auto types_list = item.members();
		for (const auto& current_item: types_list)
		{
			members+= current_item.second;
			members+= " ";
		}
		result.setValue(members);
	} else if ( role == DataTypes){
		QString types;
		for (const auto& current_item: item.members()){
			types += current_item.first;
			types += " ";
		}
		result.setValue(types);
	}
	return result;
}

Q_INVOKABLE bool LevelModel::load_level(const QUrl& file_path)
{
	QUrl url(file_path);
	bool success = m_items_ptr->load(url.toLocalFile());
	emit dataChanged(createIndex(0, 0),	createIndex(rowCount() -1, columnCount() - 1));
	return success;
}

QString LevelModel::level_path()
{
	return m_items_ptr->getSrc_path();
}

bool LevelModel::save_level(const QUrl &file_path){
	QUrl url(file_path);
	bool success = m_items_ptr->save(url.toLocalFile());
	return success;
}

bool LevelModel::save_level()
{
	return m_items_ptr->save(m_items_ptr->getSrc_path());
}

void LevelModel::clear(){
	m_items_ptr->clear();
	emit dataChanged(createIndex(0, 0), createIndex(rowCount(), columnCount()));
}

void LevelModel::clear(const int index)
{
	int row = index%height;
	int column = index/height;
	m_types_ptr->counters[m_items_ptr->get(row,column).type_name()]--;;
	m_items_ptr->insert(row, column, Item());
	emit dataChanged(createIndex(row,column), createIndex(row, column));
}

void LevelModel::setDst(const int index)
{
	m_requester_row = index%height;
	m_requester_column = index/height;
}

void LevelModel::setDstType(const QString &type)
{
	m_requester_type = type;
	qDebug() << "type: " <<type;
}

bool LevelModel::response(const QString &data)
{
	bool success = false;
	QList<QString> fields_list = data.split(" ", QString::SkipEmptyParts);
	Item item = m_types_ptr->createItem(m_requester_type, fields_list);
	if (item.type_name().isEmpty() == false){
		success = true;
		m_items_ptr->insert(m_requester_row, m_requester_column, item);
		QModelIndex changed_element = createIndex(m_requester_row, m_requester_column);
		emit dataChanged(changed_element, changed_element);
	}
	return success;
}

void LevelModel::create(){
	m_items_ptr->create();
	emit dataChanged(createIndex(0, 0), createIndex(rowCount(), columnCount()));
}

bool TypesContainer::parseFromFile(const QString &file_path)
{
	bool success = false;
	types_list = parse_config(file_path);
	for (const auto& type: types_list)
	{
		counters[type.name()] = 0;
	}
	return success;
}


