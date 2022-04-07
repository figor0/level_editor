#include <LevelsModel.h>
#include <QHash>
#include <QByteArray>
#include <QIcon>
#include <libs/parser/items_model.h>

QVariantMap LevelsModel::get(const int row, const int column) const
{
    QHash<int, QByteArray> names = roleNames();
    QHashIterator<int, QByteArray> i(names);
    QVariantMap res;
    QModelIndex idx = index(row, column);
    while(i.hasNext()) {
        i.next();
        QVariant data = idx.data(i.key());
        res[i.value()] = data;
    }
    return res;
}

LevelsModel::LevelsModel(std::shared_ptr<TypesContainer> types_ptr,
                         LevelsContainer container,
                         QObject *parent)
    : QAbstractListModel(parent),
      m_types_ptr(types_ptr),
      m_levels(container)
{
}

QHash<int, QByteArray> LevelsModel::roleNames() const
{
    static const QHash<int, QByteArray> roles_mapping
    {
        { Roles::LevelName, "name" },
        { Roles::Icon, "icon"},
        { Roles::LevelData, "data"}
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
    } else if ( role == Roles::LevelData){
        result = QVariant::fromValue(m_levels.at(index.row()));
    }
    return result;
}

bool LevelsModel::loadFromFile(const QString &path)
{
    LevelsFromXml parser;
    beginResetModel();
    QFile file(path);
    bool parsable = false;
    if ( file.open(QIODevice::ReadOnly)){
        auto data = file.readAll();
        parsable = parser.isValid(data);
        if ( parsable )
        {
            auto levels = parser.parse(data);
            m_levels.clear();
            m_levels.reserve(levels.size());
            for (const auto& level: levels){
                m_levels.push_back(LevelModel(std::make_shared<Level>(level),
                                   m_types_ptr));
            }
        }
    }
    endResetModel();
    return parsable;
}

std::vector<std::shared_ptr<Level>>
    fromModels(const std::vector<LevelModel>& models)
{
    decltype (fromModels({})) result;
    result.reserve(models.size());
    for(const auto& model: models)
    {
        result.push_back(model.items_ptr());
    }
    return result;
}

bool LevelsModel::saveToFile(const QString &path)
{
    bool success_flag = false;

    LevelsFromXml parser;
    QFile file(path);
    auto level_ptrs = fromModels(m_levels);

    quint64 writed_count = 0;
    if ( file.open(QIODevice::Truncate | QIODevice::WriteOnly) )
    {
        auto data = parser.serrialize(level_ptrs);
        quint64 data_size = static_cast<quint64>(data.size());
        writed_count = file.write(data);
        success_flag = true;
        if ( writed_count != data_size )
        {
            qDebug() << "Not equal data size = " << data_size
                     << " writed size " << writed_count;
        }
    }
    return success_flag;
}

void LevelsModel::appendModel()
{
    beginResetModel();
    m_levels.push_back(LevelModel(std::make_shared<Level>("SomeLevel"),
                                 m_types_ptr));
    endResetModel();
}
