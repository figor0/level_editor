#include <TypesModel.h>

TypesModel::TypesModel(std::shared_ptr<TypesContainer> types_ptr, QObject *parent):
	QAbstractListModel(parent), m_types_ptr(types_ptr)
{}

QHash<int, QByteArray> TypesModel::roleNames() const
{
	return {
		{ImagePath, "image_path"},
		{TypeName, "type_name"},
		{TypeFields, "type_fields"},
		{UniqueFlag, "unique"},
		{Amount, "amount"}
	};
}

int TypesModel::rowCount(const QModelIndex &parent) const
{
    return 1;
}

int TypesModel::columnCount(const QModelIndex &parent) const
{
    return m_types_ptr->types_list.size();
}

QVariant TypesModel::data(const QModelIndex &index, int role) const
{
	QVariant result;
	if (index.isValid() == true){
		auto need_it = m_types_ptr->types_list.begin();
        std::advance(need_it, index.column());
		switch (role) {
		case Roles::ImagePath:
		{
			result.setValue("file:types_config/" + need_it->image_path());
			break;
		}
		case Roles::TypeName:
		{
			result.setValue(need_it->name());
			break;
		}
		case Roles::TypeFields:
		{
			QString tags;
			for (const auto& item: need_it->tags()){
				tags+= item;
				tags+= ' ';
			}
			result.setValue(tags);
			break;
		}
		case Roles::UniqueFlag:
		{
			result.setValue(need_it->unique_flag());
			break;
		}
		case Roles::Amount:
		{
			result.setValue(m_types_ptr->counters.at(need_it->name()));
			break;
		}
		}
	}
	return result;
}
