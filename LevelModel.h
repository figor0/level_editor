#pragma once
#include <QAbstractTableModel>
#include <general.h>
#include <items_model.h>
#include <memory>
#include <list>
#include <QModelIndex>

class LevelModel : public QAbstractTableModel
{
	Q_OBJECT
	Q_ENUMS(Roles)
public:
	enum Roles {
		ImagePath = Qt::UserRole + 1,
		TypeName,
		DataMembers,
		DataTypes,
		Row,
		Column
	};
    LevelModel(std::shared_ptr<Level> items_ptr,
			   std::shared_ptr<TypesContainer> types_ptr,
			   QObject* parent = nullptr);
	QHash<int, QByteArray> roleNames() const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	Q_INVOKABLE void create();
	Q_INVOKABLE bool load_level(const QUrl &file_path);
	Q_INVOKABLE QString level_path();
	Q_INVOKABLE bool save_level(const QUrl& file_path);
	Q_INVOKABLE bool save_level();
	Q_INVOKABLE void clear();
	Q_INVOKABLE void clear(const int index);
	Q_INVOKABLE void setDst(const int index);
	Q_INVOKABLE void setDstType(const QString& type);
	Q_INVOKABLE bool response(const QString& data);
private:
	static constexpr int width = 30;
	static constexpr int height = 15;
	static constexpr int size = width * height;
	QString m_image_path;
    std::shared_ptr<Level> m_items_ptr;
	std::shared_ptr<TypesContainer> m_types_ptr;
	int m_requester_row, m_requester_column;
	QString m_requester_type;
};
