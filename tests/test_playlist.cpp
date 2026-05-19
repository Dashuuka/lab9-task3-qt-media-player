#include "filesystemscanner.h"
#include "playlistmodel.h"
#include "playlistrepository.h"
#include <QTemporaryDir>
#include <QTest>

class PlaylistTests : public QObject {
    Q_OBJECT
private slots:
    void filtersAudioFiles()
    {
        QVERIFY(FileSystemScanner::isSupportedAudio("song.mp3"));
        QVERIFY(FileSystemScanner::isSupportedAudio("song.FLAC"));
        QVERIFY(!FileSystemScanner::isSupportedAudio("cover.png"));
    }

    void addsAndRemovesTracks()
    {
        PlaylistModel model;
        model.addTrack(AudioTrack{"Track", "track.mp3", QUrl::fromLocalFile("track.mp3"), 0});
        QCOMPARE(model.rowCount(), 1);
        model.removeAt(0);
        QCOMPARE(model.rowCount(), 0);
    }

    void savesAndRestoresSqlite()
    {
        QTemporaryDir dir;
        const QString dbPath = dir.filePath("playlist.sqlite");
        PlaylistRepository repository(dbPath);
        QList<AudioTrack> tracks = {AudioTrack{"Track", "track.mp3", QUrl::fromLocalFile("track.mp3"), 0}};
        QVERIFY(repository.save(tracks));
        QCOMPARE(repository.load().size(), 1);
    }
};

QTEST_MAIN(PlaylistTests)
#include "test_playlist.moc"
