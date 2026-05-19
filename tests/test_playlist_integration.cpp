#include "filesystemscanner.h"
#include "playlistmodel.h"
#include "playlistrepository.h"
#include <QFile>
#include <QTemporaryDir>
#include <QTest>

class PlaylistIntegrationTests : public QObject {
    Q_OBJECT
private slots:
    void scannerFindsOnlyAudioFilesInFolder()
    {
        QTemporaryDir dir;
        QFile mp3(dir.filePath("track.mp3"));
        QFile txt(dir.filePath("notes.txt"));
        QVERIFY(mp3.open(QIODevice::WriteOnly));
        QVERIFY(txt.open(QIODevice::WriteOnly));
        mp3.close();
        txt.close();

        FileSystemScanner scanner;
        const QList<AudioTrack> tracks = scanner.scanFolder(dir.path());
        QCOMPARE(tracks.size(), 1);
        QCOMPARE(tracks.first().title, QString("track"));
    }

    void repositoryPersistsAndRestoresPlaylist()
    {
        QTemporaryDir dir;
        const QString dbPath = dir.filePath("playlist.sqlite");
        PlaylistRepository repository(dbPath);
        QList<AudioTrack> tracks = {
            AudioTrack{"Track 1", "track1.mp3", QUrl::fromLocalFile("track1.mp3"), 1000},
            AudioTrack{"Track 2", "track2.wav", QUrl::fromLocalFile("track2.wav"), 2000},
        };

        QVERIFY(repository.save(tracks));
        const QList<AudioTrack> restored = repository.load();
        QCOMPARE(restored.size(), 2);
        QCOMPARE(restored.at(1).title, QString("Track 2"));
    }

    void scannerRepositoryAndModelWorkTogether()
    {
        QTemporaryDir dir;
        QFile audio(dir.filePath("episode.ogg"));
        QVERIFY(audio.open(QIODevice::WriteOnly));
        audio.close();

        FileSystemScanner scanner;
        PlaylistModel model;
        for (const AudioTrack &track : scanner.scanFolder(dir.path())) {
            model.addTrack(track);
        }
        QCOMPARE(model.rowCount(), 1);

        PlaylistRepository repository(dir.filePath("playlist.sqlite"));
        QVERIFY(repository.save(model.tracks()));
        model.clear();
        model.setTracks(repository.load());
        QCOMPARE(model.rowCount(), 1);
        QCOMPARE(model.titleAt(0), QString("episode"));
    }
};

QTEST_MAIN(PlaylistIntegrationTests)
#include "test_playlist_integration.moc"
