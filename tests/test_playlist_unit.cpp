#include "filesystemscanner.h"
#include "playlistmodel.h"
#include <QSignalSpy>
#include <QTest>

class PlaylistUnitTests : public QObject {
    Q_OBJECT
private slots:
    void supportedAudioExtensionsAreAccepted()
    {
        QVERIFY(FileSystemScanner::isSupportedAudio("song.mp3"));
        QVERIFY(FileSystemScanner::isSupportedAudio("voice.WAV"));
        QVERIFY(FileSystemScanner::isSupportedAudio("mix.flac"));
    }

    void unsupportedFilesAreRejected()
    {
        QVERIFY(!FileSystemScanner::isSupportedAudio("cover.png"));
        QVERIFY(!FileSystemScanner::isSupportedAudio("notes.txt"));
    }

    void playlistModelAddsAndRemovesTrack()
    {
        PlaylistModel model;
        QSignalSpy spy(&model, &PlaylistModel::playlistChanged);
        model.addTrack(AudioTrack{"Track", "track.mp3", QUrl::fromLocalFile("track.mp3"), 0});
        QCOMPARE(model.rowCount(), 1);
        QCOMPARE(model.titleAt(0), QString("Track"));
        model.removeAt(0);
        QCOMPARE(model.rowCount(), 0);
        QVERIFY(spy.count() >= 2);
    }

    void invalidTrackIsNotAdded()
    {
        PlaylistModel model;
        model.addTrack(AudioTrack{});
        QCOMPARE(model.rowCount(), 0);
    }
};

QTEST_MAIN(PlaylistUnitTests)
#include "test_playlist_unit.moc"
