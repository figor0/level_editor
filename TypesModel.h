#pragma once
#include <QAbstractTableModel>
#include <general.h>
#include <items_model.h>
#include <memory>
#include <list>
#include <QModelIndex>
#include <general.h>
#include <items_model.h>

class TypesModel: public QAbstractListModel
{
	Q_OBJECT
public:
	enum Roles {
		ImagePath = Qt::UserRole + 1,
		TypeName,
		TypeFields,
		UniqueFlag,
		Amount
	};
	TypesModel(std::shared_ptr<TypesContainer> types_ptr,
			   QObject* parent = nullptr);
	QHash<int, QByteArray> roleNames() const override;
	int rowCount(const QModelIndex &parent) const override;
	int columnCount(const QModelIndex &parent) const override;
	QVariant data(const QModelIndex &index, int role) const override;
private:
	std::shared_ptr<TypesContainer> m_types_ptr;
};

