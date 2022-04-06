#pragma once

#include <memory>
#include <QAbstractListModel>

#include <LevelModel.h>

class Navigator;

class LevelsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(Roles)
public:
    using LevelsContainer = std::vector<LevelModel>;
    enum Roles {
        LevelName = Qt::UserRole + 1,
        Icon,
    };
    LevelsModel(std::shared_ptr<TypesContainer> types_ptr,
                LevelsContainer levels = {},
                QObject* parent = nullptr);
    Q_INVOKABLE int size() const;
    Q_INVOKABLE LevelModel* current();
    Q_INVOKABLE bool loadFromFile(const QString& path);
    Q_INVOKABLE bool saveToFile(const QString& path);
//QAbstractItemModel
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    std::shared_ptr<TypesContainer> m_types_ptr;
    LevelsContainer m_levels;
    LevelModel* m_current;
};
