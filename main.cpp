#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QAbstractTableModel>
#include <QQmlContext>
#include <QVariant>
#include <InputModel.h>
#include <LevelModel.h>
#include <TypesModel.h>
#include <LevelsModel.h>

const QString config_path = "types_config/items_schema.xsd";

struct Containers{
	Containers():
        area_ptr(std::make_shared<Level>()),
		types_ptr(std::make_shared<TypesContainer>())
	{
		types_ptr->parseFromFile(config_path);
	}
    std::shared_ptr<Level> area_ptr;
	std::shared_ptr<TypesContainer> types_ptr;
};

struct Models{
	Models(const Containers& containers):
		level_model(containers.area_ptr, containers.types_ptr),
		types_model(containers.types_ptr),
		input_model()
	{}
	LevelModel level_model;
	TypesModel types_model;
	InputModel input_model;
    LevelsModel levels_model;
    void registration(QQmlContext* context) {
		context->setContextProperty("level_model", &level_model);
		context->setContextProperty("types_model", &types_model);
		context->setContextProperty("field_model", &input_model);
        context->setContextProperty("levels_model", &levels_model);
	}
};

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);
	app.setOrganizationName("nope");
	app.setOrganizationDomain("nope");
	QQmlApplicationEngine engine;
	Containers conteiners;
	Models models(conteiners);
	models.registration(engine.rootContext());
	const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
					 &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
