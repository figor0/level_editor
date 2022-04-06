#include <LevelsModel.h>
#include <QHash>
#include <QByteArray>
#include <QIcon>
#include <libs/parser/items_model.h>

LevelsModel::LevelsModel(std::shared_ptr<TypesContainer> types_ptr,
                         LevelsContainer container,
                         QObject *parent)
    : QAbstractListModel(parent),
      m_types_ptr(types_ptr),
      m_levels(container)
{
}

int LevelsModel::size() const
{
    return m_levels.size();
}

LevelModel *LevelsModel::current()
{
    return nullptr;
}

QHash<int, QByteArray> LevelsModel::roleNames() const
{
    static const QHash<int, QByteArray> roles_mapping
    {
        { Roles::LevelName, "level_name" },
        { Roles::Icon, "icon" }
    };
    return roles_mapping;
}

int LevelsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_levels.size();
}

QVariant LevelsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};
    QVariant result;
    if ( role == Roles::LevelName ){
//        result = m_levels.at(index.row())->name();
        result = "LevelName";
    } else if ( role == Roles::Icon ) {
        result = QIcon();
    }
    return result;
}

bool LevelsModel::loadFromFile(const QString &path)
{
    LevelsFromXml parser;
    bool parsable = parser.ILevelsIO::setTarget(path);
    beginResetModel();
    if ( parsable )
    {
        auto levels = parser.ILevelsIO::parse(path);
        m_levels.clear();
        m_levels.reserve(levels.size());
        for (const auto& level: levels){
            m_levels.push_back(LevelModel(std::make_shared<Level>(level),
                               m_types_ptr));
        }
    }
    endResetModel();
    return parsable;
}

bool LevelsModel::saveToFile(const QString &path)
{
    LevelsFromXml parser;
    auto success = parser.setTarget(path);
    if ( success == true ){
        pars
    }
    return success;
}
