#include <QDomDocument>
#include <QString>
#include <QtXmlPatterns/QXmlSchemaValidator>
#include <QtXmlPatterns/QXmlSchema>
#include <QFile>
#include <QDebug>
#include <QDomElement>
#include <iostream>
#include <list>
#include <QList>
#include <general.h>
#include <model.h>

void example1()
{
	QFile schema_file("schema.xsd");
	QFile item_file("item.xml");
	openFile(schema_file);
	openFile(item_file);
	QXmlSchema schema;
	if (schema.load(&schema_file) == true)
	{
		qDebug() << "Schema file successfuly loaded";
		QXmlSchemaValidator validator(schema);
		if (validator.validate(&item_file, QUrl::fromLocalFile(item_file.fileName())))
			qDebug() << "instance document is valid";
		else
			qDebug() << "instance document is invalid";
	} else{
		qDebug() << "Schema file not loaded";
	}
}

int main(int argc, char *argv[])
{
	auto types_container = parse_config("items_schema.xsd");
	return 0;
}
