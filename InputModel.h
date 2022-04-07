#pragma once
#include <QAbstractTableModel>
#include <general.h>
#include <items_model.h>
#include <memory>
#include <list>
#include <QModelIndex>

class InputModel: public QAbstractListModel{
	Q_OBJECT
public:
	enum Roles {
		TypeName = Qt::UserRole + 1,
		FieldValue
	};
	InputModel(QObject* parent = nullptr);
	QHash<int, QByteArray> roleNames() const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	bool setData(const QModelIndex &index,
				 const QVariant &value,
				 int role = Qt::EditRole) override;
	Q_INVOKABLE bool setFieldTypes(const QString& types_chain);
	Q_INVOKABLE bool setField(const QString& types_chain,
							  const QString& fields_chain);
	Q_INVOKABLE void clear();
	Q_INVOKABLE QString getData();
private:
	std::vector<std::pair<QString, QString>> m_fields;
};

