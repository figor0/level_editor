#include <InputModel.h>

InputModel::InputModel(QObject *parent):
	QAbstractListModel(parent)
{}

QHash<int, QByteArray> InputModel::roleNames() const{
	return {
		{TypeName, "type_name"},
		{FieldValue, "field_value"}
	};
}

int InputModel::rowCount(const QModelIndex &parent) const
{
	return m_fields.size();
}

QVariant InputModel::data(const QModelIndex &index, int role) const{
	QVariant result;
	if (index.isValid() == true){
		if (role == TypeName){
			result.setValue(m_fields.at(index.row()).first);
		} else if (role == FieldValue){
			result.setValue(m_fields.at(index.row()).second);
		}
	}
	return result;
}

bool InputModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!hasIndex(index.row(), index.column(), index.parent()) || !value.isValid())
		return false;
	if (role == FieldValue){
		m_fields.at(index.row()).second = value.toString();
	}
	return true;
}

bool InputModel::setFieldTypes(const QString &types_chain)
{
	clear();
	QStringList fields_list = types_chain.split(" ", QString::SkipEmptyParts);
	bool success = false;
	if (fields_list.size() > 0){
		success = true;
		beginInsertRows(QModelIndex(), 0, fields_list.size() - 1);
		for (const auto& field: fields_list)
		{
			m_fields.push_back({field, ""});
			qDebug() << m_fields.back();
		}
		endInsertRows();
	}
	return success;
}

bool InputModel::setField(const QString &types_chain,
								 const QString &fields_chain)
{
	clear();
	QStringList types_list = types_chain.split(" ", QString::SkipEmptyParts);
	QStringList fields_list = fields_chain.split(" ", QString::SkipEmptyParts);
	bool success = false;
	if (types_list.size() == fields_list.size())
	{
		success = true;
		if (types_list.empty() == false){
			auto types_it = types_list.begin();
			auto fields_it = fields_list.begin();
			beginInsertRows(QModelIndex(), 0, fields_list.size() - 1);
			while(types_it != types_list.end())
			{
				m_fields.push_back({*types_it, *fields_it});
				types_it++;
				fields_it++;
			}
			endInsertRows();
		}
	}
	return success;
}

void InputModel::clear()
{
	if (m_fields.size() > 0){
		beginRemoveRows(QModelIndex(), 0, m_fields.size() - 1);
		m_fields.clear();
		endRemoveRows();
	}
}

QString InputModel::getData()
{
	QString result;
	for (const auto& item: m_fields){
		result+= item.second;
		result+= " ";
	}
	qDebug() << result;
	return result;
}
