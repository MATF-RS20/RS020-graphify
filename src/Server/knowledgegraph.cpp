#include "knowledgegraph.h"

//Helper functions
int prepareArray(QStringList &query_params) {
    QString res;
    QMutableStringListIterator it(query_params);

    while(it.hasNext() == true) {
        auto value = it.next();
        if (value == "") {
            res.push_back("0");
            it.remove();
        }
        else {
            res.push_back("1");
        }
    }
    bool ok;
    return res.toInt(&ok, 2);
}

QPair<QStringList, int> prepareQuery(const QString &query) {
    QStringList query_params = query.split("::");
    int t_case = prepareArray(query_params);
    for (auto i : query_params) {
        qDebug() << i;
    }
    return QPair<QStringList, int>(query_params, t_case);
}


void KnowledgeGraph::initalizeGraph()
{
//    qDebug() << "Reading file on path " << m_inFile->fileName();

    if (!m_inFile->exists()) {
        qDebug() << "File doesnt exists";
        return;
    }

    m_inFile->open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray data = m_inFile->readAll();
    m_inFile->close();

    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(data, &errorPtr);
    if (doc.isNull()) {
        qDebug() << "Parse failed";
    }
    QJsonObject obj = doc.object();
    QJsonValue value = obj.value("data");
    QJsonArray jsonArray = value.toArray();

    parseJsonEntities(jsonArray);
}

void KnowledgeGraph::connectToEntry(const QPointer<Entity> e, QString flag, QMap<QString, QPointer<Entity>> *connectedEntities) {

    if(e == nullptr || connectedEntities->contains(e->getKey()))
        return;

    if (flag == "s") {
        m_sentries[0]->addEdge(QPointer(new Edge("CONTAINS", e, this)));
    }
    if (flag == "g") {
        m_gentries[0]->addEdge(QPointer(new Edge("CONTAINS", e, this)));
    }
    if (flag == "p") {
        m_pentries[0]->addEdge(QPointer(new Edge("CONTAINS", e, this)));
    }

    connectedEntities->insert(e->getKey(), e);
}

template <typename T>
QPointer<Entity> KnowledgeGraph::parse(const QJsonValue &v, QMap<QString, QPointer<Entity>>* entityMap, const QString &field)
{
    QString id = v.toObject().value(field).toString().trimmed();
    int idx = id.lastIndexOf(QString("/"));
    QString parsedId = id.right(id.length() - idx - 1);

    if (parsedId == "")
        return nullptr;

    if (!entityMap->contains(parsedId)) {
        QString label = v.toObject().value(field+"Label").toString();
        Entity* parsed;
        if (field == "title") {
            parsed = new T(parsedId, label, QPointer(new Metadata("url", "https://www.youtube.com/embed/u-ndajHaih8")), this);
        } else {
            parsed = new T(parsedId, label, nullptr, this);
        }

        entityMap->insert(parsedId, parsed);
        m_entities.push_back(QPointer(parsed));

        return QPointer<Entity>(parsed);
    } else {
        return entityMap->value(parsedId);
    }
    return nullptr;
}



void KnowledgeGraph::parseJsonEntities(const QJsonArray &arr)
{
    Entity *p = new PEntryPoint("P1", "P1");
    m_pentries.push_back(QPointer(p));
    p = new SEntryPoint("S1", "S1");
    m_sentries.push_back(QPointer(p));
    p = new GEntryPoint("G1", "G1");
    m_gentries.push_back(QPointer(p));
    QMap<QString, QPointer<Entity>> entityMap;
    QMap<QString, QPointer<Entity>> connectedEntities;
    QSet<QString> knownEntites;
    foreach(const QJsonValue &v, arr) {
        auto song = parse<Song>(v, &entityMap, "title");
        auto performer = parse<Performer>(v, &entityMap, "performer");
        auto genre = parse<Genre>(v, &entityMap, "genre");

        connectToEntry(song, "s", &connectedEntities);
        connectToEntry(performer, "p", &connectedEntities);
        connectToEntry(genre, "g", &connectedEntities);


//        Connect the dots
        performer->addEdge(QPointer(new Edge("SINGS", song, this)));
        song->addEdge(QPointer(new Edge("SINGED_BY", performer, this)));
        if (genre != nullptr) {
            song->addEdge(QPointer(new Edge("TYPE_OF", genre, this)));
            genre->addEdge(QPointer(new Edge("TYPE_FOR", song, this)));
        }
    }
/*
    foreach(const auto& e, entityMap.values()) {
        if (e->getType() == "PERFORMER") {
            qDebug() << e->getType() << e->getKey() << e->getValue();

            foreach(const auto& edge, e->getEdges())
                qDebug() << "\t" << edge->getType() << edge->getPointsTo()->getValue();
        }
    }

    qDebug() << "====================================";

    foreach(const auto& e, entityMap.values()) {
        if (e->getType() == "SONG") {
            qDebug() << e->getType() << e->getKey() << e->getValue();

            foreach(const auto& edge, e->getEdges())
                qDebug() << "\t" << edge->getType() << edge->getPointsTo()->getValue();
        }
    }

    qDebug() << "====================================";

    foreach(const auto& e, entityMap.values()) {
        if (e->getType() == "GENRE") {
            qDebug() << e->getType() << e->getKey() << e->getValue();

            foreach(const auto& edge, e->getEdges())
                qDebug() << "\t" << edge->getType() << edge->getPointsTo()->getValue();
        }
    }*/
}

KnowledgeGraph::KnowledgeGraph(const QString category, QObject *parent)
    : QObject(parent)

{
    for (auto pair : m_category) {
        if (pair.first == category)
            m_inFile->setFileName(pair.second);
    }

    initalizeGraph();

//    QVector<QString> res = traverseProcess("::Trap::");
//    for (auto r : res) {
//        qDebug() << "hopa " << r;
//    }

}

QVector<QString> KnowledgeGraph::traverseProcess(const QString &query) {
    QPair<QStringList, int> prepared = prepareQuery(query);
    QVector<QString> res = traverse(prepared.first, prepared.second);
    return res;
}

//QVector<QString> KnowledgeGraph::packData(QVector<Song*> data) const
//{
//    qDebug() << "Packing data...";
//    QVector<QString> result;
//    foreach(auto &s, data) {
//        result.push_back(QString(
//                          s->getValue()+","
//                         +s->getMetadataValue()+","
//                         +s->getRelatedEntity("TYPE_OF")+","
//                         +s->getRelatedEntity("PERFORMED_BY")
//                                 ));
//    }

//    qDebug() << "Packed:" << result;


//    return result;
//}

QVector<QString> KnowledgeGraph::traverse(const QStringList &query_params, const int &t_case) const {
    QVector<QString> res;
    PerformerGenreTraverse pgT;
    PerformerTraverse pT;
    SongTraverse sT;
    TraverseBehavior *t = nullptr;
    switch (t_case) {
        case 2:
            t = &sT;
            res = t->traverse(query_params, m_sentries);
            break;
        case 4:
            t = &pT;
            res = t->traverse(query_params, m_pentries);
            break;
        case 5:
            t = &pgT;
            res = t->traverse(query_params, m_pentries);
            break;
        default:
            break;
    }
    return res;
}
