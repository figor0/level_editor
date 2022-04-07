#include <general.h>

Item::Item(const QDomElement &elem)
{
	if (elem.isNull() == false){
		m_type_name = elem.tagName();
		QDomElement child = elem.firstChildElement();
		while(child.isNull() == false)
		{
			m_members.push_back({child.tagName(), child.text()});
			child = child.nextSiblingElement();
		}
	} else{
		qDebug() << "Empty root element";
	}
}

Item& Item::operator=(const Item &right)
{
	if (&right != this)
	{
		m_type_name = right.m_type_name;
		m_members = right.m_members;
	}
	return *this;
}

Item::Item(const Item &item):
	m_type_name(item.m_type_name),
	m_members(item.m_members)
{}

QDomElement Item::toElement()
{
	QDomDocument doc;
	QDomElement result = doc.createElement(m_type_name);
	for (const auto& item: m_members){
		QDomElement sub_element = doc.createElement(item.first);
		sub_element.appendChild(doc.createTextNode(item.second));
		result.appendChild(sub_element);
	}
	return result;
}

QString Item::type_name() const
{
	return m_type_name;
}

std::list<std::pair<QString, QString>> Item::members() const
{
    return m_members;
}

bool Item::operator==(const Item &other) const noexcept
{
    return m_type_name == other.m_type_name &&
           m_members == other.members();
}

bool Item::operator!=(const Item &other) const noexcept
{
    return !(*this == other);
}

QDomDocument prepareXml(const QString &root_tag,
						const QList<QString> &tags,
						const QList<QString> &values)
{
	QDomDocument doc;
	doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
	if (tags.size() == values.size()){
		auto root_head = doc.createElement(root_tag);
		assert(root_head.firstChildElement().isNull() == true);
		if (tags.size() != 0){
			auto tags_it = tags.begin();
			auto values_it = values.begin();
			for( ; tags_it != tags.end(); values_it++, tags_it++)
			{
				auto curr_elem = doc.createElement(*tags_it);
				QDomText value = doc.createTextNode(*values_it);
				curr_elem.appendChild(value);
				root_head.appendChild(curr_elem);
			}
		}
		doc.appendChild(root_head);
	} else{
		for (const auto& tag: tags){
			qDebug() <<"tag " <<tag;
		}
		for (const auto& value: values){
			qDebug() << "value " << value;
		}
		qDebug() << "not equeal sizes";
	}
	return doc;
}

ItemType::ItemType(QDomElement xml_mapping)
{
	m_name = xml_mapping.tagName();
	auto unique_elem = xml_mapping.firstChildElement("unique");
	assert(unique_elem.isNull() == false);
	assert(unique_elem.text() == "false"
		   || unique_elem.text() == "true");
	unique_elem.text() == "true" ? m_unique_flag = true :
			m_unique_flag = false;
	auto image_path_elem = xml_mapping.firstChildElement("image_path");
	assert(image_path_elem.isNull() == false);
	m_image_path = image_path_elem.text();
	auto schema_element = xml_mapping.firstChildElement("xs:schema");
	assert(schema_element.isNull() == false);
	auto element_fields = schema_element.firstChild().
										firstChild().
										firstChild().childNodes();
	for (int i = 0; i < element_fields.size(); i++){
		m_tags.push_back(element_fields.item(i).toElement().attribute("name"));
	}
	m_schema.load(buildPartAsGeneral(schema_element));
}

ItemType::ItemType(const ItemType &type):
	m_unique_flag(type.unique_flag()), m_name(type.name()),
	m_image_path(type.image_path()), m_schema(type.validator()),
	m_tags(type.m_tags)
{}

std::map<QString, QString> createMap(const QList<QString>& tags,
								 const QList<QString>& values)
{
	std::map<QString, QString> result;
	if (tags.size() == values.size()){
		auto tags_it = tags.begin();
		auto values_it = values.begin();
		for ( ; tags_it != tags.end(); ++tags_it, ++values_it)
		{
			result[*tags_it] = *values_it;
		}
	}
	return result;
}

Item ItemType::CreateItem(const QList<QString>& values)
{
	Item result;
	QDomDocument doc = prepareXml(m_name, m_tags, values);
	qDebug() << doc.toString();
	QXmlSchemaValidator validator;
	validator.setSchema(m_schema);
	if (validator.validate(doc.toByteArray()) == true){
		result.m_type_name = m_name;
		if (m_tags.size() == values.size())
		{
			if (m_tags.size() != 0){
				auto tags_it = m_tags.begin();
				auto values_it = values.begin();
				for (; tags_it != m_tags.end(); ++tags_it, ++values_it )
				{
					result.m_members.push_back({*tags_it, *values_it});
				}
			}
		}
	}
	return result;
}

bool openFile(QFile &file)
{
	bool success_flag = true;
	if (file.open(QIODevice::ReadOnly)){
		qDebug() << "File " << file.fileName() << " sucessfully opened";
		success_flag = true;
	} else {
		success_flag = false;
		qDebug() << "File " << file.fileName() << " not opened";
	}
	return success_flag;
}

QByteArray buildPartAsGeneral(const QDomElement &data)
{
	QByteArray init(init_string.toUtf8());
	QDomDocument doc;
	doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
	assert(doc.firstChildElement().isNull() == true);
	doc.appendChild(data);
	return doc.toByteArray();
}

std::list<ItemType> parse_config(const QString &file_path)
{
	std::list<ItemType> result;
	QFile file(file_path);
	openFile(file);
	QDomDocument doc;
	if (!doc.setContent(&file)){
		qDebug() << "Can't set schema doc";
	} else {
		QDomElement elem = doc.firstChildElement().firstChildElement();
		while( elem.isNull() == false)
		{
			result.push_back(ItemType(elem));
			elem = elem.nextSiblingElement();
		}
	}
	return result;
}

bool ItemType::unique_flag() const
{
	return m_unique_flag;
}

QString ItemType::image_path() const
{
	return m_image_path;
}

QString ItemType::name() const
{
	return m_name;
}

QXmlSchema ItemType::validator() const
{
	return m_schema;
}

QList<QString> ItemType::tags() const
{
	return m_tags;
}

bool ItemType::isValid() const {
	return m_schema.isValid();
}
